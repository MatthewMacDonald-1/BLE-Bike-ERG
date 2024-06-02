#include "workout-selection-scene.hpp"
#include <iostream>

WorkoutSelectionMenuScene::WorkoutSelectionMenuScene()
{
	menuBackground = LoadTexture("./resources/images/test-background.png");
}

WorkoutSelectionMenuScene::~WorkoutSelectionMenuScene()
{
	UnloadTexture(menuBackground);
}

int WorkoutSelectionMenuScene::UpdateLogic()
{
	return EXIT_SUCCESS;
}

int WorkoutSelectionMenuScene::DrawCall()
{
	return EXIT_SUCCESS;
}
