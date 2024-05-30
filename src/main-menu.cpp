#include "main-menu.hpp"

#include "config.hpp"

#include "raygui.h"

#include "bluetooth-controller.hpp"
#include "font-settings.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"

using namespace MattsUtils;

MainMenuScene::MainMenuScene()
{
    menuBackground = LoadTexture("./resources/images/test-background.png"); // Important Note: the file path is relative to the executable
	BluetoothController::StartScan();
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

	// Scroll Area Start --------------------------------------------------------------------------

	Vector2 topCorner = MattsUtils::raylib::ConstructVector2(120, 148);
	Vector2 panelDimensions = MattsUtils::raylib::ConstructVector2(GetScreenWidth() - 240, GetScreenHeight() - 240);
	panelRec = MattsUtils::raylib::ConstructRectangle(topCorner.x, topCorner.y, panelDimensions.x, panelDimensions.y);

	GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll, &panelView);

	BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
	GuiGrid(MattsUtils::raylib::ConstructRectangle(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y, panelContentRec.width, panelContentRec.height), NULL, 16, 3, NULL);
	EndScissorMode();

	if (showContentArea) DrawRectangle(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y, panelContentRec.width, panelContentRec.height, Fade(RED, 0.1));

	// Scroll Area End ----------------------------------------------------------------------------


	// Buttons Start ------------------------------------------------------------------------------
	GuiSetFont(fontType);

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	bool skipRes = RelativeDrawing::GuiButtonRelative("Next", offsetDstBR, buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);

	bool quitRes = RelativeDrawing::GuiButtonRelative("Quit", offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (quitRes) {
		BluetoothController::StopScan();
		return SIGNAL_WINDOW_CLOSE;
	}

	bool settingRes = RelativeDrawing::GuiButtonRelative("Settings", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);

	// Buttons End --------------------------------------------------------------------------------


	return EXIT_SUCCESS;
}
