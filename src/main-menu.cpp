#include "main-menu.hpp"

#include "config.hpp"

#include "raygui.h"
#include "raymath.h"
#include <string>
#include <sstream>
#include <thread>

#include "bluetooth-controller.hpp"
#include "bluetooth-utils.hpp"
#include "font-settings.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "settings-menu.hpp"

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

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	DrawTextureEx(menuBackground, raylib::ConstructVector2(0, 0), 0, (float)GetScreenWidth() / (float)menuBackground.width, WHITE);

	if (menuOpen) {
		//DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), raylib::ConstructColor(0, 0, 0, 100));
		int res = SettingsMenu::DrawSettingsMenu();
		if (res == SIGNAL_SETTINGS_MENU_CLOSE) {
			menuOpen = false;
			return EXIT_SUCCESS;
		}
		else {
			return res;
		}
	}

	

	RelativeDrawing::DrawTextRelEx(fontType, "Bike ERG", raylib::ConstructVector2(0, 0), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 128, 1.5, BLACK);

	// Scroll Area Start --------------------------------------------------------------------------

	Vector2 topCorner = raylib::ConstructVector2(120, 148);
	Vector2 panelDimensions = raylib::ConstructVector2(GetScreenWidth() - 240, GetScreenHeight() - 240);
	panelRec = raylib::ConstructRectangle(topCorner.x, topCorner.y, panelDimensions.x, panelDimensions.y);
	panelContentRec.width = panelRec.width - (GuiGetStyle(SCROLLBAR, SLIDER_WIDTH) + 8);
	

	GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll, &panelView);

	BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
	int panelInnerHeight = 0;

	std::vector<SimpleBLE::Peripheral> pairedDevices = BluetoothController::GetConnectedDevices();
	if (pairedDevices.size() > 0) {
		panelInnerHeight += DrawBluetoothDeviceListHeading(
			"Connected Devices",
			raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight),
			panelRec.width
		);
	}
	for (int i = 0; i < pairedDevices.size(); i++) {
		if (pairedDevices.at(i).identifier().length() != 0) {

			panelInnerHeight += DrawPairedBluetoothDevice(
				pairedDevices.at(i),
				raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight),
				panelContentRec.width
			);
		}
	}


	// Draw discovered devices
	std::vector<SimpleBLE::Peripheral> discoveredDevices = BluetoothController::GetDiscoveredDevices();
	bool hasdrawnHeading = false;

	for (int i = 0; i < discoveredDevices.size(); i++) {
		if (discoveredDevices.at(i).identifier().length() != 0) {
			if (!hasdrawnHeading) {
				panelInnerHeight += DrawBluetoothDeviceListHeading(
					"Discovered Devices", 
					raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight),
					panelRec.width
				);

				hasdrawnHeading = true;
			}

			// Check that this device is not already connected too.
			bool unconnected = true;
			for (int j = 0; j < pairedDevices.size(); j++) {
				if (discoveredDevices.at(i).address() == pairedDevices.at(j).address()) {
					unconnected = false;
					break;
				}
			}

			if (unconnected) {
				panelInnerHeight += DrawDiscoveredBluetoothDevice(
					discoveredDevices.at(i),
					raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight),
					panelContentRec.width
				);
			}
		}
	}
	panelContentRec.height = panelInnerHeight != panelContentRec.height ? panelInnerHeight : panelContentRec.height;

	EndScissorMode();


	// Scroll Area End ----------------------------------------------------------------------------


	// Buttons Start ------------------------------------------------------------------------------
	GuiSetFont(fontType);

	bool nextSceneRes = RelativeDrawing::GuiButtonRelative("Next", offsetDstBR, buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);

	bool settingRes = RelativeDrawing::GuiButtonRelative("Settings", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);
	if (settingRes) {
		menuOpen = true;
	}
	// Buttons End --------------------------------------------------------------------------------


	return EXIT_SUCCESS;
}

int MainMenuScene::DrawBluetoothDeviceListHeading(std::string heading, Vector2 position, int width)
{
	Font fontType = FontSettings::GetMainFont();

	RelativeDrawing::DrawRectangle(
		position,
		raylib::ConstructVector2(width, 44),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		raylib::ConstructColor(255, 255, 255)
	);
	RelativeDrawing::DrawRectangle(
		raylib::ConstructVector2(position.x, position.y + 44),
		raylib::ConstructVector2(width, 2),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		raylib::ConstructColor(206, 206, 206)
	);

	RelativeDrawing::DrawTextRelEx(
		fontType,
		heading.c_str(),
		raylib::ConstructVector2(position.x + 10, position.y + 10),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		24,
		1.5,
		BLACK
	);

	return 46;
}

int MainMenuScene::DrawPairedBluetoothDevice(SimpleBLE::Peripheral device, Vector2 position, int width)
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
		raylib::ConstructColor(206, 206, 206)
	);

	std::vector<SimpleBLE::Service> deviceServices = device.services();
	std::stringstream ss;
	bool containsUnknwon = false || deviceServices.size() == 0;
	for (int i = 0; i < deviceServices.size(); i++) {
		BleUtils::ServiceType type = BleUtils::GetServiceType(deviceServices.at(i).uuid());
		if (type == BleUtils::UNKNOWN) {
			containsUnknwon = true;
		}

		ss << BleUtils::ToString(type);
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

	int clicked = RelativeDrawing::GuiButtonRelative(
		"Disconnect",
		raylib::ConstructVector2(position.x + width - 10 - 128, position.y + 16),
		raylib::ConstructVector2(128, 32),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		24
	);

	if (clicked) {
		//TraceLog(LOG_INFO, "Menu Button Clicked");
		std::thread disconnectThread(BluetoothController::DisconnectFromDevice, device);

		disconnectThread.detach();
	}

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

	return 66;
}

int MainMenuScene::DrawDiscoveredBluetoothDevice(SimpleBLE::Peripheral device, Vector2 position, int width)
{
	Font fontType = FontSettings::GetMainFont();

	std::vector<SimpleBLE::Service> deviceServices = device.services();
	std::stringstream ss;
	bool containsUnknwon = false || deviceServices.size() == 0;
	for (int i = 0; i < deviceServices.size(); i++) {
		BleUtils::ServiceType type = BleUtils::GetServiceType(deviceServices.at(i).uuid());
		if (type == BleUtils::UNKNOWN) {
			containsUnknwon = true;
		}

		ss << BleUtils::ToString(type);
		if (i + 1 < deviceServices.size()) {
			ss << ", ";
		}
	}

#ifdef HIDE_UNKNOWN_BLE_DEVICES
	if (containsUnknwon) {
		return 0;
	}
#endif // HIDE_UNKNOWN_BLE_DEVICES

	

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
		raylib::ConstructColor(206, 206, 206)
	);

	

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

	if (!containsUnknwon) {
		int clicked = RelativeDrawing::GuiButtonRelative(
			"Connect",
			raylib::ConstructVector2(position.x + width - 10 - 128, position.y + 16),
			raylib::ConstructVector2(128, 32),
			RelativeDrawing::TopLeft,
			RelativeDrawing::TopLeft,
			24
		);

		if (clicked) {

			std::thread connectThread(BluetoothController::ConnectToDevice, device);
			connectThread.detach(); // Runs the thread detached
			//BluetoothController::ConnectToDevice(device);
		}
	}
	

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

	return 66;
}
