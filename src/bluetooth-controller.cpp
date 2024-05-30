#include "bluetooth-controller.hpp"

#include <iostream>

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

	// ToDo: Set callbacks for scanning. Start, Stop, on found.

	initialized = true;
	return EXIT_SUCCESS;
}

void BluetoothController::StartScan()
{

}

void BluetoothController::StopScan()
{

}
