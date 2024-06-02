#pragma once

#include "scene.hpp"

#include "raylib.h"
#include <simpleble/SimpleBLE.h>

class WorkoutSelectionMenuScene : public Scene {
public:
	WorkoutSelectionMenuScene();
	~WorkoutSelectionMenuScene();

	int UpdateLogic();
	int DrawCall();

private:
	Texture menuBackground;
	bool menuOpen = false;
};