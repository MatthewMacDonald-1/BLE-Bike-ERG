#pragma once
#include "raylib.h"

namespace MattsUtils::raylib {

	Vector2 ConstructVector2(float x, float y);

	Color ConstructColor(unsigned char r, unsigned char g, unsigned char b);

	Color ConstructColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	Rectangle ConstructRectangle(float x, float y, float width, float height);

}