#include "States_&_Scenes\SceneManager.h"

SceneManager::SceneManager()
{
	std::vector<std::string> entityList = {"Imp", "Imp" , "Imp" };

	auto entity = m_registry.CreateEntity();
	m_registry.AddComponent<PlayerComponent>(entity, 10);

	m_levels.insert({ "Level_1", Level("Level_1", m_registry, entityList) });
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
	if (m_currentScene != "Shop")
		m_levels.find(m_currentScene)->second.ResetEntities(m_registry);
	m_currentScene = id;

	if (id != "Shop")
		m_currentSceneType = Scene::Level;
	else
		m_currentSceneType = Scene::Shop;

}

void SceneManager::AddScene(std::string name, std::vector<std::string> entityList)
{
	m_levels.insert({ name, Level(name, m_registry, entityList) });
}

void SceneManager::WriteEntities()
{
	if (m_currentSceneType == Scene::Level)
		m_levels.find(m_currentScene)->second.WriteEntities(m_registry);
	else
		m_shop.WriteEntities(m_registry);
}

std::string SceneManager::GetCurrentSceneName() const
{
	return m_currentScene;
}
