#include "user-data.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "MattsUtils/string-func.hpp"
#include "MattsUtils/number.hpp"

int UserSettings::ftp = 0;
int UserSettings::powerAveragePeriod = 3; // Default
int UserSettings::cadenceAveragePeriod = 3; // Default

int UserSettings::ReadSettingsConfigFile()
{
	TraceLog(LOG_INFO, "Reading UserSettings from file.");

	// Read video settigns in. If there are any errors in the input file load the default video settings
	if (FileExists("UserSettings.settings")) {

		std::ifstream file("UserSettings.settings");

		// Read lines
		std::set<std::string> settingNames;
		std::string line;

		while (getline(file, line)) {

			line = MattsUtils::String::trim(line);
			std::vector<std::string> subStr;
			MattsUtils::String::split(line, subStr, ':');
			std::string key, value;

			if (subStr.size() < 2) {
				TraceLog(LOG_ERROR, "Invalid UserSettings Kay Value set.");
				LoadDefaultSettings();
				return EXIT_FAILURE;
			}

			key = MattsUtils::String::trim(subStr.at(0));
			value = MattsUtils::String::trim(subStr.at(1));

			std::pair<std::set<std::string>::iterator, bool> res = settingNames.insert(key);

			if (!res.second) {
				TraceLog(LOG_ERROR, "Invalid UserSettings Kay Value.");
				LoadDefaultSettings();
				return EXIT_FAILURE;
			}
			else {
				if (key == "FTP") {
					ftp = MattsUtils::Number::parseInt(value);
				}
				else if (key == "POWER_AVERAGE_PERIOD") {
					powerAveragePeriod = MattsUtils::Number::parseInt(value);
				}
				else if (key == "CADENCE_AVERAGE_PERIOD") {
					cadenceAveragePeriod = MattsUtils::Number::parseInt(value);
				}
			}

		}

		file.close();
		return EXIT_SUCCESS;
	}
	else {
		LoadDefaultSettings();
		return EXIT_FAILURE;
	}
}

void UserSettings::WriteSettingsConfigFile()
{
	TraceLog(LOG_INFO, "Writing UserSettings to file.");

	std::ofstream file("UserSettings.settings");

	int version = USER_SETTINGS_VERSION;
	file << "FTP: " << ftp << std::endl;
	file << "POWER_AVERAGE_PERIOD: " << powerAveragePeriod << std::endl;
	file << "CADENCE_AVERAGE_PERIOD: " << cadenceAveragePeriod << std::endl;

	file.close();
}

void UserSettings::LoadDefaultSettings()
{
	ftp = 200;
	powerAveragePeriod = 3;
	cadenceAveragePeriod = 1;
}

int UserSettings::GetFTP()
{
	return ftp;
}

void UserSettings::SetFTP(int value)
{
	ftp = value;
}

int UserSettings::GetPowerAveragePeriod()
{
	return powerAveragePeriod;
}

int UserSettings::GetCadenceAveragePeriod()
{
	return cadenceAveragePeriod;
}
