#pragma once
#include <string>

namespace MattsUtils {

	/// <summary>
	/// A helper class to keep track of the bounds of an object.
	/// </summary>
	class Bounds {
	public:
		float minX, minY;
		float maxX, maxY;

		Bounds();
		Bounds(float minX, float maxX, float minY, float maxY);

		bool InBounds(int x, int y);

		std::string ToString();


		bool operator==(Bounds const& ref) { return minX == ref.minX && maxX == ref.maxX && minY == ref.minY && maxY == ref.maxY; }
	};
}