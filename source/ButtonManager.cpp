#include "ButtonManager.h"

void ButtonManager::AddButton(std::string id, std::pair<int, int> origin, std::pair<int, int> extends, std::function<void()> action)
{
	m_buttons.insert(std::pair<std::string, Button>(id, Button(origin, extends, action)));
}

void ButtonManager::DoButtonAction(std::string id)
{
	m_buttons.find(id)->second.action();
}

Button ButtonManager::GetButton(std::string id) const
{
	return m_buttons.find(id)->second;
}
