#pragma once
#include "raylib.h"

#define USER_SETTINGS_VERSION 1;

class UserSettings {
public:

	static int ReadSettingsConfigFile();
	static void WriteSettingsConfigFile();

	static void LoadDefaultSettings();

	static int GetFTP();
	static void SetFTP(int value);
	
	static int GetPowerAveragePeriod();
	
	static int GetCadenceAveragePeriod();

private:
	static int ftp;
	static int powerAveragePeriod;
	static int cadenceAveragePeriod;
};