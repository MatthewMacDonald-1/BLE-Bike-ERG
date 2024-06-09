#pragma once

#include <string>
#include <vector>
#include <utility>
#include "raylib.h"

class WorkoutDefinition {
public:

	enum TargetType {
		RAW_POWER,
		FTP
	};

	WorkoutDefinition(std::string fileName);

	WorkoutDefinition(std::vector<std::string> fromFile);

	/// <summary>
	/// Load the workouts from a directory with a manifest file.
	/// </summary>
	/// <param name="dir">Directory containing the Manifest and workout files.</param>
	/// <returns></returns>
	static std::vector<WorkoutDefinition*> LoadFromDirectory(std::string dir);

	/// <summary>
	/// Wheather the workout is valid.
	/// </summary>
	/// <returns></returns>
	bool IsValid();

	/// <summary>
	/// Workout name.
	/// </summary>
	/// <returns></returns>
	std::string GetName();

	/// <summary>
	/// Length of workout in seconds.
	/// </summary>
	/// <returns></returns>
	int GetWorkoutLength();

	/// <summary>
	/// The target type of the workout. Used to interprete the evaluate workout function.
	/// </summary>
	/// <returns></returns>
	TargetType GetTargetType();

	std::string GetTargetTypeStr();

	/// <summary>
	/// The target power at specified time.
	/// </summary>
	/// <param name="time"></param>
	/// <returns>Tagret value (not interpreted)</returns>
	double EvaluateWorkoutAt(int time);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position">Top Left Corner</param>
	/// <param name="width">Graph container width</param>
	/// <param name="height">Graph container height</param>
	void DrawWorkout(Vector2 position, int width, int height, int ftp);

private:
	bool isValid = false;

	std::string name;

	TargetType workoutType;

	std::vector<std::pair<int, std::pair<int, int>>> segments; // Time, Start Power, End Power

	void ReadWorkout(std::vector<std::string> fromFile);
};