#pragma once

#include "scene.hpp"
#include "workout-definition.hpp"

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

	Rectangle panelRec = { 20, 40, 200, 150 };
	Rectangle panelContentRec = { 0, 0, 0, 0 };
	Rectangle panelView = { 0 };
	Vector2 panelScroll = { 0 };

	std::vector<WorkoutDefinition*> workouts;

	int DrawWorkoutListHeading(std::string heading, Vector2 position, int width);

	bool WorkoutSelectionMenuScene::DrawWorkoutListButton(std::string text, Vector2 position, int width, int& heightReturn);

	int DrawWorkoutButton(WorkoutDefinition* device, Vector2 position, int width);
};