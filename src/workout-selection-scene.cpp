#include "workout-selection-scene.hpp"
#include "config.hpp"

#include "raygui.h"
#include "raymath.h"
#include <string>
#include <iostream>

#include "font-settings.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "settings-menu.hpp"
#include "scene-manager.hpp"

using namespace MattsUtils;

WorkoutSelectionMenuScene::WorkoutSelectionMenuScene()
{
	menuBackground = LoadTexture("./resources/images/test-background.png");
}

WorkoutSelectionMenuScene::~WorkoutSelectionMenuScene()
{
	UnloadTexture(menuBackground);
}

int WorkoutSelectionMenuScene::UpdateLogic()
{
	return EXIT_SUCCESS;
}

int WorkoutSelectionMenuScene::DrawCall()
{
	Font fontType = FontSettings::GetMainFont();

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	DrawTextureEx(menuBackground, raylib::ConstructVector2(0, 0), 0, (float)GetScreenWidth() / (float)menuBackground.width, WHITE);

	if (menuOpen) {
		int res = SettingsMenu::DrawSettingsMenu();
		if (res == SIGNAL_SETTINGS_MENU_CLOSE) {
			menuOpen = false;
			return EXIT_SUCCESS;
		}
		else {
			return res;
		}
	}

	RelativeDrawing::DrawTextRelEx(fontType, "Workout Selection", raylib::ConstructVector2(0, 16), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 64, 1.5, BLACK);

	// Buttons Start ------------------------------------------------------------------------------
	GuiSetFont(fontType);

	bool nextSceneRes = RelativeDrawing::GuiButtonRelative("Start", offsetDstBR, buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);
	if (nextSceneRes) {
		//SceneManager::LoadScene("WorkoutSelectionMenu");
	}

	bool backToStartMenuSceneRes = RelativeDrawing::GuiButtonRelative("Back", Vector2Add(offsetDstBR, raylib::ConstructVector2(-(buttonSize.x + 32), 0)), buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);
	if (backToStartMenuSceneRes) {
		SceneManager::LoadScene("MainMenu");
	}


	bool settingRes = RelativeDrawing::GuiButtonRelative("Settings", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);
	if (settingRes) {
		menuOpen = true;
	}
	// Buttons End --------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}
