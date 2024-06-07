#include "workout-definition.hpp"

WorkoutDefinition::WorkoutDefinition(std::vector<std::string> fromFile)
{
	name = "";
}

bool WorkoutDefinition::IsValid()
{
	return isValid;
}

std::string WorkoutDefinition::GetName()
{
	return name;
}
