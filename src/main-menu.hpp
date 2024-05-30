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
};