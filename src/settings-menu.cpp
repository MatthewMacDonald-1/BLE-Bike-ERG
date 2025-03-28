#include "settings-menu.hpp"
#include <iostream>
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "MattsUtils/number.hpp"
#include "raygui.h"
#include "font-settings.hpp"
#include "video-settings.hpp"
#include "user-data.hpp"
#include "config.hpp"
#include <cstring>
#include <math.h>

bool SettingsMenu::initialized = false;
Texture SettingsMenu::settingsBackground;

int SettingsMenu::activeFPS = 0;
bool SettingsMenu::editingActiveFPS = false;

int SettingsMenu::activeMSAALevel = 0;
bool SettingsMenu::editingActiveMSAALevel = false;

bool SettingsMenu::ftpHover = false;
MattsUtils::Bounds SettingsMenu::ftpInputBounds(0, 0, 0, 0);
char SettingsMenu::ftpInputBuffer[10];
int SettingsMenu::ftpInputLetterCount = 0;
#define FTP_MAX_INPUT_CHARS 9


int SettingsMenu::InitializeSettingsMenu()
{
	if (initialized) {
		return EXIT_SUCCESS;
	}

	settingsBackground = LoadTexture("./resources/images/test-background.png");

	memset(ftpInputBuffer, '\0', 10);
	const char* ftpCurrentChar = TextFormat("%d", UserSettings::GetFTP());
	int ftpLength = strlen(ftpCurrentChar);
	for (int i = 0; i < ftpLength; i++) {
		ftpInputBuffer[i] = ftpCurrentChar[i];
		ftpInputLetterCount++;
	}

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

	RelativeDrawing::DrawTextRelEx(fontType, "FTP", raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 24, 1.5, BLACK); offsetIndex++;
	
	ftpHover = ftpInputBounds.InBounds(GetMouseX(), GetMouseY());

	Color borderColor = DARKGRAY;
	const char* inputText;
	if (ftpHover) {
		borderColor = BLUE;
		SetMouseCursor(MOUSE_CURSOR_IBEAM);

		// Get char pressed (unicode character) on the queue
		int key = GetCharPressed();

		// Check if more characters have been pressed on the same frame
		while (key > 0)
		{
			// NOTE: Only allow keys in range [32..125]
			if ((key >= 48) && (key <= 57) && (ftpInputLetterCount < FTP_MAX_INPUT_CHARS))
			{
				ftpInputBuffer[ftpInputLetterCount] = (char)key;
				ftpInputBuffer[ftpInputLetterCount + 1] = '\0'; // Add null terminator at the end of the string.
				ftpInputLetterCount++;
			}

			key = GetCharPressed();  // Check next character in the queue
		}

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			ftpInputLetterCount--;
			if (ftpInputLetterCount < 0) ftpInputLetterCount = 0;
			ftpInputBuffer[ftpInputLetterCount] = '\0';
		}

		// Coveret to int and limit size
		std::string input(ftpInputBuffer);
		int inputInt = 0;
		if (ftpInputLetterCount > 0) inputInt = MattsUtils::Number::parseInt(input);
		inputInt = std::min(1000, std::max(0, inputInt));
		UserSettings::SetFTP(inputInt);

		char blinker = ' ';
		if (std::abs(std::sin(GetTime() * 3.0))  > 0.5f) {
			blinker = '_';
		}
		inputText = TextFormat("%d%c", inputInt, blinker);
	}
	else {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		inputText = TextFormat("%d", UserSettings::GetFTP());
	}

	
	


	ftpInputBounds = RelativeDrawing::DrawRectangle(raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex), dropdownSize, RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, borderColor);
	RelativeDrawing::DrawRectangle(raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex + 1), raylib::ConstructVector2(dropdownSize.x - 2, dropdownSize.y - 2), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, LIGHTGRAY);
	RelativeDrawing::DrawTextRelEx(
		fontType,
		inputText,
		raylib::ConstructVector2(0, menuItemsStart + offsetAmount * offsetIndex + 4),
		RelativeDrawing::TopCenter, RelativeDrawing::TopCenter,
		24,
		1.5,
		BLACK
	);
	offsetIndex++;

	
	// Exit buttons responses
	if (quitRes || settingCloseRes) {
		UserSettings::WriteSettingsConfigFile();
	}
	
	if (quitRes) {
		return SIGNAL_WINDOW_CLOSE;
	}

	if (settingCloseRes) {
		return SIGNAL_SETTINGS_MENU_CLOSE;
	}
	return EXIT_SUCCESS;
}
