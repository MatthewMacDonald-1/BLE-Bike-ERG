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

/// Devices found in scan.
std::vector<SimpleBLE::Peripheral> BluetoothController::foundDevices;

std::vector<SimpleBLE::Peripheral> BluetoothController::connectedDevices;

int* BluetoothController::heartRateValue = NULL;
int* BluetoothController::cyclingPowerValue = NULL;
int* BluetoothController::cyclingCadenceValue = NULL;

int BluetoothController::lastCrankRevolutions = 0;
int BluetoothController::lastCrankEventTime = 0;

int BluetoothController::cyclingCadenceAvgIdx = 0;
int BluetoothController::cyclingCadenceAvg[3] = { 0 };


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

	cyclingCadenceAvg[0] = 0;
	cyclingCadenceAvg[1] = 0;
	cyclingCadenceAvg[2] = 0;

	return SubscribeToGenericNotify(CYCLING_SPEED_CADENCE, CadenceCallback);
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

typedef union BitsOfByte_t {
	uint8_t byte;
	struct {
		uint8_t _0 : 1, _1 : 1, _2 : 1, _3 : 1, _4 : 1, _5 : 1, _6 : 1, _7 : 1; // Little endian
	};

} BitsOfByte_t;

typedef union _2Bytes_t {
	uint16_t _2byte;
	struct {
		uint8_t byte1, byte2;
	};

} _2Bytes_t;

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

	std::cout << "Cadence Data: ";
	for (auto b : bytes) {
		std::cout << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)((uint8_t)b) << " ";
	}

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

	std::cout << " Has Wheel: " << hasWheelData;
	std::cout << " Has Crank: " << hasCrankData;
	std::cout << " Cadence: " << std::to_string(cadenceValue);
	std::cout << " Revs: " << std::to_string(crankRevolutions);
	std::cout << " Time: " << std::to_string(crankEventTime);
	std::cout << std::endl;

	// Calculate 3 second avg
	cyclingCadenceAvg[cyclingCadenceAvgIdx] = cadenceValue;
	cyclingCadenceAvgIdx++;
	if (cyclingCadenceAvgIdx > 2) {
		cyclingCadenceAvgIdx = 0;
	}

	double avgCadence = (cyclingCadenceAvg[0] + cyclingCadenceAvg[1] + cyclingCadenceAvg[2]) / 3.0;

	*cyclingCadenceValue = (int)(avgCadence);

	lastCrankRevolutions = crankRevolutions;
	lastCrankEventTime = crankEventTime;
}
