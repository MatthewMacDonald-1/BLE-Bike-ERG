#include "user-data.hpp"

int UserSettings::ftp = 0;
int UserSettings::powerAveragePeriod = 3; // Default
int UserSettings::cadenceAveragePeriod = 3; // Default

void UserSettings::LoadUserSettings()
{
	ftp = 243;
	powerAveragePeriod = 3;
	cadenceAveragePeriod = 3;
}

int UserSettings::GetFTP()
{
	return ftp;
}

int UserSettings::GetPowerAveragePeriod()
{
	return powerAveragePeriod;
}

int UserSettings::GetCadenceAveragePeriod()
{
	return cadenceAveragePeriod;
}
