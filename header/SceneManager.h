#pragma once
#include <unordered_map>

#include "SceneStructures.h"

class SceneManager
{
private:

	Shop m_shop;
	std::unordered_map<std::string, Level> m_levels;

	Registry m_registry;

	Scene m_currentSceneType = Scene::Shop;
	std::string m_currentScene = "Shop";

public:
	SceneManager();
	~SceneManager() = default;

	void Update();

	void SetScene(std::string);
	void AddScene(std::string, std::vector<std::string>);

	void WriteEntities();

	std::string GetCurrentSceneName() const;
};
