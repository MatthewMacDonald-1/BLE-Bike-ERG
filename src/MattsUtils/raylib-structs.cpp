#include "raylib-structs.hpp"

Vector2 MattsUtils::raylib::ConstructVector2(float x, float y)
{
    Vector2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

Color MattsUtils::raylib::ConstructColor(unsigned char r, unsigned char g, unsigned char b)
{
    return MattsUtils::raylib::ConstructColor(r, g, b, 255);
}

Color MattsUtils::raylib::ConstructColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    Color col;
    col.r = r;
    col.g = g;
    col.b = b;
    col.a = a;
    return col;
}

Rectangle MattsUtils::raylib::ConstructRectangle(float x, float y, float width, float height)
{
    Rectangle rec;
    rec.x = x;
    rec.y = y;
    rec.width = width;
    rec.height = height;
    return rec;
}
