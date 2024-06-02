#include "scene-manager.hpp"

#include "raylib.h"
#include <iostream>

Scene* SceneManager::currentScene = NULL;
std::unordered_map<std::string, Scene*> SceneManager::scenes;

int SceneManager::InitializeSceneManager(std::vector<std::pair<std::string, Scene*>> sceneList)
{
	for (int i = 0; i < sceneList.size(); i++) {
		scenes[sceneList.at(i).first] = sceneList.at(i).second;
	}

	return EXIT_SUCCESS;
}

void SceneManager::DeinitializeSceneManager()
{
}

Scene* SceneManager::GetCurrentScene()
{
	return currentScene;
}

void SceneManager::LoadScene(std::string sceneName)
{
	try {
		TraceLog(LOG_INFO, "SceneManager: Loading scene %s.", sceneName.c_str());
		currentScene = scenes[sceneName];

		if (currentScene == NULL) {
			TraceLog(LOG_ERROR, "SceneManager: Scene %s does not exist.", sceneName.c_str());
			exit(EXIT_FAILURE);
		}
	}
	catch (...) {
		TraceLog(LOG_ERROR, "SceneManager: Scene %s does not exist.", sceneName.c_str());
		exit(EXIT_FAILURE);
	}
}
