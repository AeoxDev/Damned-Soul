#include "States_&_Scenes\Settings.h"

void Settings::HandleKeyInputs(int keyInput[])
{
	switch (currentSubState)
	{
	case SettingsState::Graphics:
		std::cout << "Graphics\n";

		if (keyInput[SDL_SCANCODE_LEFT])
			currentSubState = SettingsState::Controls;

		if (keyInput[SDL_SCANCODE_RIGHT])
			currentSubState = SettingsState::Audio;

		break;
	case SettingsState::Audio:
		std::cout << "Audio\n";

		if (keyInput[SDL_SCANCODE_LEFT])
			currentSubState = SettingsState::Graphics;

		if (keyInput[SDL_SCANCODE_RIGHT])
			currentSubState = SettingsState::Controls;

		break;
	case SettingsState::Controls:
		std::cout << "Controls\n";

		if (keyInput[SDL_SCANCODE_LEFT])
			currentSubState = SettingsState::Audio;

		if (keyInput[SDL_SCANCODE_RIGHT])
			currentSubState = SettingsState::Graphics;

		break;
	}
}

void Settings::HandleMouseInputs(SDL_MouseButtonEvent mouseEvent, ButtonManager buttonManager, std::pair<int, int> mousePos)
{
	switch (currentSubState)
	{
	case SettingsState::Graphics:
		std::cout << "Graphics\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Audio").Intersects(mousePos))
			buttonManager.DoButtonAction("Audio");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Controls").Intersects(mousePos))
			buttonManager.DoButtonAction("Controls");

		break;
	case SettingsState::Audio:
		std::cout << "Audio\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Graphics").Intersects(mousePos))
			buttonManager.DoButtonAction("Graphics");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Controls").Intersects(mousePos))
			buttonManager.DoButtonAction("Controls");

		break;
	case SettingsState::Controls:
		std::cout << "Controls\n";

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Graphics").Intersects(mousePos))
			buttonManager.DoButtonAction("Graphics");

		if (mouseEvent.button == SDL_BUTTON_LEFT && buttonManager.GetButton("Audio").Intersects(mousePos))
			buttonManager.DoButtonAction("Audio");

		break;
	}
}
