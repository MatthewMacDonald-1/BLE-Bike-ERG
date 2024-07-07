#include "main-menu.hpp"

#include "config.hpp"

#include "raygui.h"
#include "raymath.h"
#include <string>
#include <sstream>
#include <thread>
#include <set>
#include <vector>

#include "bluetooth-controller.hpp"
#include "font-settings.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "settings-menu.hpp"
#include "scene-manager.hpp"

using namespace MattsUtils;

MainMenuScene::MainMenuScene()
{
	menuBackground = LoadTexture("./resources/images/test-background.png"); // Important Note: the file path is relative to the executable
	hrIcon = LoadTexture("./resources/icons/hr.png"); // Important Note: the file path is relative to the executable
	cadenceIcon = LoadTexture("./resources/icons/cadence.png"); // Important Note: the file path is relative to the executable
	trainerIcon = LoadTexture("./resources/icons/trainer.png"); // Important Note: the file path is relative to the executable
	powerIcon = LoadTexture("./resources/icons/power.png"); // Important Note: the file path is relative to the executable
	BluetoothController::StartScan();
}

MainMenuScene::~MainMenuScene()
{
	UnloadTexture(menuBackground);
	UnloadTexture(hrIcon);
	UnloadTexture(cadenceIcon);
	UnloadTexture(trainerIcon);
	UnloadTexture(powerIcon);
}

int MainMenuScene::UpdateLogic()
{
	return EXIT_SUCCESS;
}

int MainMenuScene::DrawCall()
{
	if (!connectingScreen) {
		BluetoothController::StartScan();
	}
	

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

	if (connectingScreen) {
		if (BluetoothController::GetActiveConnectionThreads() == 0) {
			BluetoothController::StopScan();
			SceneManager::LoadScene("WorkoutSelectionMenu");
			connectingScreen = false;
			disconnectingScreen = true;
		}

		// Dot animation with sin waves
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), raylib::ConstructColor(200, 200, 200, 155));

		std::stringstream dots;
		int numDots = (
			std::cos(2 * GetTime() + PI / 2) < 0 ? 
			(int)std::round(std::abs(std::sin(GetTime())) * 3) : 
			(int)std::round(std::abs(std::cos(GetTime())) * 3)
		);
		for (int i = 0; i < numDots; i++) {
			dots << '.';
		}

		RelativeDrawing::DrawTextRelEx(fontType, "Connecting", raylib::ConstructVector2(0, -24), RelativeDrawing::Center, RelativeDrawing::Center, 64, 1.5, BLACK);
		RelativeDrawing::DrawTextRelEx(fontType, dots.str().c_str(), raylib::ConstructVector2(144, -24), RelativeDrawing::Center, RelativeDrawing::MiddleLeft, 64, 1.5, BLACK); // Draw the dot animation

		RelativeDrawing::DrawTextRelEx(fontType, TextFormat("Waiting on %d devices.", BluetoothController::GetActiveConnectionThreads()), raylib::ConstructVector2(0, 24), RelativeDrawing::Center, RelativeDrawing::Center, 24, 1.5, DARKGRAY);

		return EXIT_SUCCESS;
	}

	if (disconnectingScreen) {
		if (BluetoothController::GetActiveDisconnectionThreads() == 0) {
			disconnectingScreen = false;
		}

		// Dot animation with sin waves
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), raylib::ConstructColor(200, 200, 200, 155));

		std::stringstream dots;
		int numDots = (
			std::cos(2 * GetTime() + PI / 2) < 0 ?
			(int)std::round(std::abs(std::sin(GetTime())) * 3) :
			(int)std::round(std::abs(std::cos(GetTime())) * 3)
			);
		for (int i = 0; i < numDots; i++) {
			dots << '.';
		}

		RelativeDrawing::DrawTextRelEx(fontType, "Disconnecting", raylib::ConstructVector2(0, -24), RelativeDrawing::Center, RelativeDrawing::Center, 64, 1.5, BLACK);
		RelativeDrawing::DrawTextRelEx(fontType, dots.str().c_str(), raylib::ConstructVector2(144, -24), RelativeDrawing::Center, RelativeDrawing::MiddleLeft, 64, 1.5, BLACK); // Draw the dot animation

		RelativeDrawing::DrawTextRelEx(fontType, TextFormat("Waiting on %d devices.", BluetoothController::GetActiveDisconnectionThreads()), raylib::ConstructVector2(0, 24), RelativeDrawing::Center, RelativeDrawing::Center, 24, 1.5, DARKGRAY);

		return EXIT_SUCCESS;
	}

	RelativeDrawing::DrawTextRelEx(fontType, "Bike ERG", raylib::ConstructVector2(0, 16), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 64, 1.5, BLACK);

	int boxSpacing = 6;
	//Vector2 connectionBoxDimensions = MattsUtils::raylib::ConstructVector2(384, 192);
	Vector2 connectionBoxDimensions = MattsUtils::raylib::ConstructVector2((GetScreenWidth() / 2) - 24 - boxSpacing, 192);
	Vector2 center = MattsUtils::raylib::ConstructVector2(GetScreenWidth() / 2, GetScreenHeight() / 2);
	

	DrawDeviceConnectionBox(Vector2Add(center, MattsUtils::raylib::ConstructVector2(-boxSpacing + -connectionBoxDimensions.x, -boxSpacing + -connectionBoxDimensions.y)), connectionBoxDimensions, hrIcon, 128, BleUtils::HEART_RATE, hrPanelContentRec, hrPanelView, hrPanelScroll, hrSelected);

	DrawDeviceConnectionBox(Vector2Add(center, MattsUtils::raylib::ConstructVector2(-boxSpacing + -connectionBoxDimensions.x, boxSpacing)), connectionBoxDimensions, cadenceIcon, 128, BleUtils::CYCLING_SPEED_CADENCE, cadencePanelContentRec, cadencePanelView, cadencePanelScroll, cadenceSelected);

	DrawDeviceConnectionBox(Vector2Add(center, MattsUtils::raylib::ConstructVector2(boxSpacing, -boxSpacing + -connectionBoxDimensions.y)), connectionBoxDimensions, trainerIcon, 128, BleUtils::FITNESS_MACHINE, trainerPanelContentRec, trainerPanelView, trainerPanelScroll, trainerSelected);

	DrawDeviceConnectionBox(Vector2Add(center, MattsUtils::raylib::ConstructVector2(boxSpacing, boxSpacing)), connectionBoxDimensions, powerIcon, 128, BleUtils::CYCLING_POWER, powerPanelContentRec, powerPanelView, powerPanelScroll, powerSelected);


	// Buttons Start ------------------------------------------------------------------------------
	GuiSetFont(fontType);

	bool nextSceneRes = RelativeDrawing::GuiButtonRelative("Continue", offsetDstBR, buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);
	if (nextSceneRes) {
		BluetoothController::StopScan();
		
		// Create list of bluetooth devices for connection
		std::vector<SimpleBLE::Peripheral> discoveredDevices = BluetoothController::GetDiscoveredDevices();
		std::set<SimpleBLE::BluetoothAddress> selectedDevicesAddressSet;
		std::vector<SimpleBLE::Peripheral> selectedDevicesList;
		

		int hrIdx = 0;
		int powerIdx = 0;
		int cadenceIdx = 0;
		int trainerIdx = 0;

		for (int i = 0; i < discoveredDevices.size(); i++) {
			bool containsHR = false;
			bool containsPower = false;
			bool containsCadence = false;
			bool containsTrainer = false;

			std::vector<SimpleBLE::Service> services;
			try {
				services = discoveredDevices.at(i).services();
			}
			catch (...) {
				continue;
			}

			for (int j = 0; j < services.size(); j++) {
				BluetoothController::ServiceType serviceType = BluetoothController::GetServiceType(services.at(j).uuid());

				switch (serviceType)
				{
				case BluetoothController::UNKNOWN:
					break;
				case BluetoothController::HEART_RATE:
					containsHR = true;
					break;
				case BluetoothController::CYCLING_POWER:
					containsPower = true;
					break;
				case BluetoothController::CYCLING_SPEED_CADENCE:
					containsCadence = true;
					break;
				case BluetoothController::FITNESS_MACHINE:
					containsTrainer = true;
					break;
				default:
					break;
				}
			}

			if (containsHR && hrSelected == hrIdx) {
				std::pair<std::set<SimpleBLE::BluetoothAddress>::iterator, bool> res = selectedDevicesAddressSet.insert(discoveredDevices.at(i).address());
				if (res.second) {
					selectedDevicesList.push_back(discoveredDevices.at(i));
				}

				BluetoothController::SetServiceDeviceMap(BluetoothController::HEART_RATE, discoveredDevices.at(i).address());
			}
			if (containsPower && powerSelected == powerIdx) {
				std::pair<std::set<SimpleBLE::BluetoothAddress>::iterator, bool> res = selectedDevicesAddressSet.insert(discoveredDevices.at(i).address());
				if (res.second) {
					selectedDevicesList.push_back(discoveredDevices.at(i));
				}

				BluetoothController::SetServiceDeviceMap(BluetoothController::CYCLING_POWER, discoveredDevices.at(i).address());
			}
			if (containsCadence && cadenceSelected == cadenceIdx) {
				std::pair<std::set<SimpleBLE::BluetoothAddress>::iterator, bool> res = selectedDevicesAddressSet.insert(discoveredDevices.at(i).address());
				if (res.second) {
					selectedDevicesList.push_back(discoveredDevices.at(i));
				}

				BluetoothController::SetServiceDeviceMap(BluetoothController::CYCLING_SPEED_CADENCE, discoveredDevices.at(i).address());
			}
			if (containsTrainer && trainerSelected == trainerIdx) {
				std::pair<std::set<SimpleBLE::BluetoothAddress>::iterator, bool> res = selectedDevicesAddressSet.insert(discoveredDevices.at(i).address());
				if (res.second) {
					selectedDevicesList.push_back(discoveredDevices.at(i));
				}

				BluetoothController::SetServiceDeviceMap(BluetoothController::FITNESS_MACHINE, discoveredDevices.at(i).address());
			}

			if (containsHR) hrIdx++;
			if (containsPower) powerIdx++;
			if (containsCadence) cadenceIdx++;
			if (containsTrainer) trainerIdx++;

		}

		// Initiate connections with the selected devices
		for (int i = 0; i < selectedDevicesList.size(); i++) {
			std::thread connectThread(BluetoothController::ConnectToDevice, selectedDevicesList.at(i));
			connectThread.detach(); // Runs the thread detached
		}
		
		
		connectingScreen = true;
	}


	bool settingRes = RelativeDrawing::GuiButtonRelative("Settings", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);
	if (settingRes) {
		menuOpen = true;
	}

	bool quitRes = RelativeDrawing::GuiButtonRelative("Quit", offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (quitRes) {
		return SIGNAL_WINDOW_CLOSE;
	}

	// Buttons End --------------------------------------------------------------------------------


	return EXIT_SUCCESS;
}

void MainMenuScene::DrawDeviceConnectionBox(Vector2 position, Vector2 dimensions, Texture icon, int iconWidth, BleUtils::ServiceType type, Rectangle& panelContentRec, Rectangle& panelView, Vector2& panelScroll, int &selectedIndex)
{
	Font fontType = FontSettings::GetMainFont();

	DrawRectangle(position.x, position.y, dimensions.x, dimensions.y, GRAY);

	DrawRectangle(position.x, position.y, iconWidth, dimensions.y, WHITE);
	double scale = 1;
	scale = (double)iconWidth / (double)icon.width;
	int vOffset = ((int)dimensions.y - (int)std::ceil((double)icon.height * scale)) / 2;
	DrawTextureEx(icon, Vector2Add(position, MattsUtils::raylib::ConstructVector2(0, vOffset)), 0, (float)scale, WHITE);


	// Scroll Area
	Vector2 topCorner = raylib::ConstructVector2(position.x + iconWidth, position.y);
	Vector2 panelDimensions = raylib::ConstructVector2(dimensions.x - iconWidth, dimensions.y);
	Rectangle panelRec = raylib::ConstructRectangle(topCorner.x, topCorner.y, panelDimensions.x, panelDimensions.y);
	panelContentRec.width = panelRec.width - (GuiGetStyle(SCROLLBAR, SLIDER_WIDTH) + 8);

	GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll, &panelView);

	BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
	int panelInnerHeight = 0;

	int buttonWidth = panelContentRec.width;
	std::vector<SimpleBLE::Peripheral> discoveredDevices = BluetoothController::GetDiscoveredDevices();

	int ofTypeIdx = 0;
	for (int i = 0; i < discoveredDevices.size(); i++) {
		Vector2 buttonPosition = raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight);
		std::vector<SimpleBLE::Service> services;

		try {
			services = discoveredDevices.at(i).services();
		}
		catch (...) {
			continue;
		}

		bool containsMatch = false;
		for (int j = 0; j < services.size(); j++) {
			if (BleUtils::GetServiceType(services.at(j).uuid()) == type) {
				containsMatch = true;
				break;
			}
		}

		if (containsMatch) {
			RelativeDrawing::DrawRectangle(
				buttonPosition,
				raylib::ConstructVector2(buttonWidth, 64),
				RelativeDrawing::TopLeft,
				RelativeDrawing::TopLeft,
				raylib::ConstructColor(255, 255, 255)
			);
			RelativeDrawing::DrawRectangle(
				raylib::ConstructVector2(buttonPosition.x, buttonPosition.y + 64),
				raylib::ConstructVector2(buttonWidth, 2),
				RelativeDrawing::TopLeft,
				RelativeDrawing::TopLeft,
				raylib::ConstructColor(206, 206, 206)
			);

			RelativeDrawing::DrawTextRelEx(
				fontType,
				TextFormat("%s", discoveredDevices.at(i).identifier().c_str()),
				raylib::ConstructVector2(buttonPosition.x + 10, buttonPosition.y + 10),
				RelativeDrawing::TopLeft,
				RelativeDrawing::TopLeft,
				24,
				1.5,
				BLACK
			);

			RelativeDrawing::DrawTextRelEx(
				fontType,
				TextFormat("%s", discoveredDevices.at(i).address().c_str()),
				raylib::ConstructVector2(buttonPosition.x + 10, buttonPosition.y + 10 + 24),
				RelativeDrawing::TopLeft,
				RelativeDrawing::TopLeft,
				16,
				1.5,
				BLACK
			);

			int selectButtonWidth = 80;

			bool checked = (ofTypeIdx == selectedIndex);
			if (checked) {
				RelativeDrawing::GuiCheckBoxRelative(
					"",
					&checked,
					raylib::ConstructVector2(buttonPosition.x + buttonWidth - 10 - 24, buttonPosition.y + 20),
					raylib::ConstructVector2(24, 24),
					RelativeDrawing::TopLeft,
					RelativeDrawing::TopLeft,
					16
				);
				if (CheckCollisionPointRec(GetMousePosition(), MattsUtils::raylib::ConstructRectangle(buttonPosition.x, buttonPosition.y, buttonWidth, 64))) {
					bool res = RelativeDrawing::GuiButtonRelative(
						"Deselect",
						raylib::ConstructVector2(buttonPosition.x + buttonWidth - 10 - selectButtonWidth, buttonPosition.y + 16),
						raylib::ConstructVector2(selectButtonWidth, 32),
						RelativeDrawing::TopLeft,
						RelativeDrawing::TopLeft,
						20
					);

					if (res) {
						selectedIndex = -1;
					}
				}
			}
			else {
				if (CheckCollisionPointRec(GetMousePosition(), MattsUtils::raylib::ConstructRectangle(buttonPosition.x, buttonPosition.y, buttonWidth, 64))) {
					bool res = RelativeDrawing::GuiButtonRelative(
						"Select",
						raylib::ConstructVector2(buttonPosition.x + buttonWidth - 10 - selectButtonWidth, buttonPosition.y + 16),
						raylib::ConstructVector2(selectButtonWidth, 32),
						RelativeDrawing::TopLeft,
						RelativeDrawing::TopLeft,
						20
					);

					if (res) {
						selectedIndex = ofTypeIdx;
					}
				}
			}

			panelInnerHeight += 66;
			ofTypeIdx++;
		}
	}

	panelContentRec.height = panelInnerHeight != panelContentRec.height ? panelInnerHeight : panelContentRec.height;

	EndScissorMode();
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
