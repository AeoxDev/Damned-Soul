#include "SceneManager.h"

SceneManager::SceneManager()
{
	m_levels =
	{
		{"Level_1", Level("Level_1")},
		{"Level_2", Level("Level_2")},
		{"Level_3", Level("Level_3")}
	};
}

void SceneManager::Update()
{
	switch (m_currentSceneType)
	{
	case Scene::Level:
		m_levels.find(m_currentScene)->second.Update();
		break;
	case Scene::Shop:
		m_shop.Update();
		break;
	}

}

void SceneManager::SetScene(std::string id)
{
	m_currentScene = id;

	if (id != "Shop")
		m_currentSceneType = Scene::Level;
	else
		m_currentSceneType = Scene::Shop;

}
