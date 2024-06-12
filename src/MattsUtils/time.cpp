#include "time.hpp"
#include "raylib.h"
#include <cmath>

std::string MattsUtils::Time::ToString(double time)
{
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

	std::string strTime = hour == 0 ? 
		TextFormat("%s%d:%s%d", (min < 10 ? "" : ""), min, (sec < 10 ? "0" : ""), sec) : 
		TextFormat("%s%d:%s%d:%s%d", (hour < 10 ? "" : ""), hour, (min < 10 ? "0" : ""), min, (sec < 10 ? "0" : ""), sec);

	return strTime;
}
