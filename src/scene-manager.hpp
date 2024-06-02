#pragma once

#include "scene.hpp"

#include <unordered_map>
#include <string>
#include <vector>

class SceneManager
{
public:

	static int InitializeSceneManager(std::vector<std::pair<std::string, Scene*>> scenes);

	static void DeinitializeSceneManager();

	static Scene* GetCurrentScene();
	static void LoadScene(std::string sceneName);

private:
	static Scene* currentScene;
	static std::unordered_map<std::string, Scene*> scenes;

};