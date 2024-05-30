#include "relative-drawing.hpp"
#include "raylib-structs.hpp"
#include <iostream>

// For using raygui
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_STATIC
#include "raygui.h"

using namespace MattsUtils::raylib;

void MattsUtils::RelativeDrawing::DrawTextRelEx(Font font, const char* text, Vector2 position, PinLocation relativeToWindow, PinLocation relativeToText, float fontSize, float spacing, Color tint) {
    Vector2 textRenderSize = MeasureTextEx(font, text, fontSize, spacing);

    Vector2 pos = { 0, 0 };
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    pos = OffsetPosByPinLocation(position, ConstructVector2((float)width, (float)height), relativeToWindow, false);
    pos = OffsetPosByPinLocation(pos, textRenderSize, relativeToText, true);

    DrawTextEx(font, text, pos, fontSize, spacing, tint);
}

int MattsUtils::RelativeDrawing::GuiButtonRelative(char* text, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize)
{
    position = OffsetPosByPinLocation(position, ConstructVector2(GetScreenWidth(), GetScreenHeight()), relativeToWindow, false);
    position = OffsetPosByPinLocation(position, buttonDimensions, relativeToText, true);
    Rectangle bounds = ConstructRectangle(position.x, position.y, buttonDimensions.x, buttonDimensions.y);
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    return GuiButton(bounds, text);
}

int MattsUtils::RelativeDrawing::GuiDropdownBoxRelative(char* text, int* active, bool editMode, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize)
{
    position = OffsetPosByPinLocation(position, ConstructVector2(GetScreenWidth(), GetScreenHeight()), relativeToWindow, false);
    position = OffsetPosByPinLocation(position, buttonDimensions, relativeToText, true);
    Rectangle bounds = ConstructRectangle(position.x, position.y, buttonDimensions.x, buttonDimensions.y);
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    return GuiDropdownBox(bounds, text, active, editMode);
}

int MattsUtils::RelativeDrawing::GuiCheckBoxRelative(const char* text, bool* checked, Vector2 position, Vector2 checkBoxDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize)
{
    position = OffsetPosByPinLocation(position, ConstructVector2(GetScreenWidth(), GetScreenHeight()), relativeToWindow, false);
    position = OffsetPosByPinLocation(position, checkBoxDimensions, relativeToText, true);
    Rectangle bounds = ConstructRectangle(position.x, position.y, checkBoxDimensions.x, checkBoxDimensions.y );
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    return GuiCheckBox(bounds, text, checked);
}

MattsUtils::Bounds MattsUtils::RelativeDrawing::DrawRectangle(Vector2 position, Vector2 dimensions, PinLocation relativeToWindow, PinLocation relativeToRectangle, Color color, bool onlyBounds)
{
    position = OffsetPosByPinLocation(position, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);
    position = OffsetPosByPinLocation(position, dimensions, relativeToRectangle, true);
    if (!onlyBounds) {
        if (PIXEL_COORD_ROUND_TO_CEIL) {
            position.x = std::ceil(position.x);
            position.y = std::ceil(position.y);

            dimensions.x = std::ceil(dimensions.x);
            dimensions.y = std::ceil(dimensions.y);
        }
        DrawRectangle(position.x, position.y, dimensions.x, dimensions.y, color);
    }

    return MattsUtils::Bounds(position.x, position.x + dimensions.x, position.y, position.y + dimensions.y);
}

MattsUtils::Bounds MattsUtils::RelativeDrawing::DrawRectangleRot(Vector2 position, Vector2 dimensions, float rotation, PinLocation relativeToWindow, PinLocation relativeToRectangle, Color color, bool onlyBounds)
{
    position = OffsetPosByPinLocation(position, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);
    position = OffsetPosByPinLocation(position, dimensions, relativeToRectangle, true);
    if (!onlyBounds) {
        if (PIXEL_COORD_ROUND_TO_CEIL) {
            position.x = std::ceil(position.x);
            position.y = std::ceil(position.y);

            dimensions.x = std::ceil(dimensions.x);
            dimensions.y = std::ceil(dimensions.y);
        }

        //DrawRectangle(position.x, position.y, dimensions.x, dimensions.y, color)
        using namespace MattsUtils::raylib;
        DrawRectanglePro(ConstructRectangle(position.x + dimensions.x / 2, position.y + dimensions.y / 2, dimensions.x, dimensions.y), ConstructVector2(dimensions.x/2, dimensions.y/2), rotation, color);
    }

    return MattsUtils::Bounds(position.x, position.x + dimensions.x, position.y, position.y + dimensions.y);
}

MattsUtils::Bounds MattsUtils::RelativeDrawing::DrawLineExRel(Vector2 start, Vector2 end, float thick, PinLocation relativeToWindow, Color color, bool onlyBounds)
{
    start = OffsetPosByPinLocation(start, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);
    end = OffsetPosByPinLocation(end, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);

    if (!onlyBounds) {
        if (PIXEL_COORD_ROUND_TO_CEIL) {
            start.x = std::ceil(start.x);
            start.y = std::ceil(start.y);

            end.x = std::ceil(end.x);
            end.y = std::ceil(end.y);
        }

        DrawLineEx(start, end, thick, color);
    }

    return MattsUtils::Bounds(
        std::min(start.x, end.x),
        std::max(start.x, end.x),
        std::min(start.y, end.y),
        std::max(start.y, end.y)
    );
}

MattsUtils::Bounds MattsUtils::RelativeDrawing::DrawCircleRel(Vector2 center, float radius, PinLocation relativeToWindow, Color color, bool onlyBounds)
{
    center = OffsetPosByPinLocation(center, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);

    if (!onlyBounds) {
        if (PIXEL_COORD_ROUND_TO_CEIL) {
            center.x = std::ceil(center.x);
            center.y = std::ceil(center.y);
        }

        DrawCircle(center.x, center.y, radius, color);
    }
    return MattsUtils::Bounds(center.x - radius, center.x + radius, center.y - radius, center.y + radius);
}

MattsUtils::Bounds MattsUtils::RelativeDrawing::DrawTriangleRel(Vector2 v1, Vector2 v2, Vector2 v3, PinLocation relativeToWindow, Color color, bool onlyBounds)
{
    v1 = OffsetPosByPinLocation(v1, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);
    v2 = OffsetPosByPinLocation(v2, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);
    v3 = OffsetPosByPinLocation(v3, ConstructVector2((float)GetScreenWidth(), (float)GetScreenHeight()), relativeToWindow, false);


    if (!onlyBounds) {
        if (PIXEL_COORD_ROUND_TO_CEIL) {
            v1.x = std::ceil(v1.x);
            v1.y = std::ceil(v1.y);

            v3.x = std::ceil(v3.x);
            v3.y = std::ceil(v3.y);

            v2.x = std::ceil(v2.x);
            v2.y = std::ceil(v2.y);
        }

        DrawTriangle(v1, v3, v2, color);
    }
    return MattsUtils::Bounds(
        std::min(std::min(v1.x, v2.x), v3.x),
        std::max(std::max(v1.x, v2.x), v3.x),
        std::min(std::min(v1.y, v2.y), v3.y),
        std::max(std::max(v1.y, v2.y), v3.y)
    );
}


/**
 * Returns an offset defined by the offsetVal and offsetFrom parameters.
 * @param currentPos The desired location to be offset from using the offsetVal and offsetFrom.
 * @param offsetVal Used in conjunction with the offsetFrom to correctly offset the currentPos.
 * @param offsetFrom Where to offset from.
 * @returns Returns the pos correctly offset using the offsetVal and offsetFrom parameters.
*/
Vector2 MattsUtils::RelativeDrawing::OffsetPosByPinLocation(Vector2 currentPos, Vector2 offsetVal, PinLocation offsetFrom, bool negativeOffset) {
    if (negativeOffset) {
        offsetVal.x = -1 * offsetVal.x;
        offsetVal.y = -1 * offsetVal.y;
    }
    switch (offsetFrom)
    {
    case 0: // TopLeft
        // Do nothing. Default operation is to draw from left
        break;

    case 1: // TopCenter
        currentPos.x = (offsetVal.x / 2) + currentPos.x;
        currentPos.y = 0 + currentPos.y;
        break;

    case 2: // TopRight
        currentPos.x = offsetVal.x + currentPos.x;
        currentPos.y = 0 + currentPos.y;
        break;

    case 3: // MiddleLeft
        currentPos.x = 0 + currentPos.x;
        currentPos.y = (offsetVal.y / 2) + currentPos.y;
        break;

    case 5: // MiddleRight
        currentPos.x = offsetVal.x + currentPos.x;
        currentPos.y = (offsetVal.y / 2) + currentPos.y;
        break;

    case 6: // BottomLeft
        currentPos.x = 0 + currentPos.x;
        currentPos.y = offsetVal.y + currentPos.y;
        break;

    case 7: // BottomCenter
        currentPos.x = (offsetVal.x / 2) + currentPos.x;
        currentPos.y = offsetVal.y + currentPos.y;
        break;

    case 8: // BottomRight
        currentPos.x = offsetVal.x + currentPos.x;
        currentPos.y = offsetVal.y + currentPos.y;
        break;

    default: // center
        currentPos.x = offsetVal.x / 2 + currentPos.x;
        currentPos.y = offsetVal.y / 2 + currentPos.y;
        break;
    }

    return currentPos;
}