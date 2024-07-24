#include "bounds.hpp"

using namespace MattsUtils;

Bounds::Bounds()
{
	this->minX = 0;
	this->maxX = 0;
	this->minY = 0;
	this->maxY = 0;
}

Bounds::Bounds(float minX, float maxX, float minY, float maxY)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
}

bool MattsUtils::Bounds::InBounds(int x, int y)
{
	return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

std::string Bounds::ToString()
{
	std::string output = "Min(" + std::to_string(this->minX) + "," + std::to_string(this->minY) + ") Max(" + std::to_string(this->maxX) + "," + std::to_string(this->maxY) + ")";
	return output;
}
