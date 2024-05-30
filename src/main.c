#include "utils/common.h"
#include "stdlib.h"
#include "stdio.h"
#include "bluetooth/ble-manager.h"

#define EXIT_CLOSE_WINDOW 11

#include "pages/main-menu.h"

static int (*logicDrawUpdateFunc)(void);

// For returning the pointer
typedef int (*logicDrawUpdateFuncPtr)(void);
// For calling the function
typedef logicDrawUpdateFuncPtr (*logicDrawUpdateFuncType)(void);

logicDrawUpdateFuncPtr SwitchLogicAndDrawFunction(int (*newLogicDrawUpdateFunc)(void));
static int handleLogicAndDrawFunctionChange = false;

static bool debugEnabled = false;
static bool bluetoothSupported = false;

int DefaultLogicDrawFunc(void);
void DrawDebug(void);
bool GetBluetoothSupported(void) {
    return bluetoothSupported;
}

int main(void) {
    // raylib initialization start

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, "World Engines");
    SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    SetTargetFPS(60); 
    SetExitKey(0); // Disable esc key closing the window
    // raylib initialization end

    if (BleManInitialize() == EXIT_SUCCESS) {
        bluetoothSupported = true;
    }

    // Set Defaults global vars
    logicDrawUpdateFunc = DefaultLogicDrawFunc;
    // End Set Defaults global vars

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (GetKeyPressed() == KEY_F3)
            debugEnabled = !debugEnabled;

        int logicDrawRes = logicDrawUpdateFunc();
        if (handleLogicAndDrawFunctionChange) {
            handleLogicAndDrawFunctionChange = false;
        }

        if (logicDrawRes == WINDOW_CLOSED)
            break;
    }

    BleManDeinitialize();

    CloseWindow();

    return EXIT_SUCCESS;
}



int DefaultLogicDrawFunc(void) {
    
    return MainMenuLogicDrawFunc();
}

/**
 * Updates the draw and logic functions for the app. Useful for switching scenes and menus. Note: after calling this function that the newLogicUpdate function will be called before drawing the scene.
 * @param newLogicDrawUpdateFunc A pointer to the new logic Update and draw function.
 * @returns Returns the address of the replaced logicUpdateDrawFunc.
*/
logicDrawUpdateFuncPtr SwitchLogicAndDrawFunction(int (*newLogicDrawUpdateFunc)(void)) {
    handleLogicAndDrawFunctionChange = true; // Signal that the functions have changed.
    logicDrawUpdateFuncPtr old = logicDrawUpdateFunc; // point at mem location of old
    logicDrawUpdateFunc = newLogicDrawUpdateFunc; // now point the current at the new mem location
    return old;
}


static bool vsyncEnabled = false;
void SetVsyncStatus(bool val) {
    vsyncEnabled = val;
}
bool GetDebugStatus(void) {
    return debugEnabled;
}
void DrawDebug() {
    Color background = { 0, 0, 0, 100 };
    Color textColor = RAYWHITE;
    Font defaultFont = GetFontDefault();

    int fontSize = 16;
    int spacing = 1;

    int startY = 3;
    int leftX = 3;
    int verticalOffset = fontSize + fontSize / 4;

    char *vsyncInfo = vsyncEnabled ? "VSYNC" : " ";
    char *fpsText = TextFormat("FPS %d %s", GetFPS(), vsyncInfo);
    Vector2 fpsTextSize = MeasureTextEx(defaultFont, fpsText, fontSize, spacing);
    Vector2 fpsTextLoc = {.x=leftX,.y=startY + 0 * verticalOffset};
    DrawRectangleRelative(fpsTextLoc, fpsTextSize, TopLeft, TopLeft, background);
    DrawTextRelativeEx(defaultFont, fpsText, fpsTextLoc, TopLeft, TopLeft, fontSize, spacing, textColor);

    char *frameTimeText = TextFormat("Frame Time %.3f", GetFrameTime(), vsyncInfo);
    Vector2 frameTimeTextSize = MeasureTextEx(defaultFont, frameTimeText, fontSize, spacing);
    Vector2 frameTimeTextLoc = {.x=leftX,.y=startY + 1 * verticalOffset};
    DrawRectangleRelative(frameTimeTextLoc, frameTimeTextSize, TopLeft, TopLeft, background);
    DrawTextRelativeEx(defaultFont, frameTimeText, frameTimeTextLoc, TopLeft, TopLeft, fontSize, spacing, textColor);

    char *windowTimeText = TextFormat("Window alive time: %.1f", GetTime());
    Vector2 windowTimeTextSize = MeasureTextEx(defaultFont, windowTimeText, fontSize, spacing);
    Vector2 windowTimeTextLoc = {.x=leftX,.y=startY + 2 * verticalOffset};
    DrawRectangleRelative(windowTimeTextLoc, windowTimeTextSize, TopLeft, TopLeft, background);
    DrawTextRelativeEx(defaultFont, windowTimeText, windowTimeTextLoc, TopLeft, TopLeft, fontSize, spacing, textColor);

    char *breakText = "----------";
    Vector2 breakTextSize = MeasureTextEx(defaultFont, breakText, fontSize, spacing);
    Vector2 breakTextLoc = {.x=leftX,.y=startY + 3 * verticalOffset};
    DrawRectangleRelative(breakTextLoc, breakTextSize, TopLeft, TopLeft, background);
    DrawTextRelativeEx(defaultFont, breakText, breakTextLoc, TopLeft, TopLeft, fontSize, spacing, textColor);
}