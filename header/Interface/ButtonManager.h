#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "SDLHandler.h"

struct Button
{
	std::pair<int, int> origin;
	std::pair<int, int> extends;

	std::function<void()> action;

	Button(std::pair<int, int> origin, std::pair<int, int> extends, std::function<void()> action) :origin(origin), extends(extends), action(action) {};

	bool Intersects(std::pair<int, int> position) 
	{
		return position.first > origin.first - extends.first && position.first < origin.first + extends.first &&
			position.second > origin.second - extends.second && position.second < origin.second + extends.second;
	};
};

class ButtonManager
{
private:
	std::unordered_map<std::string, Button> m_buttons;

public:

	void AddButton(std::string, std::pair<int, int>, std::pair<int, int>, std::function<void()>);
	void DoButtonAction(std::string);

	Button GetButton(std::string) const;
};
