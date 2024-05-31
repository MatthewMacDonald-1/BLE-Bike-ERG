#include "main-menu.hpp"

#include "config.hpp"

#include "raygui.h"
#include "raymath.h"
#include <string>
#include <sstream>

#include "bluetooth-controller.hpp"
#include "bluetooth-utils.hpp"
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

	Vector2 topCorner = raylib::ConstructVector2(120, 148);
	Vector2 panelDimensions = raylib::ConstructVector2(GetScreenWidth() - 240, GetScreenHeight() - 240);
	panelRec = raylib::ConstructRectangle(topCorner.x, topCorner.y, panelDimensions.x, panelDimensions.y);
	panelContentRec.width = panelRec.width - (GuiGetStyle(SCROLLBAR, SLIDER_WIDTH) + 8);
	

	GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll, &panelView);

	BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
	//GuiGrid(raylib::ConstructRectangle(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y, panelContentRec.width, panelContentRec.height), NULL, 16, 3, NULL);
	int panelInnerHeight = 0;

	std::vector<SimpleBLE::Peripheral> discoveredDevices = BluetoothController::GetDiscoveredDevices();
	int actualIndex = 0;
	for (int i = 0; i < discoveredDevices.size(); i++) {
		if (discoveredDevices.at(i).identifier().length() != 0) {
			/*RelativeDrawing::DrawTextRelEx(
				fontType, 
				TextFormat("Device: %s, %s", discoveredDevices.at(i).identifier().c_str(), discoveredDevices.at(i).address().c_str()), 
				raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + 32 * actualIndex), 
				RelativeDrawing::TopLeft, 
				RelativeDrawing::TopLeft, 
				24, 
				1.5, 
				BLACK
			);*/

			int height = DrawDiscoveredBluetoothDevice(
				discoveredDevices.at(i),
				raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight),
				panelContentRec.width
			);

			panelInnerHeight += height;
			actualIndex++;
		}
	}
	panelContentRec.height = panelInnerHeight != panelContentRec.height ? panelInnerHeight : panelContentRec.height;
	//panelContentRec.height = actualIndex * 32 != panelContentRec.height ? actualIndex * 32 : panelContentRec.height;

	EndScissorMode();

	//if (showContentArea) DrawRectangle(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y, panelContentRec.width, panelContentRec.height, Fade(RED, 0.1));

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

int MainMenuScene::DrawDiscoveredBluetoothDevice(SimpleBLE::Peripheral device, Vector2 position, int width)
{
	Font fontType = FontSettings::GetMainFont();

	RelativeDrawing::DrawRectangle(
		position,
		raylib::ConstructVector2(width, 64),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		raylib::ConstructColor(255, 255, 255)
	);
	RelativeDrawing::DrawRectangle(
		raylib::ConstructVector2(position.x, position.y + 64),
		raylib::ConstructVector2(width, 2),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		//raylib::ConstructColor(0, 206, 206)
		raylib::ConstructColor(206, 206, 206)
	);

	std::vector<SimpleBLE::Service> deviceServices = device.services();
	std::stringstream ss;
	for (int i = 0; i < deviceServices.size(); i++) {
		ss << BleUtils::ToString(BleUtils::GetServiceType(deviceServices.at(i).uuid()));
		if (i + 1 < deviceServices.size()) {
			ss << ", ";
		}
	}

	RelativeDrawing::DrawTextRelEx(
		fontType,
		TextFormat("%s - %s", device.identifier().c_str(), ss.str().c_str()),
		raylib::ConstructVector2(position.x + 10, position.y + 10),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		24,
		1.5,
		BLACK
	);

	RelativeDrawing::DrawTextRelEx(
		fontType,
		TextFormat("%s", device.address().c_str()),
		raylib::ConstructVector2(position.x + 10, position.y + 10 + 24),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		16,
		1.5,
		BLACK
	);

	/*RelativeDrawing::DrawTextRelEx(
		fontType,
		TextFormat("Device: %s, %s", device.identifier().c_str(), device.address().c_str()),
		position,
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		24,
		1.5,
		BLACK
	);*/

	return 66;
}
