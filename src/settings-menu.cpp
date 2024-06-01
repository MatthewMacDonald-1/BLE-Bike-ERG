#include "settings-menu.hpp"
#include <iostream>
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "raygui.h"
#include "font-settings.hpp"
#include "config.hpp"

bool SettingsMenu::initialized = false;
Texture SettingsMenu::settingsBackground;

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
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	DrawTextureEx(settingsBackground, raylib::ConstructVector2(0, 0), 0, (float)GetScreenWidth() / (float)settingsBackground.width, WHITE);

	RelativeDrawing::DrawTextRelEx(fontType, "Settings", raylib::ConstructVector2(0, 16), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 64, 1.5, BLACK);

	bool quitRes = RelativeDrawing::GuiButtonRelative("Quit", offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (quitRes) {
		return SIGNAL_WINDOW_CLOSE;
	}

	bool settingCloseRes = RelativeDrawing::GuiButtonRelative("Back", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);
	if (settingCloseRes) {
		return SIGNAL_SETTINGS_MENU_CLOSE;
	}
	return EXIT_SUCCESS;
}
