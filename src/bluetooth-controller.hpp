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

private:
	static bool initialized;
	static SimpleBLE::Adapter activeAdapter;

};