#pragma once
#include <string>
#include <vector>
#include <ctime>

namespace FIT_FILE_ENCODER {

	enum DataTypes {
		TIME_EPOCH, // std::time_t (long long)
		POWER, // int
		CADENCE, // int
		HEART_RATE // int
	};

	int EncodeWorkout(std::string filename, std::vector<std::time_t> timeStamps, std::vector<int> powerValues, std::vector<int> cadenceValues, std::vector<int> heartRateValues);
};