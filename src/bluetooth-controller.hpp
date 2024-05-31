#pragma once
#include <simpleble/SimpleBLE.h>

class BluetoothController {
public:
	static bool BluetoothSupported();

	static int InitializeBluetooth();

	/// <summary>
	/// Scan for bluetooth devices
	/// </summary>
	static void StartScan();

	/// <summary>
	/// Stop scanning for bluetooth devices
	/// </summary>
	static void StopScan();

	/// <summary>
	/// Return list of devices found in scan.
	/// </summary>
	static std::vector<SimpleBLE::Peripheral> GetDiscoveredDevices();

	static std::vector<SimpleBLE::Peripheral> GetConnectedDevices();

	static void ConnectToDevice(SimpleBLE::Peripheral device);

private:
	static bool initialized;
	static SimpleBLE::Adapter activeAdapter;

	/// Devices found in scan.
	static std::vector<SimpleBLE::Peripheral> foundDevices;
	static std::vector<SimpleBLE::Peripheral> connectedDevices;

};