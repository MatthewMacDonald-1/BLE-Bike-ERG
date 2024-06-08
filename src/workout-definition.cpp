#include "workout-definition.hpp"

#include "MattsUtils/string-func.hpp"
#include "MattsUtils/number.hpp"
#include "raylib.h"
#include <set>

WorkoutDefinition::WorkoutDefinition(std::vector<std::string> fromFile)
{
	name = "Default Name";
	if (fromFile.size() < 3) {
		isValid = false;
		TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Must have a title, header line, and at least one workout segment.");
		return;
	}
	else {
		isValid = true;
	}

	std::set<std::string> headers;
	std::vector<std::string> columnType;

	std::string temp;
	for (int i = 0; i < fromFile.size(); i++) {
		temp = MattsUtils::String::trim(fromFile.at(i));

		if (i == 0) {
			name = temp;
		} 
		else if (i == 1) {
			workoutType = RAW_POWER;
			std::vector<std::string> rawHeaders;
			MattsUtils::String::split(temp, rawHeaders, ' ');

			for (int j = 0; j < rawHeaders.size(); j++) {
				std::string headerValue = MattsUtils::String::trim(rawHeaders.at(j));
				std::pair<std::set<std::string>::iterator, bool> res = headers.insert(headerValue);
				if (!res.second) {
					isValid = false;
					TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line 2. Duplicate header values.");
					return;
				}
				else {
					columnType.push_back(headerValue);
					if (headerValue == "target_ftp") {
						workoutType = FTP;
						if (headers.find("target_power") != headers.end()) { // Contains a target_power header
							isValid = false;
							TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line 2. Contains target_ftp and target_power columns.");
							return;
						}
					}
					else if (headerValue == "target_power") {
						workoutType = RAW_POWER;
						if (headers.find("target_ftp") != headers.end()) { // Contains a target_ftp header
							isValid = false;
							TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line 2. Contains target_ftp and target_power columns.");
							return;
						}
					}

				}
			}

			if (columnType.size() < 2) {
				isValid = false;
				TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line 2. Must have at least two header values.");
				return;
			}

			if (headers.find("time_sec") == headers.end()) {
				isValid = false;
				TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line 2. Must have a time_sec header.");
				return;
			}
		}
		else {
			std::vector<std::string> columns;
			MattsUtils::String::split(temp, columns, ',');

			if (columns.size() < columnType.size()) {
				isValid = false;
				TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line: %d. Less columns then defined in header (Line 2).", (i+1));
				return;
			}
			else if (columns.size() > columnType.size()) {
				isValid = false;
				TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line: %d. More columns then defined in header (Line 2).", (i + 1));
				return;
			}
			else {
				// Read in the values
				std::pair<int, std::pair<int, int>> segment;

				for (int j = 0; j < columns.size(); j++) {
					if (columnType.at(j) == "time_sec") {
						segment.first = MattsUtils::Number::parseInt(MattsUtils::String::trim(columns.at(j)));
					}
					else if (columnType.at(j) == "target_ftp" || columnType.at(j) == "target_power") {
						
						if (MattsUtils::String::containsChar(columns.at(j), '_')) {
							std::vector<std::string> rampValues;
							MattsUtils::String::split(columns.at(j), rampValues, '_');

							if (rampValues.size() < 2) {
								isValid = false;
								TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line: %d. Less then two values defined for a slope.", (i + 1));
								return;
							}
							else if (rampValues.size() > 2) {
								isValid = false;
								TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line: %d. More then two values defined for a slope.", (i + 1));
								return;
							}

							int value1 = MattsUtils::Number::parseInt(MattsUtils::String::trim(rampValues.at(0)));
							int value2 = MattsUtils::Number::parseInt(MattsUtils::String::trim(rampValues.at(1)));
							segment.second = std::pair(value1, value2);
						}
						else {
							int value = MattsUtils::Number::parseInt(MattsUtils::String::trim(columns.at(j)));
							segment.second = std::pair(value, value);
						}
					}
				}

				segments.push_back(segment);
			}

		}
	}


}

bool WorkoutDefinition::IsValid()
{
	return isValid;
}

std::string WorkoutDefinition::GetName()
{
	return name;
}

int WorkoutDefinition::GetWorkoutLength()
{
	int sum = 0;

	for (int i = 0; i < segments.size(); i++) {
		sum += segments.at(i).first;
	}

	return sum;
}

WorkoutDefinition::TargetType WorkoutDefinition::GetTargetType()
{
	return workoutType;
}

double WorkoutDefinition::EvaluateWorkoutAt(int time)
{
	return 0.0;
}
