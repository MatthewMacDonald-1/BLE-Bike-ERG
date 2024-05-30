#include "bluetooth-controller.hpp"

#include <iostream>
#include "raylib.h"

bool BluetoothController::initialized = false;
SimpleBLE::Adapter BluetoothController::activeAdapter;

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
		TraceLog(LOG_INFO, "BLE: Scan started");
	});

	// Set the callback to be called when the scan stops
	activeAdapter.set_callback_on_scan_stop([]() {
		TraceLog(LOG_INFO, "BLE: Scan stopped");
	});

	// Set the callback to be called when the scan finds a new peripheral
	activeAdapter.set_callback_on_scan_found([](SimpleBLE::Peripheral peripheral) {
		TraceLog(LOG_INFO, "BLE: Peripheral found: %s, %s", peripheral.identifier().c_str(), peripheral.address().c_str());
	});

	// Set the callback to be called when a peripheral property has changed
	activeAdapter.set_callback_on_scan_updated([](SimpleBLE::Peripheral peripheral) {
		TraceLog(LOG_INFO, "BLE: Peripheral updated: %s, %s", peripheral.identifier().c_str(), peripheral.address().c_str());
	});


	initialized = true;
	return EXIT_SUCCESS;
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
