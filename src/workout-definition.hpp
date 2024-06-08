#pragma once

#include <string>
#include <vector>
#include <utility>

class WorkoutDefinition {
public:

	enum TargetType {
		RAW_POWER,
		FTP
	};

	WorkoutDefinition(std::vector<std::string> fromFile);

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

	/// <summary>
	/// The target power at specified time.
	/// </summary>
	/// <param name="time"></param>
	/// <returns>Tagret value (not interpreted)</returns>
	double EvaluateWorkoutAt(int time);

private:
	bool isValid = false;

	std::string name;

	TargetType workoutType;

	std::vector<std::pair<int, std::pair<int, int>>> segments; // Time, Start Power, End Power
};