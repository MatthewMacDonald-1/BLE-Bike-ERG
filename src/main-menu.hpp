#pragma once

#include "scene.hpp"

#include "raylib.h"

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
};