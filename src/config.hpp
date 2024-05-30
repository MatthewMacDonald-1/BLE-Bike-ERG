#pragma once
#include "raylib.h"

#ifndef NDEBUG // If the cmake is release mode
#define DEBUG_TRACELOG
#define DEBUG
#endif

// Sets the gui debug overlay default state
#ifdef DEBUG
#define DEBUG_OVERLAY_DEFAULT_STATE true
#endif // DEBUG
#ifndef DEBUG
#define DEBUG_OVERLAY_DEFAULT_STATE false
#endif // DEBUG


//#define DISABLE_EXIT_KEY

// The game version
#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define PATCH_VERSION 1
#define STR_VERSION TextFormat("%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION)

// Min window size
#define MIN_WINDOW_WIDTH 1280
#define MIN_WINDOW_HEIGHT 720

#define SIGNAL_WINDOW_CLOSE -1

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif // !EXIT_SUCCESS

