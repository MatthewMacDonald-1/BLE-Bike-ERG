#pragma once

#include <string>
#include <vector>

class WorkoutDefinition {
public:

	WorkoutDefinition(std::vector<std::string> fromFile);

	bool IsValid();

	std::string GetName();

private:
	bool isValid = false;
	std::string name;
};