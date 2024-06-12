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
#include "bluetooth-controller.hpp"

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

	Color graphScaleLines = raylib::ConstructColor(120, 120, 120);
	Color graphScaleLineFTP100 = raylib::ConstructColor(255, 255, 255);

	Color graphHeartRateRecordLineColor = raylib::ConstructColor(255, 39, 0);
	Color graphCadenceRecordLineColor = raylib::ConstructColor(255, 255, 255);
	Color graphProgressLineColor = raylib::ConstructColor(255, 199, 0);
	Color graphProgressCompletedColor = raylib::ConstructColor(255, 199, 0, 50);
	
	int dataDisplayHeight = 128;
	int graphTimeAxisHeight = 20;
	int bottomControlBarHeight = 32;
	int graphAreaHeight = GetScreenHeight() - dataDisplayHeight - graphTimeAxisHeight - bottomControlBarHeight;
	

	DrawRectangle(0, 0, GetScreenWidth(), dataDisplayHeight, dataDisplayBackground);
	DrawRectangle(0, dataDisplayHeight, GetScreenWidth(), graphAreaHeight, graphAreaBackground);
	DrawRectangle(0, dataDisplayHeight + graphAreaHeight, GetScreenWidth(), graphTimeAxisHeight, graphAreaBackground);
	DrawRectangle(0, GetScreenHeight() - bottomControlBarHeight, GetScreenWidth(), bottomControlBarHeight, dataDisplayBackground);

	// Data

	int actualFTP = 243;
	int ftp = workout->GetTargetType() == WorkoutDefinition::RAW_POWER ? 100 : actualFTP;

	int targetPower = (int)std::round(((double)workout->EvaluateWorkoutAt((int)workoutTime) / 100.0) * (double)ftp);

	if ((int)workoutTime != previousFrameIntTime) {
		powerRecord.push_back(currentPower);
		heartRateRecord.push_back(currentHeartRate);
		cadenceRecord.push_back(currentCadence);

		previousFrameIntTime = (int)workoutTime;
	}
	

	std::pair<int, int> intervalTimeData = workout->GetIntervalTime((int)workoutTime);
	int intervalElapsedTime = (int)workoutTime - intervalTimeData.first;
	int intervalLength = intervalTimeData.second - intervalTimeData.first;

	std::stringstream elapsedTimeSS;
	std::string elapsedTime;
	std::string elapsedTimeHeading;

	std::stringstream elapsedIntervalTimeSS;
	std::string elapsedIntervalTime;
	std::string elapsedIntervalTimeHeading;
	switch (timeMode) {
	default:
		elapsedTimeHeading = "Elapsed Time";
		elapsedTime = MattsUtils::Time::ToString(workoutTime);

		elapsedIntervalTimeHeading = "Elapsed Interval Time";
		elapsedIntervalTime = MattsUtils::Time::ToString(intervalElapsedTime);
		break;
	case 1:
		elapsedTimeHeading = "Time Remaining";
		elapsedTimeSS << MattsUtils::Time::ToString(workout->GetWorkoutLength() - workoutTime);
		elapsedTime = elapsedTimeSS.str();

		elapsedIntervalTimeHeading = "Interval Time Remaining";
		elapsedIntervalTimeSS << MattsUtils::Time::ToString(intervalLength - intervalElapsedTime);
		elapsedIntervalTime = elapsedIntervalTimeSS.str();
		break;
	case 2:
		elapsedTimeHeading = "Time";
		elapsedTimeSS << MattsUtils::Time::ToString(workoutTime) << " / " << MattsUtils::Time::ToString(workout->GetWorkoutLength());
		elapsedTime = elapsedTimeSS.str();

		elapsedIntervalTimeHeading = "Interval Time";
		elapsedIntervalTimeSS << MattsUtils::Time::ToString(intervalElapsedTime) << " / " << MattsUtils::Time::ToString(intervalLength);
		elapsedIntervalTime = elapsedIntervalTimeSS.str();
		break;
	}

	bool timeElapsedClicked = DrawDataValue(
		fontType, 
		elapsedTimeHeading, 
		elapsedTime,
		raylib::ConstructVector2(0, 10)
	);

	bool intervalTimeClicked = DrawDataValue(
		fontType,
		elapsedIntervalTimeHeading,
		elapsedIntervalTime,
		raylib::ConstructVector2(0, 70)
	);

	if (timeElapsedClicked || intervalTimeClicked) {
		timeMode++;
		if (timeMode > maxTimeMode) {
			timeMode = 0;
		}
	}

	DrawDataValue(
		fontType,
		"Power",
		std::string(TextFormat("%s", (currentPower == -1 ? "--" : TextFormat("%d", currentPower)))),
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
		std::string(TextFormat("%s", (currentCadence == -1 ? "--" : TextFormat("%d", currentCadence)))),
		raylib::ConstructVector2(GetScreenWidth() / 4, 10)
	);

	DrawDataValue(
		fontType,
		"Heart Rate",
		std::string(TextFormat("%s", (currentHeartRate == -1 ? "--" : TextFormat("%d", currentHeartRate)))),
		raylib::ConstructVector2(GetScreenWidth() / 4, 70)
	);


	// Graph Stuff Start

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

	// Draw Time line
	int x_pos = (int)std::round((double)GetScreenWidth() * ((double)(int)workoutTime / (double)workout->GetWorkoutLength()));

	DrawRectangle(0, dataDisplayHeight, x_pos, graphAreaHeight, graphProgressCompletedColor);

	// Draw Power Record
	int rec_yp, rec_xp;
	for (int i = 0; i < cadenceRecord.size(); i++) {
		int rec_y = dataDisplayHeight + (graphAreaHeight - (oneWattDist * std::max(cadenceRecord.at(i), 0)));
		int rec_x = (int)std::round((double)GetScreenWidth() * ((double)i / (double)workout->GetWorkoutLength()));

		if (i != 0) {
			DrawLine(rec_xp, rec_yp, rec_x, rec_y, graphCadenceRecordLineColor);
		}

		rec_xp = rec_x;
		rec_yp = rec_y;
	}

	for (int i = 0; i < heartRateRecord.size(); i++) {
		int rec_y = dataDisplayHeight + (graphAreaHeight - (oneWattDist * std::max(heartRateRecord.at(i), 0)));
		int rec_x = (int)std::round((double)GetScreenWidth() * ((double)i / (double)workout->GetWorkoutLength()));

		if (i != 0) {
			DrawLine(rec_xp, rec_yp, rec_x, rec_y, graphHeartRateRecordLineColor);
		}

		rec_xp = rec_x;
		rec_yp = rec_y;
	}

	for (int i = 0; i < powerRecord.size(); i++) {
		int rec_y = dataDisplayHeight + (graphAreaHeight - (oneWattDist * std::max(powerRecord.at(i), 0)));
		int rec_x = (int)std::round((double)GetScreenWidth() * ((double)i / (double)workout->GetWorkoutLength()));

		if (i != 0) {
			DrawLine(rec_xp, rec_yp, rec_x, rec_y, graphProgressLineColor);
		}

		rec_xp = rec_x;
		rec_yp = rec_y;
	}

	DrawLine(x_pos, dataDisplayHeight, x_pos, dataDisplayHeight + graphAreaHeight, graphProgressLineColor);

	// Graph Time axis
	int timeAxisY = dataDisplayHeight + graphAreaHeight;
	DrawLine(0, timeAxisY, GetScreenWidth(), timeAxisY, graphScaleLines);
	double secondDist = (double)GetScreenWidth() / (double)workout->GetWorkoutLength();

	int workoutLength = workout->GetWorkoutLength();
	for (int i = 0; i < workoutLength; i += 60) {
		int timeAxisX = secondDist * i;
		if (i % 300 == 0) {
			
			DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + graphTimeAxisHeight, graphScaleLines);
			std::string timeStr = MattsUtils::Time::ToString(i);

			if (GetScreenWidth() - timeAxisX > MeasureTextEx(fontType, timeStr.c_str(), 16, 1.0).x + 15) {
				RelativeDrawing::DrawTextRelEx(
					fontType,
					timeStr.c_str(),
					raylib::ConstructVector2(timeAxisX + 3, timeAxisY + 3),
					RelativeDrawing::TopLeft,
					RelativeDrawing::TopLeft,
					16,
					1.0,
					graphScaleLines
				);
			}
			
		}
		else {
			DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 4, graphScaleLines);
		}
	}

	// Graph Stuff end

	bool pauseRes = RelativeDrawing::GuiButtonRelative((!paused ? "Pause" : "Play"), raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (pauseRes) {
		paused = !paused;
	}

	bool finishRes = RelativeDrawing::GuiButtonRelative("Finish", raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);
	if (finishRes) {
		started = false;
		SceneManager::LoadScene("WorkoutSelectionMenu");
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
			WHITE
		);

		bool startRes = RelativeDrawing::GuiButtonRelative("Start", raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::Center, RelativeDrawing::Center, 24);
		if (startRes) {
			started = true;
			paused = true;
			workoutTime = 0;

			powerRecord.clear();
			heartRateRecord.clear();
			cadenceRecord.clear();

			currentPower = -1;
			currentHeartRate = -1;
			currentCadence = -1;

			BluetoothController::SubscribeToCyclingPower(&currentPower);
			BluetoothController::SubscribeToHeartRate(&currentHeartRate);
			
			
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
	Bounds bounds = RelativeDrawing::DrawTextRelEx(
		font,
		value.c_str(),
		raylib::ConstructVector2(position.x, position.y + 16),
		RelativeDrawing::TopCenter,
		RelativeDrawing::TopCenter,
		valueFontSize,
		1,
		valueColor
	);

	return CheckCollisionPointRec(GetMousePosition(), raylib::ConstructRectangle(bounds.minX, bounds.minY, bounds.maxX - bounds.minX, bounds.maxY - bounds.minY)) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
