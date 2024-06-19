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
#include "user-data.hpp"

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

	if (!started) {
		// Reset the records so that if this workout is opened a second time it will apear fresh and ready.
		finished = false;
		paused = true;

		workoutTime = 0;
		startCountDown = 5;
		timeMode = 0;

		powerRecord.clear();
		heartRateRecord.clear();
		cadenceRecord.clear();

		currentPower = -1;
		currentHeartRate = -1;
		currentCadence = -1;
	}

	if (!started || startCountDown > 0) GuiLock();

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

	if (finished) {
		return DrawWorkoutOverScreen(
			fontType,
			buttonSize,
			dataDisplayBackground,
			graphAreaBackground,
			graphScaleLines,
			graphScaleLineFTP100,
			graphHeartRateRecordLineColor,
			graphCadenceRecordLineColor,
			graphProgressLineColor,
			graphProgressCompletedColor
		);
	}
	
	int dataDisplayHeight = 128;
	int graphTimeAxisHeight = 20;
	int bottomControlBarHeight = 32;
	int graphAreaHeight = GetScreenHeight() - dataDisplayHeight - graphTimeAxisHeight - bottomControlBarHeight;

	int actualFTP = UserData::GetFTP();
	int ftp = workout->GetTargetType() == WorkoutDefinition::RAW_POWER ? 100 : actualFTP;

	if ((int)workoutTime != previousFrameIntTime) {
		powerRecord.push_back(currentPower);
		heartRateRecord.push_back(currentHeartRate);
		cadenceRecord.push_back(currentCadence);

		previousFrameIntTime = (int)workoutTime;
	}

	int targetPower = (int)std::round(((double)workout->EvaluateWorkoutAt((int)workoutTime) / 100.0) * (double)ftp);

	DrawWorkoutGraph(
		raylib::ConstructRectangle(0, 0 + dataDisplayHeight, GetScreenWidth(), graphAreaHeight),
		fontType,
		graphAreaBackground,
		graphScaleLines,
		graphScaleLineFTP100,
		graphProgressCompletedColor,
		graphHeartRateRecordLineColor,
		graphCadenceRecordLineColor,
		graphProgressLineColor,
		graphProgressLineColor,
		ftp
	);

	DrawWorkoutTimeAxis(
		raylib::ConstructRectangle(0, 0 + dataDisplayHeight + graphAreaHeight, GetScreenWidth(), graphTimeAxisHeight),
		fontType,
		graphScaleLines,
		graphAreaBackground,
		workout->GetWorkoutLength()
	);

	DrawRectangle(0, GetScreenHeight() - bottomControlBarHeight, GetScreenWidth(), bottomControlBarHeight, dataDisplayBackground);

	DrawWorkoutDataReadout(
		raylib::ConstructRectangle(0, 0, GetScreenWidth(), dataDisplayHeight),
		fontType,
		dataDisplayBackground,
		targetPower
	);

	bool pauseRes = RelativeDrawing::GuiButtonRelative((!paused ? "Pause" : "Play"), raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (pauseRes) {
		paused = !paused;
	}

	bool finishRes = RelativeDrawing::GuiButtonRelative("Finish", raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);
	if (finishRes) {
		/*started = false;
		SceneManager::LoadScene("WorkoutSelectionMenu");*/
		finished = true;
	}

	GuiUnlock();

	if (started && startCountDown > 0) {
		DrawRectangle(0, dataDisplayHeight, GetScreenWidth(), graphAreaHeight, raylib::ConstructColor(0, 0, 0, 100));

		RelativeDrawing::DrawTextRelEx(
			fontType,
			TextFormat("%d", (int)std::ceil(startCountDown)),
			raylib::ConstructVector2(0, dataDisplayHeight + graphAreaHeight / 2),
			RelativeDrawing::TopCenter,
			RelativeDrawing::Center,
			64,
			1.5,
			WHITE
		);

		startCountDown -= GetFrameTime();
		if (startCountDown <= 0) {
			paused = false;
		}
	}

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
			finished = false;
			workoutTime = 0;
			startCountDown = 5;
			timeMode = 0;

			powerRecord.clear();
			heartRateRecord.clear();
			cadenceRecord.clear();

			currentPower = -1;
			currentHeartRate = -1;
			currentCadence = -1;

			BluetoothController::SubscribeToCyclingPower(&currentPower);
			BluetoothController::SubscribeToHeartRate(&currentHeartRate);
			BluetoothController::SubscribeToCadence(&currentCadence);
			
			
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

void WorkoutScene::DrawWorkoutDataReadout(Rectangle dataRect, Font fontType, Color backgroundColor, int targetPower)
{
	using namespace MattsUtils;

	DrawRectangle(dataRect.x, dataRect.y, GetScreenWidth(), dataRect.height, backgroundColor);

	// Data

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
}

void WorkoutScene::DrawWorkoutGraph(Rectangle graphRect, Font fontType, Color backgroundColor, Color graphScaleLines, Color graphScaleLineFTP100, Color graphProgressCompletedAreaColor, Color graphHeartRateRecordLineColor, Color graphCadenceRecordLineColor, Color graphPowerRecordLineColor, Color graphProgressLineColor, int ftp)
{
	using namespace MattsUtils;

	DrawRectangle(graphRect.x, graphRect.y, graphRect.width, graphRect.height, backgroundColor);

	// Graph Stuff Start

	int highestTarget = (int)(((double)workout->GetHighestTarget() / 100.0) * (double)ftp);
	int highestGraphPoint = (int)std::ceil((double)highestTarget / 100.0) * 100;

	int workoutDrawHeight = (int)(((double)highestTarget / (double)highestGraphPoint) * (double)graphRect.height);

	double oneWattDist = (double)graphRect.height / (double)highestGraphPoint;

	for (int i = 100; i < highestGraphPoint; i += 100) {
		int y = graphRect.y + (graphRect.height - (oneWattDist * i));
		DrawLine(graphRect.x, y, graphRect.width, y, graphScaleLines);
		RelativeDrawing::DrawTextRelEx(fontType, TextFormat("%d", i),
			raylib::ConstructVector2(3, y - 1),
			RelativeDrawing::TopLeft,
			RelativeDrawing::BottomLeft,
			16,
			1.0,
			graphScaleLines
		);
	}

	workout->DrawWorkout(raylib::ConstructVector2(graphRect.x, graphRect.y + (graphRect.height - workoutDrawHeight)), graphRect.width, workoutDrawHeight, raylib::ConstructColor(0, 178, 255, 255), 0);

	int ftp_y = graphRect.y + (graphRect.height - (oneWattDist * UserData::GetFTP()));
	DrawLine(graphRect.x, ftp_y, graphRect.width, ftp_y, graphScaleLineFTP100);

	RelativeDrawing::DrawTextRelEx(fontType, TextFormat("FTP %d", UserData::GetFTP()),
		raylib::ConstructVector2(-3, ftp_y - 1),
		RelativeDrawing::TopRight,
		RelativeDrawing::BottomRight,
		16,
		1.0,
		graphScaleLineFTP100
	);

	// Draw Time line
	int x_pos = (int)std::round((double)graphRect.width * ((double)(int)workoutTime / (double)workout->GetWorkoutLength()));

	DrawRectangle(graphRect.x, graphRect.y, x_pos, graphRect.height, graphProgressCompletedAreaColor);

	Rectangle graphSizedata = raylib::ConstructRectangle(graphRect.x, graphRect.y, graphRect.width, graphRect.height);

	// Draw Power Record
	GraphDrawDataLine(cadenceRecord, currentCadence, (int)workoutTime, workout->GetWorkoutLength(), oneWattDist, graphCadenceRecordLineColor, graphSizedata);

	GraphDrawDataLine(heartRateRecord, currentHeartRate, (int)workoutTime, workout->GetWorkoutLength(), oneWattDist, graphHeartRateRecordLineColor, graphSizedata);

	GraphDrawDataLine(powerRecord, currentPower, (int)workoutTime, workout->GetWorkoutLength(), oneWattDist, graphPowerRecordLineColor, graphSizedata);

	DrawLine(x_pos, graphRect.y, x_pos, graphRect.y + graphRect.height, graphProgressLineColor);

	// Graph Stuff end
}

void WorkoutScene::DrawWorkoutTimeAxis(Rectangle axisRect, Font fontType, Color lineColor, Color backgroundColor, int timePeriod)
{
	using namespace MattsUtils;

	DrawRectangle(0, axisRect.y, GetScreenWidth(), axisRect.height, backgroundColor);

	// Graph Time axis
	int timeAxisY = axisRect.y;

	DrawLine(0, timeAxisY, GetScreenWidth(), timeAxisY, lineColor);
	double secondDist = (double)GetScreenWidth() / (double)timePeriod;

	int workoutLength = timePeriod;

	int minuteDist = secondDist * 60;
	int halfMinuteDist = secondDist * 30;
	int tenSecDist = secondDist * 10;
	int minMinuteDist = 100;

	//int increment = !drawMoreMiuntes ? 60 : 30;
	int increment = 1;

	for (int i = 0; i < workoutLength; i += increment) {
		int timeAxisX = secondDist * i;

		if (tenSecDist >= minMinuteDist) {
			if (i % 10 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + axisRect.height, lineColor);
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
						lineColor
					);
				}
			}
			else if (i % 5 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 8, lineColor);
			}
			else if (i % 1 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 4, lineColor);
			}
		}
		else if (halfMinuteDist >= minMinuteDist) {
			if (i % 30 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + axisRect.height, lineColor);
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
						lineColor
					);
				}
			}
			else if (i % 10 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 8, lineColor);
			}
			else if (i % 5 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 4, lineColor);
			}
		}
		else if (minuteDist >= minMinuteDist) {
			if (i % 60 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + axisRect.height, lineColor);
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
						lineColor
					);
				}
			}
			else if (i % 30 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 8, lineColor);
			}
			else if (i % 10 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 4, lineColor);
			}
			else if (i % 5 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 2, lineColor);
			}
		}
		else {
			if (i % 300 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + axisRect.height, lineColor);
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
						lineColor
					);
				}
			}
			else if (i % 60 == 0) {
				DrawLine(timeAxisX, timeAxisY, timeAxisX, timeAxisY + 4, lineColor);
			}
		}
	}
}

void WorkoutScene::GraphDrawDataLine(std::vector<int> dataValues, int currentValue, int currentTime, int totalTime, double yScale, Color lineColor, Rectangle graph)
{
	int x_pos = (int)std::round((double)graph.width * ((double)currentTime / (double)totalTime));
	int rec_xp = 0;
	int rec_yp = 0;
	for (int i = 0; i < dataValues.size(); i++) {
		int rec_y = graph.y + (graph.height - (yScale * std::max(dataValues.at(i), 0)));
		int rec_x = graph.x + (int)std::round((double)graph.width * ((double)i / (double)totalTime));

		if (i != 0) {
			DrawLine(rec_xp, rec_yp, rec_x, rec_y, lineColor);
		}

		rec_xp = rec_x;
		rec_yp = rec_y;
	}
	if (x_pos > 0) DrawLine(rec_xp, rec_yp, x_pos, graph.y + (graph.height - (yScale * std::max(currentValue, 0))), lineColor);
}

int WorkoutScene::DrawWorkoutOverScreen(Font fontType, Vector2 buttonSize, Color dataDisplayBackground, Color graphAreaBackground, Color graphScaleLines, Color graphScaleLineFTP100, Color graphHeartRateRecordLineColor, Color graphCadenceRecordLineColor, Color graphProgressLineColor, Color graphProgressCompletedColor)
{
	using namespace MattsUtils;

	int actualFTP = UserData::GetFTP();
	int ftp = workout->GetTargetType() == WorkoutDefinition::RAW_POWER ? 100 : actualFTP;

	int bottomControlBarHeight = 32;
	int timeAxisHeight = 20;
	int graphStartY = GetScreenHeight() / 2 - bottomControlBarHeight - timeAxisHeight;
	
	DrawWorkoutGraph(
		raylib::ConstructRectangle(0, graphStartY, GetScreenWidth(), GetScreenHeight() / 2),
		fontType,
		graphAreaBackground,
		graphScaleLines,
		graphScaleLineFTP100,
		raylib::ConstructColor(0, 0, 0, 0),
		graphHeartRateRecordLineColor,
		graphCadenceRecordLineColor,
		graphProgressLineColor,
		raylib::ConstructColor(0, 0, 0, 0),
		ftp
	);

	DrawWorkoutTimeAxis(
		raylib::ConstructRectangle(0, 0 + GetScreenHeight() - bottomControlBarHeight - timeAxisHeight, GetScreenWidth(), timeAxisHeight),
		fontType,
		graphScaleLines,
		graphAreaBackground,
		(int)workoutTime
	);

	// Summary Start

	DrawRectangle(0, 0, GetScreenWidth(), graphStartY, dataDisplayBackground);

	RelativeDrawing::DrawTextRelEx(
		fontType,
		"Summary",
		raylib::ConstructVector2(0, 0),
		RelativeDrawing::TopCenter,
		RelativeDrawing::TopCenter,
		48,
		1.0,
		WHITE
	);

	int avgPower = -1;
	DrawDataValue(
		fontType,
		"Avg Power",
		std::string(TextFormat("%s", (avgPower == -1 ? "--" : TextFormat("%d", avgPower)))),
		raylib::ConstructVector2(-GetScreenWidth() / 4, 10)
	);

	int maxPower = -1;
	DrawDataValue(
		fontType,
		"Max Power",
		std::string(TextFormat("%s", (maxPower == -1 ? "--" : TextFormat("%d", maxPower)))),
		raylib::ConstructVector2(-GetScreenWidth() / 4, 70)
	);

	int avgCadence = -1;
	DrawDataValue(
		fontType,
		"Cadence",
		std::string(TextFormat("%s", (avgCadence == -1 ? "--" : TextFormat("%d", avgCadence)))),
		raylib::ConstructVector2(GetScreenWidth() / 4, 10)
	);

	int avgHeartRate = -1;
	DrawDataValue(
		fontType,
		"Heart Rate",
		std::string(TextFormat("%s", (avgHeartRate == -1 ? "--" : TextFormat("%d", avgHeartRate)))),
		raylib::ConstructVector2(GetScreenWidth() / 4, 70)
	);

	// Summary End

	DrawRectangle(0, GetScreenHeight() - bottomControlBarHeight, GetScreenWidth(), bottomControlBarHeight, dataDisplayBackground);


	bool continueRes = RelativeDrawing::GuiButtonRelative("Continue", raylib::ConstructVector2(0, 0), buttonSize, RelativeDrawing::BottomRight, RelativeDrawing::BottomRight, 24);
	if (continueRes) {
		started = false;
		SceneManager::LoadScene("WorkoutSelectionMenu");
	}

	return EXIT_SUCCESS;
}
