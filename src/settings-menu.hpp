#pragma once
#include "raylib.h"
#include "MattsUtils/bounds.hpp"
#define CLOSE_MENU


class SettingsMenu
{
public:
	static int InitializeSettingsMenu();
	static void DeinitializeSettingsMenu();

	static int DrawSettingsMenu();

private:
	static bool initialized;
	static Texture settingsBackground;

	static int activeFPS;
	static bool editingActiveFPS;

	static int activeMSAALevel;
	static bool editingActiveMSAALevel;

	static bool ftpHover;
	static MattsUtils::Bounds ftpInputBounds;
	static char ftpInputBuffer[10];
	static int ftpInputLetterCount;
};

