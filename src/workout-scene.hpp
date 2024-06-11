#pragma once

#include "scene.hpp"
#include "workout-definition.hpp"

#include "raylib.h"
#include <simpleble/SimpleBLE.h>

class WorkoutScene : public Scene {
public:
	WorkoutScene();
	~WorkoutScene();

	int UpdateLogic();
	int DrawCall();

private:
};