#include "video-settings.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "MattsUtils/string-func.hpp"
#include "MattsUtils/number.hpp"

#define VIDEO_SETTINGS_VERSION 1;

VideoSettings::MSAA_Level VideoSettings::currentMSAALevel = VideoSettings::_x1;
VideoSettings::MSAA_Level VideoSettings::actualMSAALevel;
VideoSettings::FPS_Limit VideoSettings::currentFPSLimit = VideoSettings::VSYNC;

int VideoSettings::ReadVideoSettingsConfigFile()
{
	TraceLog(LOG_INFO, "Reading VideoSettings from file.");

	// Read video settigns in. If there are any errors in the input file load the default video settings
	if (FileExists("VideoSettings.settings")) {

		std::ifstream file("VideoSettings.settings");

		// Read lines
		std::set<std::string> settingNames;
		std::string line;

		while (getline(file, line)) {

			line = MattsUtils::String::trim(line);
			std::vector<std::string> subStr;
			MattsUtils::String::split(line, subStr, ':');
			std::string key, value;

			if (subStr.size() < 2) {
				TraceLog(LOG_ERROR, "Invalid VideoSetting Kay Value set.");
				LoadDefaultVideoSettings();
				return EXIT_FAILURE;
			}

			key = MattsUtils::String::trim(subStr.at(0));
			value = MattsUtils::String::trim(subStr.at(1));
			
			std::pair<std::set<std::string>::iterator, bool> res = settingNames.insert(key);
			
			if (!res.second) {
				TraceLog(LOG_ERROR, "Invalid VideoSetting Kay Value.");
				LoadDefaultVideoSettings();
				return EXIT_FAILURE;
			}
			else {
				if (key == "FPS_Limit") {
					currentFPSLimit = (FPS_Limit)MattsUtils::Number::parseInt(value);
				} else if (key == "MSAA_Level") {
					currentMSAALevel = (MSAA_Level)MattsUtils::Number::parseInt(value);
					actualMSAALevel = (MSAA_Level)MattsUtils::Number::parseInt(value);
				}
			}

		}

		file.close();
		return EXIT_SUCCESS;
	}
	else {
		LoadDefaultVideoSettings();
		return EXIT_FAILURE;
	}
}

void VideoSettings::WriteVideoSettingsConfigFile()
{
	TraceLog(LOG_INFO, "Writing VideoSettings to file.");

	std::ofstream file("VideoSettings.settings");

	int version = VIDEO_SETTINGS_VERSION;
	file << "MSAA_Level: " << currentMSAALevel << std::endl;
	file << "FPS_Limit: " << currentFPSLimit << std::endl;

	file.close();

}

void VideoSettings::LoadDefaultVideoSettings()
{
	// The default video settings
	currentMSAALevel = VideoSettings::_x1;
	currentFPSLimit = VideoSettings::VSYNC;
}

void VideoSettings::PreWindowCreationFlags()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	actualMSAALevel = currentMSAALevel;
	if (currentMSAALevel == _x4) {
		SetConfigFlags(FLAG_MSAA_4X_HINT);
	}
}

void VideoSettings::PostWindowCreatedFlags()
{
	ChangeFPSLimit();
}

void VideoSettings::UpdateMSAALevel(MSAA_Level newMSAALevel)
{
	currentMSAALevel = newMSAALevel;
}

VideoSettings::MSAA_Level VideoSettings::GetCurrentMSAALevel()
{
	return currentMSAALevel;
}

VideoSettings::MSAA_Level VideoSettings::GetActualMSAALevel()
{
	return actualMSAALevel;
}

std::string VideoSettings::GetMSAALevelDropdownText()
{
	return "x1;x4";
}

void VideoSettings::UpdateFPSLimit(FPS_Limit newLimit)
{
	currentFPSLimit = newLimit;
	ChangeFPSLimit();
}

VideoSettings::FPS_Limit VideoSettings::GetCurrentFPSLimit()
{
	return currentFPSLimit;
}

std::string VideoSettings::GetFPSDropdownText()
{
	return "None;VSYNC;30;60;90;120;144;240;360;480";
}

void VideoSettings::ChangeFPSLimit()
{
	int fpsTarget[] = { 0, -1, 30, 60, 90, 120, 144, 240, 360, 480 };
	int sel = fpsTarget[(int)currentFPSLimit];
	TraceLog(LOG_INFO, "Set FPS LIMIT: %d", sel);

	if (sel == -1) {
		if (!IsWindowState(FLAG_VSYNC_HINT)) {
			SetWindowState(FLAG_VSYNC_HINT);
		}
	}
	else {
		if (IsWindowState(FLAG_VSYNC_HINT)) {
			ClearWindowState(FLAG_VSYNC_HINT);
		}
		SetTargetFPS(sel);
	}
}
