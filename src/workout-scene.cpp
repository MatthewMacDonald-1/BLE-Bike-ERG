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
#include "MattsUtils/time.hpp"
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

	Color dataDisplayBackground = raylib::ConstructColor(64, 64, 64);
	Color graphAreaBackground = raylib::ConstructColor(32, 32, 32);
	
	int dataDisplayHeight = 128;
	int graphAreaHeight = GetScreenHeight() - dataDisplayHeight - 32;
	DrawRectangle(0, 0, GetScreenWidth(), dataDisplayHeight, dataDisplayBackground);
	DrawRectangle(0, dataDisplayHeight, GetScreenWidth(), graphAreaHeight, graphAreaBackground);
	DrawRectangle(0, GetScreenHeight() - 32, GetScreenWidth(), 32, dataDisplayBackground);

	// Data

	int targetPower = workout->EvaluateWorkoutAt((int)workoutTime);
	int* currentPower = NULL;
	int* currentHeartRate = NULL;
	int* currentCadence = NULL;

	DrawDataValue(
		fontType, 
		"Elapsed Time", 
		MattsUtils::Time::ToString(workoutTime),
		raylib::ConstructVector2(0, 10)
	);

	DrawDataValue(
		fontType,
		"Interval Time",
		MattsUtils::Time::ToString(0),
		raylib::ConstructVector2(0, 70)
	);

	DrawDataValue(
		fontType,
		"Power",
		std::string(TextFormat("%s", (currentPower == NULL ? "--" : TextFormat("%d", *currentPower)))),
		raylib::ConstructVector2(-GetScreenWidth() / 4, 10)
	);

	DrawDataValue(
		fontType,
		"Target Power",
		std::string(TextFormat("%d", targetPower)),
		raylib::ConstructVector2(-GetScreenWidth() / 4, 70)
	);

	DrawDataValue(
		fontType,
		"Cadence",
		std::string(TextFormat("%s", (currentCadence == NULL ? "--" : TextFormat("%d", *currentCadence)))),
		raylib::ConstructVector2(GetScreenWidth() / 4, 10)
	);

	DrawDataValue(
		fontType,
		"Heart Rate",
		std::string(TextFormat("%s", (currentHeartRate == NULL ? "--" : TextFormat("%d", *currentHeartRate)))),
		raylib::ConstructVector2(GetScreenWidth() / 4, 70)
	);


	// Graph Stuff Start
	Color graphScaleLines = raylib::ConstructColor(120, 120, 120);
	Color graphScaleLineFTP100 = raylib::ConstructColor(255, 255, 255);

	int actualFTP = 243;
	int ftp = workout->GetTargetType() == WorkoutDefinition::RAW_POWER ? 100 : actualFTP;
	int highestTarget = (int)(((double)workout->GetHighestTarget() / 100.0) * (double)ftp);
	int highestGraphPoint = (int)std::ceil((double)highestTarget / 100.0) * 100;

	int workoutDrawHeight = (int)(((double)highestTarget / (double)highestGraphPoint) * (double)graphAreaHeight);

	double oneWattDist = (double)graphAreaHeight / (double)highestGraphPoint;

	for (int i = 100; i < highestGraphPoint; i += 100) {
		int y = dataDisplayHeight + (graphAreaHeight - (oneWattDist * i));
		DrawLine(0, y, GetScreenWidth(), y, graphScaleLines);
		RelativeDrawing::DrawTextRelEx(fontType, TextFormat("%d", i),
			raylib::ConstructVector2(3, y - 1),
			RelativeDrawing::TopLeft,
			RelativeDrawing::BottomLeft,
			16,
			1.0,
			graphScaleLines
		);
	}

	workout->DrawWorkout(raylib::ConstructVector2(0, dataDisplayHeight + (graphAreaHeight - workoutDrawHeight)), GetScreenWidth(), workoutDrawHeight, raylib::ConstructColor(0, 178, 255, 255), 0);

	int ftp_y = dataDisplayHeight + (graphAreaHeight - (oneWattDist * actualFTP));
	DrawLine(0, ftp_y, GetScreenWidth(), ftp_y, graphScaleLineFTP100);

	RelativeDrawing::DrawTextRelEx(fontType, TextFormat("FTP %d", actualFTP),
		raylib::ConstructVector2(-3, ftp_y - 1),
		RelativeDrawing::TopRight,
		RelativeDrawing::BottomRight,
		16,
		1.0,
		graphScaleLineFTP100
	);
	// Graph Stuff end

	bool pauseRes = RelativeDrawing::GuiButtonRelative((!paused ? "Pause" : "Play"), raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (pauseRes) {
		paused = !paused;
	}

	bool finishRes = RelativeDrawing::GuiButtonRelative("Finish", raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);
	

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
			WHITE
		);

		bool startRes = RelativeDrawing::GuiButtonRelative("Start", raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::Center, RelativeDrawing::Center, 24);
		if (startRes) {
			started = true;
			workoutTime = 0;
			// Set up data logging.

		}
		bool backRes = RelativeDrawing::GuiButtonRelative("Cancel", raylib::ConstructVector2(0, 40), buttonSize, RelativeDrawing::Center, RelativeDrawing::Center, 24);
		if (backRes) {
			SceneManager::LoadScene("WorkoutSelectionMenu");
		}
	}

	if (started && !paused) {
		workoutTime += GetFrameTime();
	}

	return EXIT_SUCCESS;
}

bool WorkoutScene::DrawDataValue(Font font, std::string heading, std::string value, Vector2 position)
{
	using namespace MattsUtils;

	Color valueNameColor = raylib::ConstructColor(200, 200, 200);
	Color valueColor = raylib::ConstructColor(255, 255, 255);
	int valueNameFontSize = 16;
	int valueFontSize = 32;

	RelativeDrawing::DrawTextRelEx(
		font,
		heading.c_str(),
		position,
		RelativeDrawing::TopCenter,
		RelativeDrawing::TopCenter,
		valueNameFontSize,
		1,
		valueNameColor
	);
	RelativeDrawing::DrawTextRelEx(
		font,
		value.c_str(),
		raylib::ConstructVector2(position.x, position.y + 16),
		RelativeDrawing::TopCenter,
		RelativeDrawing::TopCenter,
		valueFontSize,
		1,
		valueColor
	);
}
