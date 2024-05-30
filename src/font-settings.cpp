#include "font-settings.hpp"

Font FontSettings::mainFont;
Font FontSettings::debugFont;

void FontSettings::InitializeFontSettings()
{
	//mainFont = LoadFontEx("./resources/fonts/Nasalization/nasalization-rg.ttf", 128, NULL, 0);
	mainFont = LoadFontEx("./resources/fonts/Open Sans/static/OpenSans-Medium.ttf", 128, NULL, 0);
	//mainFont = LoadFontEx("./resources/fonts/Noto Sans/NotoSans-Regular.ttf", 128, NULL, 0);
	//mainFont = LoadFontEx("./resources/fonts/Roboto/Roboto-Regular.ttf", 128, NULL, 0);
	debugFont = LoadFontEx("./resources/fonts/Ubuntu Sans Mono/static/UbuntuSansMono-Regular.ttf", 128, NULL, 0);
}

void FontSettings::DeinitializeFontSettings()
{
	UnloadFont(mainFont);
	UnloadFont(debugFont);
}

Font FontSettings::GetMainFont()
{
	return mainFont;
}

Font FontSettings::GetDebugFont()
{
	return debugFont;
}
