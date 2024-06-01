#include "bluetooth-controller.hpp"

#include <iostream>
#include "raylib.h"
#include "bluetooth-utils.hpp"

bool BluetoothController::initialized = false;
SimpleBLE::Adapter BluetoothController::activeAdapter;

/// Devices found in scan.
std::vector<SimpleBLE::Peripheral> BluetoothController::foundDevices;

std::vector<SimpleBLE::Peripheral> BluetoothController::connectedDevices;


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
	TraceLog(LOG_INFO, "BLE: Connecting to device: %s, %s", device.identifier().c_str(), device.address().c_str());
	try {
		device.connect();
		TraceLog(LOG_INFO, "BLE: Sucessfully connected to device: %s, %s", device.identifier().c_str(), device.address().c_str());
		connectedDevices.push_back(device);
	}
	catch (...) {
		TraceLog(LOG_INFO, "BLE: Failed to connected to device: %s, %s", device.identifier().c_str(), device.address().c_str());
	}
}

void BluetoothController::DisconnectFromDevice(SimpleBLE::Peripheral device)
{
	TraceLog(LOG_INFO, "BLE: Disconnecting from device: %s, %s", device.identifier().c_str(), device.address().c_str());
	try {
		device.disconnect();
		TraceLog(LOG_INFO, "BLE: Sucessfully disconnected from device: %s, %s", device.identifier().c_str(), device.address().c_str());
		

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
		TraceLog(LOG_INFO, "BLE: Failed to disconnect from device: %s, %s", device.identifier().c_str(), device.address().c_str());
	}
}
