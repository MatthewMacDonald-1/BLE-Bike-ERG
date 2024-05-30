#pragma once

#include "raylib.h"

class FontSettings {
public:
	static void InitializeFontSettings();
	static void DeinitializeFontSettings();
	
	static Font GetMainFont();
	static Font GetDebugFont();
	
private:
	static Font mainFont;
	static Font debugFont;
};