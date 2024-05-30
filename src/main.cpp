#include <iostream>
#include <string>
#include <chrono>

#include "raylib.h"
#include "raymath.h"

#include "config.hpp"
#include "font-settings.hpp"
#include "video-settings.hpp"
#include "bluetooth-controller.hpp"
#include "scene.hpp"
#include "main-menu.hpp"

#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"

bool bleSupported = false;

bool drawGUIDebug = DEBUG_OVERLAY_DEFAULT_STATE;
void DrawGUIDebug();

int main(void) {

#ifdef DEBUG
    std::cout << "World Engines Version: " << STR_VERSION << std::endl;
#endif // DEBUG

    // Read in video settings
    int res = VideoSettings::ReadVideoSettingsConfigFile();
    if (res == EXIT_FAILURE) {
        VideoSettings::WriteVideoSettingsConfigFile();
    }


    // Raylib Initialization ----------------------------------------------------------------------
#ifndef DEBUG_TRACELOG
    SetTraceLogLevel(LOG_WARNING); // Disables debugging 
#endif // !DEBUG_TRACELOG

    VideoSettings::PreWindowCreationFlags();

    InitWindow(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, "World Engines");
    SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    
    VideoSettings::PostWindowCreatedFlags();

#ifdef DISABLE_EXIT_KEY
    SetExitKey(0); // Disable esc key closing the window
#endif // DISABLE_EXIT_KEY
    // raylib initialization end ------------------------------------------------------------------

    // Load Fonts
    FontSettings::InitializeFontSettings();

    // BLE Initilization
    int bleRes = BluetoothController::InitializeBluetooth();
    if (BluetoothController::BluetoothSupported() && bleRes == EXIT_SUCCESS) {
        bleSupported = true;
    }

    /// Window Start time in ms.
    long long windowStartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long frameNum = 0;

    Scene* currentScene = new MainMenuScene();

    while (!WindowShouldClose()) { // Render loop

        // Do logic here
        if (IsKeyPressed(KEY_F3)) {
            drawGUIDebug = !drawGUIDebug;
        }

        int logicRes = currentScene->UpdateLogic();
        if (logicRes == -1) break;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Scene Here.
        int drawRes = currentScene->DrawCall();
        if (drawRes == -1) break;

        if (drawGUIDebug) {
            DrawGUIDebug();
        }

        EndDrawing();

        

        frameNum++;
    }

    // UnloadFonts
    FontSettings::DeinitializeFontSettings();

    CloseWindow();

    return EXIT_SUCCESS;
}

void DrawGUIDebug()
{
    using namespace MattsUtils::raylib;
    using namespace MattsUtils::RelativeDrawing;
    Vector2 origin = ConstructVector2(5, 5);
    Vector2 spacing = ConstructVector2(0, 32);
    int fontSize = 24;
    float fontSpacing = 1.0;
    Color fontColor = ConstructColor(104, 104, 104);
    Font fontType = FontSettings::GetDebugFont();

    PinLocation pane1Location = TopLeft;
    PinLocation pane2Location = TopRight;

    // Pane 1 Start -------------------------------------------------------------------------------
    int rowIdx = 0;
    DrawTextRelEx(fontType, TextFormat("Build: %s", STR_VERSION), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane1Location, pane1Location, fontSize, fontSpacing, fontColor); rowIdx++;

    double time = GetTime();
    int ms = 0;
    int sec = 0;
    int min = 0;
    int hour = 0;

    ms = (int)(time * 100.0);
    if (ms >= 100) {
        ms = ms % 100;
    }

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

    std::string aliveTime = hour == 0 ? TextFormat("%s%d:%s%d.%s%d", (min < 10 ? "0" : ""), min, (sec < 10 ? "0" : ""), sec, (ms < 10 ? "0" : ""), ms) : TextFormat("%s%d:%s%d:%s%d.%s%d", (hour < 10 ? "0" : ""), hour, (min < 10 ? "0" : ""), min, (sec < 10 ? "0" : ""), sec, (ms < 10 ? "0" : ""), ms);
    DrawTextRelEx(fontType, TextFormat("Alive Time: %s", aliveTime.c_str()), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane1Location, pane1Location, fontSize, fontSpacing, fontColor); rowIdx++;
    DrawTextRelEx(fontType, TextFormat("BLE Support: %s", (bleSupported ? "Yes" : "No")), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane1Location, pane1Location, fontSize, fontSpacing, fontColor); rowIdx++;

    // Pane 1 End ---------------------------------------------------------------------------------

    // Graphics Panel Start -----------------------------------------------------------------------
    rowIdx = 0;
    origin.x = -origin.x;

    DrawTextRelEx(fontType, TextFormat("Display Res: %dx%d", GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor())), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane2Location, pane2Location, fontSize, fontSpacing, fontColor); rowIdx++;
    DrawTextRelEx(fontType, TextFormat("Window Res: %dx%d", GetScreenWidth(), GetScreenHeight()), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane2Location, pane2Location, fontSize, fontSpacing, fontColor); rowIdx++;
    DrawTextRelEx(fontType, TextFormat("Render Res: %dx%d", GetRenderWidth(), GetRenderHeight()), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane2Location, pane2Location, fontSize, fontSpacing, fontColor); rowIdx++;
    DrawTextRelEx(fontType, TextFormat("FPS: %d%s", GetFPS(), VideoSettings::VSYNC == VideoSettings::GetCurrentFPSLimit() ? " (VSYNC)" : ""), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane2Location, pane2Location, fontSize, fontSpacing, fontColor); rowIdx++;

    std::string msaaStr = VideoSettings::GetActualMSAALevel() == VideoSettings::_x1 ? "x1" : "x4";
    DrawTextRelEx(fontType, TextFormat("MSAA: %s", msaaStr.c_str()), Vector2Add(origin, Vector2Scale(spacing, rowIdx)), pane2Location, pane2Location, fontSize, fontSpacing, fontColor); rowIdx++;
    

    // Graphics Panel End -------------------------------------------------------------------------
}
