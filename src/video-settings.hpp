#pragma once

#include "raylib.h"

class VideoSettings {
public:
	enum MSAA_Level {
		_x1,
		_x4
	};
	enum FPS_Limit {
		NONE,
		VSYNC,
		_30,
		_60,
		_90,
		_120,
		_144,
		_240,
		_360,
		_480
	};

	static int ReadVideoSettingsConfigFile();
	static void WriteVideoSettingsConfigFile();

	static void LoadDefaultVideoSettings();

	static void PreWindowCreationFlags();
	static void PostWindowCreatedFlags();

	/// <summary>
	/// Dosen't Actual upadate until next game launch
	/// </summary>
	static void UpdateMSAALevel(MSAA_Level newMSAALevel);
	static MSAA_Level GetCurrentMSAALevel();
	/// <summary>
	/// Gets the current;y in use MSAA Level.
	/// </summary>
	/// <returns></returns>
	static MSAA_Level GetActualMSAALevel();

	static void UpdateFPSLimit(FPS_Limit newLimit);
	static FPS_Limit GetCurrentFPSLimit();

private:
	static MSAA_Level currentMSAALevel;
	static MSAA_Level actualMSAALevel;

	static FPS_Limit currentFPSLimit;

	static void ChangeFPSLimit();

};