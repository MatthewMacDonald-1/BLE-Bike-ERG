#pragma once

#include "scene.hpp"

#include "raylib.h"
#include <simpleble/SimpleBLE.h>

class MainMenuScene : public Scene {
public:
	MainMenuScene();
	~MainMenuScene();

	int UpdateLogic();
	int DrawCall();

private:
	Texture menuBackground;

	Rectangle panelRec = { 20, 40, 200, 150 };
	Rectangle panelContentRec = { 0, 0, 0, 0 };
	Rectangle panelView = { 0 };
	Vector2 panelScroll = { 0 };
	bool showContentArea = true;

	bool menuOpen = false;

	int DrawBluetoothDeviceListHeading(std::string heading, Vector2 position, int width);

	int DrawPairedBluetoothDevice(SimpleBLE::Peripheral device, Vector2 position, int width);

	int DrawDiscoveredBluetoothDevice(SimpleBLE::Peripheral device, Vector2 position, int width);
};