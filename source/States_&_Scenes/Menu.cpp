#include "States_&_Scenes\Menu.h"

void Menu::Update()
{

}

void Menu::HandleKeyInputs(int keyInput[], Settings& settings)
{
	switch (currentSubState)
	{
	case MenuState::Main:
		std::cout << "Main\n";

		break;
	case MenuState::Settings:
		std::cout << "Settings\n";

		if (keyInput[SDL_SCANCODE_ESCAPE])
			currentSubState = MenuState::Main;

		settings.HandleKeyInputs(keyInput);
		break;
	case MenuState::Credits:
		std::cout << "Credits\n";

		if (keyInput[SDL_SCANCODE_ESCAPE])
			currentSubState = MenuState::Main;

		break;
	}
}

void Menu::HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, Settings& settings, std::pair<int, int> mousePos)
{
	switch (currentSubState)
	{
	case MenuState::Main:
		std::cout << "Main\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Play").Intersects(mousePos))
			buttonManager.DoButtonAction("Play");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("MenuSettings").Intersects(mousePos))
			buttonManager.DoButtonAction("MenuSettings");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Credits").Intersects(mousePos))
			buttonManager.DoButtonAction("Credits");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Exit").Intersects(mousePos))
			buttonManager.DoButtonAction("Exit");

		break;
	case MenuState::Settings:
		std::cout << "Settings\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Main").Intersects(mousePos))
			buttonManager.DoButtonAction("Main");

		settings.HandleMouseInputs(mouseEvent, buttonManager, mousePos);
		break;
	case MenuState::Credits:
		std::cout << "Credits\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Main").Intersects(mousePos))
			buttonManager.DoButtonAction("Main");

		break;
	}
}
