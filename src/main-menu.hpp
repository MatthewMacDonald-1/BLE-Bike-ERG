#pragma once

#include "scene.hpp"

#include "raylib.h"
#include <simpleble/SimpleBLE.h>
#include "bluetooth-utils.hpp"

class MainMenuScene : public Scene {
public:
	MainMenuScene();
	~MainMenuScene();

	int UpdateLogic();
	int DrawCall();

private:
	Texture menuBackground;
	Texture hrIcon;
	Texture cadenceIcon;
	Texture trainerIcon;
	Texture powerIcon;

	Rectangle hrPanelContentRec = { 0, 0, 0, 0 };
	Rectangle hrPanelView = { 0 };
	Vector2 hrPanelScroll = { 0 };
	int hrSelected = 0;

	Rectangle cadencePanelContentRec = { 0, 0, 0, 0 };
	Rectangle cadencePanelView = { 0 };
	Vector2 cadencePanelScroll = { 0 };
	int cadenceSelected = 0;

	Rectangle trainerPanelContentRec = { 0, 0, 0, 0 };
	Rectangle trainerPanelView = { 0 };
	Vector2 trainerPanelScroll = { 0 };
	int trainerSelected = 0;

	Rectangle powerPanelContentRec = { 0, 0, 0, 0 };
	Rectangle powerPanelView = { 0 };
	Vector2 powerPanelScroll = { 0 };
	int powerSelected = 0;

	bool menuOpen = false;
	bool connectingScreen = false;

	void DrawDeviceConnectionBox(Vector2 position, Vector2 dimensions, Texture icon, int iconWidth, BleUtils::ServiceType type, Rectangle& panelContentRec, Rectangle& panelView, Vector2& panelScroll, int& selectedIndex);

	int DrawBluetoothDeviceListHeading(std::string heading, Vector2 position, int width);

	//int DrawPairedBluetoothDevice(SimpleBLE::Peripheral device, Vector2 position, int width);

	//int DrawDiscoveredBluetoothDevice(SimpleBLE::Peripheral device, Vector2 position, int width);
};