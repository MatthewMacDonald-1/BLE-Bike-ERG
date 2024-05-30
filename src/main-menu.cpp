#include "main-menu.hpp"

#include "config.hpp"

#include "raygui.h"

#include "font-settings.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"

using namespace MattsUtils;

MainMenuScene::MainMenuScene()
{
    menuBackground = LoadTexture("./resources/images/test-background.png"); // Important Note: the file path is relative to the executable
}

MainMenuScene::~MainMenuScene()
{
	UnloadTexture(menuBackground);
}

int MainMenuScene::UpdateLogic()
{
	return EXIT_SUCCESS;
}

int MainMenuScene::DrawCall()
{
	Font fontType = FontSettings::GetMainFont();

	DrawTextureEx(menuBackground, raylib::ConstructVector2(0, 0), 0, (float)GetScreenWidth() / (float)menuBackground.width, WHITE);

	RelativeDrawing::DrawTextRelEx(fontType, "Bike ERG", raylib::ConstructVector2(0, 0), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 128, 1.5, BLACK);

	// Buttons Start ------------------------------------------------------------------------------
	GuiSetFont(fontType);

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	bool skipRes = RelativeDrawing::GuiButtonRelative("Skip", offsetDstBR, buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);

	bool quitRes = RelativeDrawing::GuiButtonRelative("Quit", offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (quitRes) {
		return SIGNAL_WINDOW_CLOSE;
	}

	bool settingRes = RelativeDrawing::GuiButtonRelative("Settings", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);

	// Buttons End --------------------------------------------------------------------------------


	return EXIT_SUCCESS;
}
