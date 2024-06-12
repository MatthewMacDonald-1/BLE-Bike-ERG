#include "workout-selection-scene.hpp"
#include "config.hpp"

#include "raygui.h"
#include "raymath.h"

#include <string>
#include <iostream>
#include <sstream>
#include <thread>

#include "font-settings.hpp"
#include "MattsUtils/relative-drawing.hpp"
#include "MattsUtils/raylib-structs.hpp"
#include "settings-menu.hpp"
#include "scene-manager.hpp"
#include "workout-scene.hpp"
#include "bluetooth-controller.hpp"

using namespace MattsUtils;

WorkoutSelectionMenuScene::WorkoutSelectionMenuScene()
{
	menuBackground = LoadTexture("./resources/images/test-background.png");

	//workouts.push_back(new WorkoutDefinition("./resources/workouts/test.workout"));
	workouts = WorkoutDefinition::LoadFromDirectory("./resources/workouts/");

	for (int i = 0; i < workouts.size(); i++) {
		std::stringstream ss;
		ss << "Workout_" << workouts.at(i)->GetId();
		SceneManager::AddScene(ss.str(), new WorkoutScene(workouts.at(i)));
	}
}

WorkoutSelectionMenuScene::~WorkoutSelectionMenuScene()
{
	UnloadTexture(menuBackground);
}

int WorkoutSelectionMenuScene::UpdateLogic()
{
	return EXIT_SUCCESS;
}

int WorkoutSelectionMenuScene::DrawCall()
{
	Font fontType = FontSettings::GetMainFont();

	Vector2 buttonSize = raylib::ConstructVector2(128, 32);
	Vector2 offsetDstBL = raylib::ConstructVector2(24, -24);
	Vector2 offsetDstBC = raylib::ConstructVector2(0, -24);
	Vector2 offsetDstBR = raylib::ConstructVector2(-24, -24);

	DrawTextureEx(menuBackground, raylib::ConstructVector2(0, 0), 0, (float)GetScreenWidth() / (float)menuBackground.width, WHITE);

	if (menuOpen) {
		int res = SettingsMenu::DrawSettingsMenu();
		if (res == SIGNAL_SETTINGS_MENU_CLOSE) {
			menuOpen = false;
			return EXIT_SUCCESS;
		}
		else {
			return res;
		}
	}

	RelativeDrawing::DrawTextRelEx(fontType, "Workout Selection", raylib::ConstructVector2(0, 16), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 64, 1.5, BLACK);


	std::string workoutName = workouts.at(0)->GetName();
	RelativeDrawing::DrawTextRelEx(fontType, TextFormat("%s - Length: %d sec", workoutName.c_str(), workouts.at(0)->GetWorkoutLength()), raylib::ConstructVector2(0, 256), RelativeDrawing::TopCenter, RelativeDrawing::TopCenter, 32, 1.5, BLACK);

	// Scroll Area Start --------------------------------------------------------------------------

	Vector2 topCorner = raylib::ConstructVector2(24, 148);
	Vector2 panelDimensions = raylib::ConstructVector2(GetScreenWidth() - 24 * 2, GetScreenHeight() - 240);
	panelRec = raylib::ConstructRectangle(topCorner.x, topCorner.y, panelDimensions.x, panelDimensions.y);
	panelContentRec.width = panelRec.width - (GuiGetStyle(SCROLLBAR, SLIDER_WIDTH) + 8);


	GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll, &panelView);

	BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
	int panelInnerHeight = 0;

	panelInnerHeight += DrawWorkoutListHeading(
		"Workouts",
		raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight),
		panelContentRec.width
	);

	for (int i = 0; i < workouts.size(); i++) {

		panelInnerHeight += DrawWorkoutButton(
			workouts.at(i),
			raylib::ConstructVector2(panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + panelInnerHeight),
			panelContentRec.width
		);

		
	}

	panelContentRec.height = panelInnerHeight != panelContentRec.height ? panelInnerHeight : panelContentRec.height;

	EndScissorMode();


	// Scroll Area End ----------------------------------------------------------------------------

	// Buttons Start ------------------------------------------------------------------------------
	GuiSetFont(fontType);

	bool backToStartMenuSceneRes = RelativeDrawing::GuiButtonRelative("Back", offsetDstBC, buttonSize, RelativeDrawing::BottomCenter, RelativeDrawing::BottomCenter, 24);
	if (backToStartMenuSceneRes) {
		std::vector<SimpleBLE::Peripheral> devices = BluetoothController::GetConnectedDevices();

		for (int i = 0; i < devices.size(); i++) {
			std::thread connectThread(BluetoothController::DisconnectFromDevice, devices.at(i));
			connectThread.detach(); // Runs the thread detached
		}


		SceneManager::LoadScene("MainMenu");
	}


	bool settingRes = RelativeDrawing::GuiButtonRelative("Settings", offsetDstBL, buttonSize, RelativeDrawing::BottomLeft, RelativeDrawing::BottomLeft, 24);
	if (settingRes) {
		menuOpen = true;
	}
	// Buttons End --------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}

int WorkoutSelectionMenuScene::DrawWorkoutListHeading(std::string heading, Vector2 position, int width)
{
	Font fontType = FontSettings::GetMainFont();

	RelativeDrawing::DrawRectangle(
		position,
		raylib::ConstructVector2(width, 44),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		raylib::ConstructColor(255, 255, 255)
	);
	RelativeDrawing::DrawRectangle(
		raylib::ConstructVector2(position.x, position.y + 44),
		raylib::ConstructVector2(width, 2),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		raylib::ConstructColor(206, 206, 206)
	);

	RelativeDrawing::DrawTextRelEx(
		fontType,
		heading.c_str(),
		raylib::ConstructVector2(position.x + 10, position.y + 10),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		24,
		1.5,
		BLACK
	);

	return 46;
}

int WorkoutSelectionMenuScene::DrawWorkoutButton(WorkoutDefinition* workout, Vector2 position, int width)
{
	Font fontType = FontSettings::GetMainFont();

	RelativeDrawing::DrawRectangle(
		position,
		raylib::ConstructVector2(width, 64),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		raylib::ConstructColor(255, 255, 255)
	);
	RelativeDrawing::DrawRectangle(
		raylib::ConstructVector2(position.x, position.y + 64),
		raylib::ConstructVector2(width, 2),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		raylib::ConstructColor(206, 206, 206)
	);

	std::stringstream duration;
	duration << "Duration";

	RelativeDrawing::DrawTextRelEx(
		fontType,
		TextFormat("%s - %s", workout->GetName().c_str(), workout->GetWorkoutLegthStr().c_str()),
		raylib::ConstructVector2(position.x + 10, position.y + 10),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		24,
		1.5,
		BLACK
	);

	int clicked = RelativeDrawing::GuiButtonRelative(
		"Ride",
		raylib::ConstructVector2(position.x + width - 10 - 128, position.y + 16),
		raylib::ConstructVector2(128, 32),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		24
	);

	int workoutGraphWidth = 192;
	workout->DrawWorkout(
		raylib::ConstructVector2(position.x + width - 10 * 2 - 128 - workoutGraphWidth, position.y + 16),
		workoutGraphWidth,
		32,
		4
	);

	if (clicked) {
		// Clicked
		std::stringstream ss;
		ss << "Workout_" << workout->GetId();
		SceneManager::LoadScene(ss.str());
	}


	RelativeDrawing::DrawTextRelEx(
		fontType,
		TextFormat("%s", workout->GetTargetTypeStr().c_str()),
		raylib::ConstructVector2(position.x + 10, position.y + 10 + 24),
		RelativeDrawing::TopLeft,
		RelativeDrawing::TopLeft,
		16,
		1.5,
		BLACK
	);

	return 66;
}
