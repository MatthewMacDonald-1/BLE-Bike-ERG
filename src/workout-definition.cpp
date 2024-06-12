#include "workout-definition.hpp"

#include "MattsUtils/string-func.hpp"
#include "MattsUtils/number.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include <set>
#include <fstream>

int WorkoutDefinition::idGen = 0;

WorkoutDefinition::WorkoutDefinition(std::string fileName)
{
	id = GetNewId();
	std::vector<std::string> lines;

	std::ifstream file(fileName);
	std::string temp;
	while (std::getline(file, temp)) {
		// Output the text from the file
		lines.push_back(temp);
	}

	file.close();

	ReadWorkout(lines);

}

WorkoutDefinition::WorkoutDefinition(std::vector<std::string> fromFile)
{
	id = GetNewId();
	workoutType = RAW_POWER;
	ReadWorkout(fromFile);
}

std::vector<WorkoutDefinition*> WorkoutDefinition::LoadFromDirectory(std::string dir)
{
	std::vector<WorkoutDefinition*> workoutDefinitions;
	std::vector<std::string> workoutSources;

	std::ifstream manifest(dir + "Manifest.txt");
	std::string temp;
	while (std::getline(manifest, temp)) {
		// Output the text from the file
		temp = MattsUtils::String::trim(temp);
		if (temp.length() > 0) {
			workoutSources.push_back(temp);
		}
	}

	manifest.close();

	for (int i = 0; i < workoutSources.size(); i++) {
		std::string src = "";

		workoutDefinitions.push_back(new WorkoutDefinition(dir + workoutSources.at(i)));
	}

	return workoutDefinitions;
}

bool WorkoutDefinition::IsValid()
{
	return isValid;
}

std::string WorkoutDefinition::GetName()
{
	return name;
}

int WorkoutDefinition::GetId()
{
	return id;
}

int WorkoutDefinition::GetWorkoutLength()
{
	int sum = 0;

	for (int i = 0; i < segments.size(); i++) {
		sum += segments.at(i).first;
	}

	return sum;
}

std::string WorkoutDefinition::GetWorkoutLegthStr()
{
	double time = (double)GetWorkoutLength();
	int sec = 0;
	int min = 0;
	int hour = 0;

	double secT = time;
	double minT = secT / 60.0;
	double hourT = minT / 60.0;

	if (secT >= 1) {
		sec = std::floor(secT);
		if (sec >= 60) {
			sec = sec % 60;
		}
	}
	if (minT >= 1) {
		min = std::floor(minT);
		if (min >= 60) {
			min = min % 60;
		}
	}
	if (hourT >= 1) {
		hour = std::floor(hourT);
	}

	std::string aliveTime = hour == 0 ? TextFormat("%s%d:%s%d", (min < 10 ? "0" : ""), min, (sec < 10 ? "0" : ""), sec) : TextFormat("%s%d:%s%d:%s%d", (hour < 10 ? "0" : ""), hour, (min < 10 ? "0" : ""), min, (sec < 10 ? "0" : ""), sec);

	return aliveTime;
}

WorkoutDefinition::TargetType WorkoutDefinition::GetTargetType()
{
	return workoutType;
}

std::string WorkoutDefinition::GetTargetTypeStr()
{
	return GetTargetType() == FTP ? "FTP" : "Raw Power";
}

double WorkoutDefinition::EvaluateWorkoutAt(int time)
{
	return 0.0;
}

void WorkoutDefinition::DrawWorkout(Vector2 position, int width, int height, Color segmentColor, int topMargin)
{
	/*DrawRectangle(position.x, position.y, width, height, DARKGRAY);

	Color segmentColor = MattsUtils::raylib::ConstructColor(0, 200, 200);*/

	double segmentSizeH = (double)width / (double)GetWorkoutLength();
	double segmentSizeV = (double)(height - topMargin) / (double)GetHighestTarget();

	int currentDrawX = position.x;

	for (int i = 0; i < segments.size(); i++) {
		int segWidth = std::max(std::floor((double)segments.at(i).first * segmentSizeH), 1.0);

		if (i == segments.size() - 1) {
			segWidth = width + position.x - currentDrawX;
		}

		int segHeight1 = std::ceil((double)segments.at(i).second.first * segmentSizeV);
		int segHeight2 = std::ceil((double)segments.at(i).second.second * segmentSizeV);

		if (segHeight1 != segHeight2) { // Sloped
			int smallestHeight = std::min(segHeight1, segHeight2);
			int tallestHeight = std::max(segHeight1, segHeight2);

			DrawRectangle(currentDrawX, position.y + ((height + 0) - smallestHeight), segWidth, smallestHeight, segmentColor);

			Vector2 v1, v2, v3;
			v1 = MattsUtils::raylib::ConstructVector2(currentDrawX, position.y + ((height + 0) - smallestHeight));
			v2 = MattsUtils::raylib::ConstructVector2(currentDrawX + segWidth, position.y + ((height + 0) - smallestHeight));

			v3 = MattsUtils::raylib::ConstructVector2(currentDrawX + (segHeight1 > segHeight2 ? 0 : segWidth), position.y + ((height + 0) - tallestHeight));

			DrawTriangle(v1, v2, v3, segmentColor);
		}
		else {
			DrawRectangle(currentDrawX, position.y + ((height + 0) - segHeight1), segWidth, segHeight1, segmentColor);
		}

		currentDrawX += segWidth;
	}

}

int WorkoutDefinition::GetNewId()
{
	int thisId = idGen;
	idGen++;
	return thisId;
}

void WorkoutDefinition::ReadWorkout(std::vector<std::string> fromFile)
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
			MattsUtils::String::split(temp, rawHeaders, ',');

			for (int j = 0; j < rawHeaders.size(); j++) {
				std::string headerValue = MattsUtils::String::trim(rawHeaders.at(j));
				std::cout << "Header Value: " << headerValue << std::endl;
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
			if (temp.length() == 0) {
				continue;
			}

			std::vector<std::string> columns;
			MattsUtils::String::split(temp, columns, ',');

			if (columns.size() < columnType.size()) {
				isValid = false;
				TraceLog(LOG_ERROR, "Workout Reader: Invalid workout. Line: %d. Less columns then defined in header (Line 2).", (i + 1));
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

int WorkoutDefinition::GetHighestTarget()
{
	int largest = 0;

	for (int i = 0; i < segments.size(); i++) {
		if (segments.at(i).second.first > largest) {
			largest = segments.at(i).second.first;
		}
		if (segments.at(i).second.second > largest) {
			largest = segments.at(i).second.second;
		}
	}

	return largest;
}
