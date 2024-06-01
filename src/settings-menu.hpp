#pragma once
#include "raylib.h"
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
};

