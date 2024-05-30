#pragma once
#include "bounds.hpp"
#include "raylib.h"

#define PIXEL_COORD_ROUND_TO_CEIL true

namespace MattsUtils::RelativeDrawing {
    enum PinLocation {
        TopLeft,
        TopCenter,
        TopRight,
        MiddleLeft,
        Center,
        MiddleRight,
        BottomLeft,
        BottomCenter,
        BottomRight
    };

    void DrawTextRelEx(Font font, const char* text, Vector2 position, PinLocation relativeToWindow, PinLocation relativeToText, float fontSize, float spacing, Color tint);

    /**
     * This is an extension of the raygui GuiButton.
     * @returns Returns true when clicked.
    */
    int GuiButtonRelative(char* text, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize);

    int GuiDropdownBoxRelative(char* text, int* active, bool editMode, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize);

    int GuiCheckBoxRelative(const char* text, bool* checked, Vector2 position, Vector2 checkBoxDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize);

    MattsUtils::Bounds DrawRectangle(Vector2 position, Vector2 dimensions, PinLocation relativeToWindow, PinLocation relativeToRectangle, Color color, bool onlyBounds = false);
    MattsUtils::Bounds DrawRectangleRot(Vector2 position, Vector2 dimensions, float rotation, PinLocation relativeToWindow, PinLocation relativeToRectangle, Color color, bool onlyBounds = false);

    MattsUtils::Bounds DrawLineExRel(Vector2 start, Vector2 end, float thick, PinLocation relativeToWindow, Color color, bool onlyBounds = false);

    MattsUtils::Bounds DrawCircleRel(Vector2 center, float radius, PinLocation relativeToWindow, Color color, bool onlyBounds = false);

    MattsUtils::Bounds DrawTriangleRel(Vector2 v1, Vector2 v2, Vector2 v3, PinLocation relativeToWindow, Color color, bool onlyBounds = false);

    /**
     * Returns an offset defined by the offsetVal and offsetFrom parameters.
     * @param currentPos The desired location to be offset from using the offsetVal and offsetFrom.
     * @param offsetVal Used in conjunction with the offsetFrom to correctly offset the currentPos.
     * @param offsetFrom Where to offset from.
     * @returns Returns the pos correctly offset using the offsetVal and offsetFrom parameters.
    */
    Vector2 OffsetPosByPinLocation(Vector2 currentPos, Vector2 offsetVal, PinLocation offsetFrom, bool negativeOffset);
}