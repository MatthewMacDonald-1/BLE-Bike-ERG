#include "settings-menu.hpp"
#include <iostream>
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "raygui.h"
#include "font-settings.hpp"
#include "video-settings.hpp"
#include "config.hpp"

bool SettingsMenu::initialized = false;
Texture SettingsMenu::settingsBackground;

int SettingsMenu::activeFPS = 0;
bool SettingsMenu::editingActiveFPS = false;

int SettingsMenu::activeMSAALevel = 0;
bool SettingsMenu::editingActiveMSAALevel = false;

int SettingsMenu::InitializeSettingsMenu()
{
	if (initialized) {
		return EXIT_SUCCESS;
	}

	settingsBackground = LoadTexture("./resources/images/test-background.png");

	initialized = true;
	return EXIT_SUCCESS;
}

void SettingsMenu::DeinitializeSettingsMenu()
{
	if (!initialized) {
		return;
	}
	
	UnloadTexture(settingsBackground);
	
	initialized = false;
}

int SettingsMenu::DrawSettingsMenu()
{
	using namespace MattsUtils;

	Font fontType = FontSettings::GetMainFont();
	GuiSetFont(fontType);

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 dropdownSize = raylib::ConstructVector2(256, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	int offsetIndex = 0;
	int offsetAmount = 32;
	int menuItemsStart = 128;

	DrawTextureEx(settingsBackground, raylib::ConstructVector2(0, 0), 0, (float)GetScreenWidth() / (float)settingsBackground.width, WHITE);

	RelativeDrawing::DrawTextRelEx(fontType, "Settings", raylib::ConstructVector2(0, 16), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 64, 1.5, BLACK);

	// Exit buttons

	bool quitRes = RelativeDrawing::GuiButtonRelative("Quit", offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);

	bool settingCloseRes = RelativeDrawing::GuiButtonRelative("Back", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);

	// Settings items
	if (editingActiveFPS) GuiUnlock();
	RelativeDrawing::DrawTextRelEx(fontType, "FPS Limit", raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 24, 1.5, BLACK); offsetIndex++;

	activeFPS = (int)VideoSettings::GetCurrentFPSLimit();
	bool res = RelativeDrawing::GuiDropdownBoxRelative((char*)VideoSettings::GetFPSDropdownText().c_str(), &activeFPS, editingActiveFPS, raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex), dropdownSize, RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 24); offsetIndex++;
	if (res != 0) {
		if (editingActiveFPS) {
			// Change the fps state

			VideoSettings::FPS_Limit newLimit = (VideoSettings::FPS_Limit)activeFPS;
			VideoSettings::UpdateFPSLimit(newLimit);

			VideoSettings::WriteVideoSettingsConfigFile();
		}
		editingActiveFPS = !editingActiveFPS;
	}

	if (editingActiveFPS) GuiLock();

	if (editingActiveMSAALevel) GuiUnlock();

	RelativeDrawing::DrawTextRelEx(fontType, "MSAA Level", raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 24, 1.5, BLACK); offsetIndex++;

	activeMSAALevel = (int)VideoSettings::GetCurrentMSAALevel();
	bool resMSAALevel = RelativeDrawing::GuiDropdownBoxRelative((char*)VideoSettings::GetMSAALevelDropdownText().c_str(), &activeMSAALevel, editingActiveMSAALevel, raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex), dropdownSize, RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 24); offsetIndex++;
	if (resMSAALevel != 0) {
		if (editingActiveMSAALevel) {
			// Change the fps state

			VideoSettings::MSAA_Level newLevel = (VideoSettings::MSAA_Level)activeMSAALevel;
			VideoSettings::UpdateMSAALevel(newLevel);

			VideoSettings::WriteVideoSettingsConfigFile();
		}
		editingActiveMSAALevel = !editingActiveMSAALevel;
	}
	

	if (editingActiveFPS || editingActiveMSAALevel) GuiLock();
	
	// Exit buttons responses
	if (quitRes) {
		return SIGNAL_WINDOW_CLOSE;
	}

	if (settingCloseRes) {
		return SIGNAL_SETTINGS_MENU_CLOSE;
	}
	return EXIT_SUCCESS;
}
