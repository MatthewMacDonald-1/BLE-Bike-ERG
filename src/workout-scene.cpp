#include "workout-scene.hpp"
#include "config.hpp"

#include "raygui.h"
#include "raymath.h"
#include <string>
#include <iostream>
#include <sstream>

#include "font-settings.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "settings-menu.hpp"
#include "scene-manager.hpp"

WorkoutScene::WorkoutScene(WorkoutDefinition* workoutSrc)
{
	workout = workoutSrc;
}

WorkoutScene::~WorkoutScene()
{
}

int WorkoutScene::UpdateLogic()
{
	return EXIT_SUCCESS;
}

int WorkoutScene::DrawCall()
{
	using namespace MattsUtils;

	Font fontType = FontSettings::GetMainFont();

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	workout->DrawWorkout(raylib::ConstructVector2(24, 24), GetScreenWidth() - 24 * 2, GetScreenHeight() - 24 * 4, 128);

	bool backRes = RelativeDrawing::GuiButtonRelative("Back", offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (backRes) {
		SceneManager::LoadScene("WorkoutSelectionMenu");
	}

	return EXIT_SUCCESS;
}
