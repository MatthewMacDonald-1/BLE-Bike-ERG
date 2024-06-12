#pragma once

#include "scene.hpp"
#include "workout-definition.hpp"

#include "raylib.h"
#include <simpleble/SimpleBLE.h>

class WorkoutScene : public Scene {
public:
	WorkoutScene(WorkoutDefinition* workoutSrc);
	~WorkoutScene();

	int UpdateLogic();
	int DrawCall();

private:
	WorkoutDefinition* workout;

	bool started = false;
	bool paused = true;
};