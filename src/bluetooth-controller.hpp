#pragma once
#include <simpleble/SimpleBLE.h>
#include <string>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <functional>

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

	static int SubscribeToCyclingPower(int* cyclingPowerReference);

	static int SubscribeToCadence(int* cadenceReference);

	/// <summary>
	/// Request control over the trainer using the FMCP.
	/// </summary>
	/// <param name="complete"></param>
	/// <param name="response"></param>
	static void RequestTrainerControl(bool* complete, int* response);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="targetPower"></param>
	/// <param name="complete">Process is completed</param>
	/// <param name="response">If the desired target was successfully communicated.</param>
	static void SetTrainerTargetPower(int targetPower, bool* complete, int* response);

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
	static std::string cyclingPowerMeasurementCharacteristic;
	static std::string cyclingCadenceMeasurementCharacteristic;

	static std::string fitnessMachine_FMF; // Fitness Machine Feature
	static std::string fitnessMachine_IBD; // Indoor Bike Data
	static std::string fitnessMachine_TS; // Training Status
	static std::string fitnessMachine_SRLR; // Supported Resistance Level Range
	static std::string fitnessMachine_SPR; // Supported Power Range
	static std::string fitnessMachine_FMCP; // Fitness Machine Control Point
	static std::string fitnessMachine_FMS; // Fitness Machine Status


	/// Devices found in scan.
	static std::vector<SimpleBLE::Peripheral> foundDevices;
	static std::vector<SimpleBLE::Peripheral> connectedDevices;

	static int* heartRateValue;
	static int* cyclingPowerValue;
	static int* cyclingCadenceValue;

	static int lastCrankRevolutions;
	static int lastCrankEventTime;

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

	static int SubscribeToGenericNotify(ServiceType type, std::function<void(SimpleBLE::ByteArray payload)> callback);

	static int Get8BitValue(char byte);
	static int Get16BitValue(char byte_1, char byte_2);

	static bool IsBigEndian();
	static int8_t Convert8BitValueToLittleEndian(int8_t value);
	static int16_t Convert16BitValueToLittleEndian(int16_t value);

	static void HeartRateCallback(SimpleBLE::ByteArray bytes);
	static void CyclingPowerCallback(SimpleBLE::ByteArray bytes);
	static void CadenceCallback(SimpleBLE::ByteArray bytes);


};