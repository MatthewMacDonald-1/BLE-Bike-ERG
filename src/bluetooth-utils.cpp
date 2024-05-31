#include "bluetooth-utils.hpp"


BleUtils::ServiceType BleUtils::GetServiceType(SimpleBLE::BluetoothUUID uuid)
{
	if (uuid == "0000180d-0000-1000-8000-00805f9b34fb") {
		return BleUtils::HEART_RATE;
	}
	else if (uuid == "00001818-0000-1000-8000-00805f9b34fb") {
		return BleUtils::CYCLING_POWER;
	}
	else if (uuid == "00001816-0000-1000-8000-00805f9b34fb") {
		return BleUtils::Cycling_Speed_Cadence;
	}
	else if (uuid == "00001826-0000-1000-8000-00805f9b34fb") {
		return BleUtils::Fitness_Machine;
	}
	else {
		return BleUtils::UNKNOWN;
	}
}

std::string BleUtils::ToString(ServiceType type)
{
	switch (type)
	{
	case BleUtils::UNKNOWN:
		return "Uknown";
		break;
	case BleUtils::HEART_RATE:
		return "Heart Rate";
		break;
	case BleUtils::CYCLING_POWER:
		return "Cycling Power";
		break;
	case BleUtils::Cycling_Speed_Cadence:
		return "Cycling Speend and Cadence";
		break;
	case BleUtils::Fitness_Machine:
		return "Fitness Machine";
		break;
	default:
		return "Uknown";
		break;
	}
}

