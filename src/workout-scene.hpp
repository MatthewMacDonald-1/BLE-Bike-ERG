#pragma once

#include "scene.hpp"
#include "workout-definition.hpp"

#include "raylib.h"
#include <simpleble/SimpleBLE.h>
#include <string>

class WorkoutScene : public Scene {
public:
	WorkoutScene(WorkoutDefinition* workoutSrc);
	~WorkoutScene();

	int UpdateLogic();
	int DrawCall();

private:
	WorkoutDefinition* workout;

	bool started = false;
	float startCountDown = 5;

	bool paused = true;

	int timeMode = 0;
	int maxTimeMode = 2;

	float workoutTime = 0;

	int currentPower = -1;
	int currentHeartRate = -1;
	int currentCadence = -1;

	int previousFrameIntTime = -1;

	std::vector<int> powerRecord;
	std::vector<int> heartRateRecord;
	std::vector<int> cadenceRecord;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="font"></param>
	/// <param name="heading"></param>
	/// <param name="value"></param>
	/// <param name="position"></param>
	/// <returns>Value clicked</returns>
	bool DrawDataValue(Font font, std::string heading, std::string value, Vector2 position);

	void GraphDrawDataLine(std::vector<int> dataValues, int currentValue, int currentTime, int totalTime, double yScale, Color lineColor, Rectangle graph);
};