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

	if (!started) GuiLock();

	Font fontType = FontSettings::GetMainFont();

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	workout->DrawWorkout(raylib::ConstructVector2(24, 24), GetScreenWidth() - 24 * 2, GetScreenHeight() - 24 * 4, raylib::ConstructColor(0, 178, 255, 255), 128);

	bool pauseRes = RelativeDrawing::GuiButtonRelative((!paused ? "Pause" : "Play"), offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (pauseRes) {
		paused = !paused;
	}

	GuiUnlock();

	if (!started) {
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), raylib::ConstructColor(0, 0, 0, 100));

		RelativeDrawing::DrawTextRelEx(
			fontType,
			workout->GetName().c_str(),
			raylib::ConstructVector2(0, -64),
			RelativeDrawing::Center,
			RelativeDrawing::Center,
			64,
			1.5,
			BLACK
		);

		bool startRes = RelativeDrawing::GuiButtonRelative("Start", raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::Center, RelativeDrawing::Center, 24);
		if (startRes) {
			started = true;

			// Set up data logging.

		}
		bool backRes = RelativeDrawing::GuiButtonRelative("Cancel", raylib::ConstructVector2(0, 40), buttonSize, RelativeDrawing::Center, RelativeDrawing::Center, 24);
		if (backRes) {
			SceneManager::LoadScene("WorkoutSelectionMenu");
		}
	}

	return EXIT_SUCCESS;
}
