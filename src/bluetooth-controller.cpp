#include "bluetooth-controller.hpp"

#include <iomanip>
#include <iostream>
#include <cstdint>
#include "raylib.h"
#include "bluetooth-utils.hpp"

bool BluetoothController::initialized = false;
SimpleBLE::Adapter BluetoothController::activeAdapter;

std::atomic<int> BluetoothController::activeConnectionThreads = 0;
std::atomic<int> BluetoothController::activeDisconnectionThreads = 0;
std::mutex BluetoothController::connectedDevicesMtx;

std::unordered_map<BluetoothController::ServiceType, SimpleBLE::BluetoothAddress> BluetoothController::serviceDeviceMap;

std::string BluetoothController::heartRateMeasurementCharacteristic = "00002a37-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::cyclingPowerMeasurementCharacteristic = "00002a63-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::cyclingCadenceMeasurementCharacteristic = "00002a5b-0000-1000-8000-00805f9b34fb";

std::string BluetoothController::fitnessMachine_FMF = "00002acc-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::fitnessMachine_IBD = "00002ad2-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::fitnessMachine_TS = "00002ad3-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::fitnessMachine_SRLR = "00002ad6-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::fitnessMachine_SPR = "00002ad8-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::fitnessMachine_FMCP = "00002ad9-0000-1000-8000-00805f9b34fb";
std::string BluetoothController::fitnessMachine_FMS = "00002ada-0000-1000-8000-00805f9b34fb";

/// Devices found in scan.
std::vector<SimpleBLE::Peripheral> BluetoothController::foundDevices;

std::vector<SimpleBLE::Peripheral> BluetoothController::connectedDevices;

int* BluetoothController::heartRateValue = NULL;
int* BluetoothController::cyclingPowerValue = NULL;
int* BluetoothController::cyclingCadenceValue = NULL;

int BluetoothController::lastCrankRevolutions = 0;
int BluetoothController::lastCrankEventTime = 0;


bool BluetoothController::BluetoothSupported()
{
	return SimpleBLE::Adapter::bluetooth_enabled();
}

int BluetoothController::InitializeBluetooth()
{
	std::vector<SimpleBLE::Adapter> availableAdapters = SimpleBLE::Adapter::get_adapters();
	if (availableAdapters.size() < 1) {
		return EXIT_FAILURE;
	}
	activeAdapter = availableAdapters.at(0);
	TraceLog(LOG_INFO, "BLE Adapter: %s", activeAdapter.identifier().c_str());

	// ToDo: Set callbacks for scanning. Start, Stop, on found.
	activeAdapter.set_callback_on_scan_start([]() {
		foundDevices.clear();
		TraceLog(LOG_INFO, "BLE: Scan started");
	});

	// Set the callback to be called when the scan stops
	activeAdapter.set_callback_on_scan_stop([]() {
		TraceLog(LOG_INFO, "BLE: Scan stopped");
	});

	// Set the callback to be called when the scan finds a new peripheral
	activeAdapter.set_callback_on_scan_found([](SimpleBLE::Peripheral peripheral) {
		TraceLog(LOG_INFO, "BLE: Peripheral found: %s, %s", peripheral.identifier().c_str(), peripheral.address().c_str());
		if (peripheral.identifier().length() != 0) {
			std::vector<SimpleBLE::Service> avalibleServices = peripheral.services();
			TraceLog(LOG_INFO, "BLE: \t\tP Service count: %d", avalibleServices.size());
			for (int i = 0; i < avalibleServices.size(); i++) {
				TraceLog(LOG_INFO, "BLE: \t\t\tService: %s, %s", BleUtils::ToString(BleUtils::GetServiceType(avalibleServices.at(i).uuid())).c_str(), avalibleServices.at(i).uuid().c_str());
			}
		}
		foundDevices.push_back(peripheral);
	});

	// Set the callback to be called when a peripheral property has changed
	activeAdapter.set_callback_on_scan_updated([](SimpleBLE::Peripheral peripheral) {
		if (peripheral.identifier().length() != 0) { // Suppress update messages for unnamed devices
			//TraceLog(LOG_INFO, "BLE: Peripheral updated: %s, %s", peripheral.identifier().c_str(), peripheral.address().c_str());
		}
	});


	initialized = true;
	return EXIT_SUCCESS;
}

void BluetoothController::DeinitializeBluetooth()
{
	BluetoothController::StopScan();

	// Any other future actions at program close

	return;
}

void BluetoothController::StartScan()
{
	if (!activeAdapter.scan_is_active()) {
		activeAdapter.scan_start();
	}
}

void BluetoothController::StopScan()
{
	if (activeAdapter.scan_is_active()) {
		activeAdapter.scan_stop();
	}
}

std::vector<SimpleBLE::Peripheral> BluetoothController::GetDiscoveredDevices()
{
	return foundDevices;
}

std::vector<SimpleBLE::Peripheral> BluetoothController::GetConnectedDevices()
{
	return connectedDevices;
}

void BluetoothController::ConnectToDevice(SimpleBLE::Peripheral device)
{
	activeConnectionThreads++;
	TraceLog(LOG_INFO, "BLE: Connecting to device: %s, %s", device.identifier().c_str(), device.address().c_str());
	try {
		device.connect();
		TraceLog(LOG_INFO, "BLE: Sucessfully connected to device: %s, %s", device.identifier().c_str(), device.address().c_str());

		connectedDevicesMtx.lock();
		try {
			connectedDevices.push_back(device);
		}
		catch (...) {

		}
		connectedDevicesMtx.unlock();
	}
	catch (...) {
		TraceLog(LOG_INFO, "BLE: Failed to connected to device: %s, %s", device.identifier().c_str(), device.address().c_str());
	}
	activeConnectionThreads--;
}

int BluetoothController::GetActiveConnectionThreads()
{
	return activeConnectionThreads;
}

void BluetoothController::DisconnectFromDevice(SimpleBLE::Peripheral device)
{
	activeDisconnectionThreads++;
	TraceLog(LOG_INFO, "BLE: Disconnecting from device: %s, %s", device.identifier().c_str(), device.address().c_str());
	try {
		device.disconnect();
		TraceLog(LOG_INFO, "BLE: Sucessfully disconnected from device: %s, %s", device.identifier().c_str(), device.address().c_str());
		
		connectedDevicesMtx.lock();
		try {
			// Search for then remove the connected device from connected list.
			for (int i = 0; i < connectedDevices.size(); i++) {
				if (connectedDevices.at(i).address() == device.address()) {
					// remove device
					connectedDevices.erase(std::next(connectedDevices.begin(), i));
					break;
				}
			}
		}
		catch (...) {

		}
		connectedDevicesMtx.unlock();
	}
	catch (...) {
		TraceLog(LOG_INFO, "BLE: Failed to disconnect from device: %s, %s", device.identifier().c_str(), device.address().c_str());
	}
	activeDisconnectionThreads--;
}

int BluetoothController::GetActiveDisconnectionThreads()
{
	return activeDisconnectionThreads;
}

void BluetoothController::SetServiceDeviceMap(ServiceType type, SimpleBLE::BluetoothAddress address)
{
	serviceDeviceMap[type] = address;
}

int BluetoothController::SubscribeToHeartRate(int* heartRateReference)
{
	heartRateValue = heartRateReference; // Set Value for callback

	return SubscribeToGenericNotify(HEART_RATE, HeartRateCallback);
}

int BluetoothController::SubscribeToCyclingPower(int* cyclingPowerReference)
{
	cyclingPowerValue = cyclingPowerReference; // Set Value for callback

	return SubscribeToGenericNotify(CYCLING_POWER, CyclingPowerCallback);
}

int BluetoothController::SubscribeToCadence(int* cadenceReference)
{
	cyclingCadenceValue = cadenceReference; // Set Value for callback

	return SubscribeToGenericNotify(CYCLING_SPEED_CADENCE, CadenceCallback);
}

void BluetoothController::RequestTrainerControl(bool* complete, int* response)
{
	// This function assumes that the Power Target Setting is supported (bit 3 of Fitness Machine Feature Target Settings Features Field)

	// This functuion uses the FMCP characteristic

	ServiceType type = FITNESS_MACHINE;
	SimpleBLE::BluetoothAddress targetDeviceAddress = serviceDeviceMap[type];

	//std::cout << "Heart Rate Device Address: " << targetDeviceAddress << " (" << targetDeviceAddress.length() << ")" << std::endl;
	if (targetDeviceAddress.length() == 0) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}

	// Step 1: Find device
	SimpleBLE::Peripheral device;
	bool foundDevice = false;

	connectedDevicesMtx.lock();
	for (int i = 0; i < connectedDevices.size(); i++) {
		if (connectedDevices.at(i).address() == targetDeviceAddress) {
			device = connectedDevices.at(i);
			foundDevice = true;
			break;
		}
	}
	connectedDevicesMtx.unlock();

	if (!foundDevice) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}

	// Step 2 Find Service
	std::vector<SimpleBLE::Service> deviceServices = device.services();

	SimpleBLE::Service service;
	bool foundService = false;

	for (int i = 0; i < deviceServices.size(); i++) {
		if (deviceServices.at(i).uuid() == GetServiceUuid(type)) {
			service = deviceServices.at(i);
			foundService = true;
			break;
		}
	}

	if (!foundService) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}

	// Step 3: Find characteristic
	std::vector<SimpleBLE::Characteristic> serviceCharacteristics = service.characteristics();

	SimpleBLE::Characteristic characteristic;
	bool foundCharacteristic = false;

	std::string characteristicUUID = fitnessMachine_FMCP; // Fitness Machine Control Point

	//std::cout << "Characteristics: ";

	for (int i = 0; i < serviceCharacteristics.size(); i++) {
		if (serviceCharacteristics.at(i).uuid() == characteristicUUID) {
			characteristic = serviceCharacteristics.at(i);
			foundCharacteristic = true;
		}

		//// Print out characteristics
		//std::cout << serviceCharacteristics.at(i).uuid();
		//if (i + 1 < serviceCharacteristics.size()) {
		//	std::cout << ", ";
		//}
	}

	std::cout << std::endl;

	if (!foundCharacteristic) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}
	else {
		//std::cout << "Fitness Machine Control Point Present." << std::endl;
	}

	// Assuming that the control has been requested set the target power using Op Code: 0x05
	char dataSend[1];
	char opCode = Convert8BitValueToLittleEndian(0x00);

	dataSend[0] = opCode;
	device.write_request(service.uuid(), characteristic.uuid(), std::string(dataSend));

	// Write opcodes and check return values
	// Not sure how to check return value

	*complete = true;
	*response = EXIT_SUCCESS;
	return;
}

void BluetoothController::SetTrainerTargetPower(int targetPower, bool* complete, int* response)
{
	// This function assumes that the Power Target Setting is supported (bit 3 of Fitness Machine Feature Target Settings Features Field)

	// This functuion uses the FMCP characteristic

	ServiceType type = FITNESS_MACHINE;
	SimpleBLE::BluetoothAddress targetDeviceAddress = serviceDeviceMap[type];

	//std::cout << "Heart Rate Device Address: " << targetDeviceAddress << " (" << targetDeviceAddress.length() << ")" << std::endl;
	if (targetDeviceAddress.length() == 0) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}

	// Step 1: Find device
	SimpleBLE::Peripheral device;
	bool foundDevice = false;

	connectedDevicesMtx.lock();
	for (int i = 0; i < connectedDevices.size(); i++) {
		if (connectedDevices.at(i).address() == targetDeviceAddress) {
			device = connectedDevices.at(i);
			foundDevice = true;
			break;
		}
	}
	connectedDevicesMtx.unlock();

	if (!foundDevice) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}

	// Step 2 Find Service
	std::vector<SimpleBLE::Service> deviceServices = device.services();

	SimpleBLE::Service service;
	bool foundService = false;

	for (int i = 0; i < deviceServices.size(); i++) {
		if (deviceServices.at(i).uuid() == GetServiceUuid(type)) {
			service = deviceServices.at(i);
			foundService = true;
			break;
		}
	}

	if (!foundService) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}

	// Step 3: Find characteristic
	std::vector<SimpleBLE::Characteristic> serviceCharacteristics = service.characteristics();

	SimpleBLE::Characteristic characteristic;
	bool foundCharacteristic = false;

	std::string characteristicUUID = fitnessMachine_FMCP; // Fitness Machine Control Point

	//std::cout << "Characteristics: ";

	for (int i = 0; i < serviceCharacteristics.size(); i++) {
		if (serviceCharacteristics.at(i).uuid() == characteristicUUID) {
			characteristic = serviceCharacteristics.at(i);
			foundCharacteristic = true;
		}

		// Print out characteristics
		/*std::cout << serviceCharacteristics.at(i).uuid();
		if (i + 1 < serviceCharacteristics.size()) {
			std::cout << ", ";
		}*/
	}

	//std::cout << std::endl;

	if (!foundCharacteristic) {
		*complete = true;
		*response = EXIT_FAILURE;
		return;
	}
	else {
		//std::cout << "Fitness Machine Control Point Present." << std::endl;
	}

	// Assuming that the control has been requested set the target power using Op Code: 0x05
	char dataSend[3];
	char opCode = Convert8BitValueToLittleEndian(0x05);
	uint16_t paramaterValue = Convert16BitValueToLittleEndian((uint16_t)targetPower);
	_2Bytes_t paramaterValue2B;
	paramaterValue2B._2byte = paramaterValue;

	dataSend[0] = opCode;
	dataSend[1] = paramaterValue2B.byte1;
	dataSend[2] = paramaterValue2B.byte2;

	// Convert Values to litte edian
	device.write_request(service.uuid(), characteristic.uuid(), std::string(dataSend));

	// Write opcodes and check return values
	// Not sure how to check return value

	*complete = true;
	*response = EXIT_SUCCESS;
	return;
}

BluetoothController::ServiceType BluetoothController::GetServiceType(SimpleBLE::BluetoothUUID uuid)
{
	if (uuid == "0000180d-0000-1000-8000-00805f9b34fb") {
		return BluetoothController::HEART_RATE;
	}
	else if (uuid == "00001818-0000-1000-8000-00805f9b34fb") {
		return BluetoothController::CYCLING_POWER;
	}
	else if (uuid == "00001816-0000-1000-8000-00805f9b34fb") {
		return BluetoothController::CYCLING_SPEED_CADENCE;
	}
	else if (uuid == "00001826-0000-1000-8000-00805f9b34fb") {
		return BluetoothController::FITNESS_MACHINE;
	}
	else {
		return BluetoothController::UNKNOWN;
	}
}

std::string BluetoothController::GetServiceUuid(ServiceType type)
{
	switch (type)
	{
	case BluetoothController::UNKNOWN:
		return "";
		break;
	case BluetoothController::HEART_RATE:
		return "0000180d-0000-1000-8000-00805f9b34fb";
		break;
	case BluetoothController::CYCLING_POWER:
		return "00001818-0000-1000-8000-00805f9b34fb";
		break;
	case BluetoothController::CYCLING_SPEED_CADENCE:
		return "00001816-0000-1000-8000-00805f9b34fb";
		break;
	case BluetoothController::FITNESS_MACHINE:
		return "00001826-0000-1000-8000-00805f9b34fb";
		break;
	default:
		return "";
		break;
	}
}

std::string BluetoothController::ToString(ServiceType type)
{
	switch (type)
	{
	case BluetoothController::UNKNOWN:
		return "Uknown";
		break;
	case BluetoothController::HEART_RATE:
		return "Heart Rate";
		break;
	case BluetoothController::CYCLING_POWER:
		return "Cycling Power";
		break;
	case BluetoothController::CYCLING_SPEED_CADENCE:
		return "Cycling Speend and Cadence";
		break;
	case BluetoothController::FITNESS_MACHINE:
		return "Fitness Machine";
		break;
	default:
		return "Uknown";
		break;
	}
}

int BluetoothController::SubscribeToGenericNotify(ServiceType type, std::function<void(SimpleBLE::ByteArray payload)> callback)
{
	SimpleBLE::BluetoothAddress targetDeviceAddress = serviceDeviceMap[type];

	//std::cout << "Heart Rate Device Address: " << targetDeviceAddress << " (" << targetDeviceAddress.length() << ")" << std::endl;
	if (targetDeviceAddress.length() == 0) {
		return EXIT_FAILURE;
	}

	// Step 1: Find device
	SimpleBLE::Peripheral device;
	bool foundDevice = false;

	connectedDevicesMtx.lock();
	for (int i = 0; i < connectedDevices.size(); i++) {
		if (connectedDevices.at(i).address() == targetDeviceAddress) {
			device = connectedDevices.at(i);
			foundDevice = true;
			break;
		}
	}
	connectedDevicesMtx.unlock();

	if (!foundDevice) {
		return EXIT_FAILURE;
	}

	// Step 2 Find Service
	std::vector<SimpleBLE::Service> deviceServices = device.services();

	SimpleBLE::Service service;
	bool foundService = false;

	for (int i = 0; i < deviceServices.size(); i++) {
		if (deviceServices.at(i).uuid() == GetServiceUuid(type)) {
			service = deviceServices.at(i);
			foundService = true;
			break;
		}
	}

	if (!foundService) {
		return EXIT_FAILURE;
	}

	// Step 3: Find characteristic
	std::vector<SimpleBLE::Characteristic> serviceCharacteristics = service.characteristics();

	SimpleBLE::Characteristic characteristic;
	bool foundCharacteristic = false;

	std::string characteristicUUID = "";
	switch (type)
	{
	case BluetoothController::UNKNOWN:
		break;
	case BluetoothController::HEART_RATE:
		characteristicUUID = heartRateMeasurementCharacteristic;
		break;
	case BluetoothController::CYCLING_POWER:
		characteristicUUID = cyclingPowerMeasurementCharacteristic;
		break;
	case BluetoothController::CYCLING_SPEED_CADENCE:
		characteristicUUID = cyclingCadenceMeasurementCharacteristic;
		break;
	case BluetoothController::FITNESS_MACHINE:
		break;
	default:
		break;
	}

	for (int i = 0; i < serviceCharacteristics.size(); i++) {
		if (serviceCharacteristics.at(i).uuid() == characteristicUUID) {
			characteristic = serviceCharacteristics.at(i);
			foundCharacteristic = true;
		}
	}

	if (!foundCharacteristic) {
		return EXIT_FAILURE;
	}

	// Step 4: Subscribe
	device.notify(service.uuid(), characteristic.uuid(), callback);


	return EXIT_SUCCESS;
}

int BluetoothController::Get8BitValue(char byte) 
{
	BitsOfByte_t byteOne;

	byteOne.byte = byte;

	int value = 0;

	value += 1 * byteOne._0;
	value += 2 * byteOne._1;
	value += 4 * byteOne._2;
	value += 8 * byteOne._3;
	value += 16 * byteOne._4;
	value += 32 * byteOne._5;
	value += 64 * byteOne._6;
	value += 128 * byteOne._7;

	return value;
}

int BluetoothController::Get16BitValue(char byte_1, char byte_2)
{
	BitsOfByte_t byteOne, byteTwo;

	byteOne.byte = byte_1;
	byteTwo.byte = byte_2;

	int value = 0;

	value += 1 * byteOne._0;
	value += 2 * byteOne._1;
	value += 4 * byteOne._2;
	value += 8 * byteOne._3;
	value += 16 * byteOne._4;
	value += 32 * byteOne._5;
	value += 64 * byteOne._6;
	value += 128 * byteOne._7;

	value += 256 * byteTwo._0;
	value += 512 * byteTwo._1;
	value += 1024 * byteTwo._2;
	value += 2048 * byteTwo._3;
	value += 4096 * byteTwo._4;
	value += 8192 * byteTwo._5;
	value += 16384 * byteTwo._6;
	value += 32768 * byteTwo._7;

	return value;
}

bool BluetoothController::IsBigEndian()
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };

	return bint.c[0] == 1;
}

int8_t BluetoothController::Convert8BitValueToLittleEndian(int8_t value)
{
	return value; // Do not need to swap byte order for 1 byte numbers
}

int16_t BluetoothController::Convert16BitValueToLittleEndian(int16_t value)
{
	bool notLittleEndian = IsBigEndian();

	if (notLittleEndian) {
		_2Bytes_t valueOrg, swappedValue;
		valueOrg._2byte = value;

		swappedValue.byte1 = valueOrg.byte2;
		swappedValue.byte2 = valueOrg.byte1;


		return swappedValue._2byte;
	}
	else {
		return value;
	}
}

void BluetoothController::HeartRateCallback(SimpleBLE::ByteArray bytes)
{
	BitsOfByte_t flags;
	flags.byte = bytes.at(0);

	bool oneByte = flags._0 == 0;

	int value = 0;

	if (!oneByte) { // Heart rate value is two bytes
		value = Get16BitValue(bytes.at(1), bytes.at(2));
	}
	else {
		value = Get8BitValue(bytes.at(1));
	}

	*heartRateValue = value;
}

void BluetoothController::CyclingPowerCallback(SimpleBLE::ByteArray bytes)
{
	int value = Get16BitValue(bytes.at(2), bytes.at(3));

	*cyclingPowerValue = value;
}

void BluetoothController::CadenceCallback(SimpleBLE::ByteArray bytes) 
{
	BitsOfByte_t flags;
	flags.byte = bytes.at(0);

	bool hasWheelData = flags._0 == 1;
	bool hasCrankData = flags._1 == 1;

	/*std::cout << "Cadence Data: ";
	for (auto b : bytes) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)((uint8_t)b) << " ";
	}*/

	int crankRevolutions = 0;
	int crankEventTime = 0;
	int cadenceValue = 0;

	if (hasWheelData && hasCrankData) {
		crankRevolutions = Get16BitValue(bytes.at(7), bytes.at(8));
		crankEventTime = Get16BitValue(bytes.at(9), bytes.at(10));
	}
	else if (!hasWheelData && hasCrankData) {
		crankRevolutions = Get16BitValue(bytes.at(1), bytes.at(2));
		crankEventTime = Get16BitValue(bytes.at(3), bytes.at(4));
	}
	
	int divisor = std::abs(lastCrankEventTime - crankEventTime);
	if (divisor != 0) {
		cadenceValue = (int)(((double)std::abs(lastCrankRevolutions - crankRevolutions) / (double)divisor) * 1024 * 60);
	}

	/*std::cout << " Has Wheel: " << hasWheelData;
	std::cout << " Has Crank: " << hasCrankData;
	std::cout << " Cadence: " << std::to_string(cadenceValue);
	std::cout << " Revs: " << std::to_string(crankRevolutions);
	std::cout << " Time: " << std::to_string(crankEventTime);
	std::cout << std::endl;*/

	*cyclingCadenceValue = (int)(cadenceValue); // Do averaging in workout class.

	lastCrankRevolutions = crankRevolutions;
	lastCrankEventTime = crankEventTime;
}
