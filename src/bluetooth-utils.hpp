#pragma once
#include <simpleble/SimpleBLE.h>
#include <string>

namespace BleUtils {

	/// <summary>
	/// Device types that this application is interested in.
	/// </summary>
	enum ServiceType {
		UNKNOWN,
		HEART_RATE,
		CYCLING_POWER,
		Cycling_Speed_Cadence,
		Fitness_Machine
	};

	ServiceType GetServiceType(SimpleBLE::BluetoothUUID uuid);

	std::string ToString(ServiceType type);

};