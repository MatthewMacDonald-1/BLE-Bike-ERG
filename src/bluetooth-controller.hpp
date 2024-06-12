#pragma once
#include <simpleble/SimpleBLE.h>
#include <string>
#include <atomic>
#include <mutex>
#include <unordered_map>

class BluetoothController {
public:
	/// <summary>
	/// Device types that this application is interested in.
	/// </summary>
	enum ServiceType {
		UNKNOWN,
		HEART_RATE,
		CYCLING_POWER,
		CYCLING_SPEED_CADENCE,
		FITNESS_MACHINE
	};

	static bool BluetoothSupported();

	static int InitializeBluetooth();

	static void DeinitializeBluetooth();

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

	/// <summary>
	/// Not Thread Safe.
	/// </summary>
	/// <returns></returns>
	static std::vector<SimpleBLE::Peripheral> GetConnectedDevices();

	/// <summary>
	/// Thread Safe.
	/// </summary>
	/// <param name="device"></param>
	static void ConnectToDevice(SimpleBLE::Peripheral device);

	/// <summary>
	/// Thread Safe.
	/// </summary>
	/// <returns></returns>
	static int GetActiveConnectionThreads();

	/// <summary>
	/// Thread Safe.
	/// </summary>
	/// <param name="device"></param>
	static void DisconnectFromDevice(SimpleBLE::Peripheral device);

	/// <summary>
	/// Thread Safe.
	/// </summary>
	/// <returns></returns>
	static int GetActiveDisconnectionThreads();

	static void SetServiceDeviceMap(ServiceType type, SimpleBLE::BluetoothAddress address);

	static int SubscribeToHeartRate(int* heartRateReference);

	static ServiceType GetServiceType(SimpleBLE::BluetoothUUID uuid);

	static std::string GetServiceUuid(ServiceType type);

	static std::string ToString(ServiceType type);

private:
	static bool initialized;
	static SimpleBLE::Adapter activeAdapter;

	static std::atomic<int> activeConnectionThreads;
	static std::atomic<int> activeDisconnectionThreads;
	static std::mutex connectedDevicesMtx;

	static std::unordered_map<ServiceType, SimpleBLE::BluetoothAddress> serviceDeviceMap;

	static std::string heartRateMeasurementCharacteristic;


	/// Devices found in scan.
	static std::vector<SimpleBLE::Peripheral> foundDevices;
	static std::vector<SimpleBLE::Peripheral> connectedDevices;

	static int* heartRateValue;

	static void HeartRateCallback(SimpleBLE::ByteArray bytes);

};