#include "Input.h"
#include "SDLHandler.h"

int m_keyInput[256]{ 0 };
MouseButtonState mouseButtonPressed[3]{MouseButtonState::noEvent};//Left, right, middle
MouseButtonDown mouseButtonDown[3]{ MouseButtonDown::up };
int mouseX, mouseY;
SDL_MouseButtonEvent mouseButton;

void UpdateMouseState(int mouse)
{
	//First go from up  to pressed
	if (mouseButton.state == SDL_PRESSED)
	{
		mouseButtonPressed[mouse] = pressed;
		mouseButtonDown[mouse] = down;
	}
	else if (mouseButton.state == SDL_RELEASED)//Then go from pressed or down to released
	{
		mouseButtonDown[mouse] = up;
		mouseButtonPressed[mouse] = released;
	}
}

void GetInput()
{
	//Loop through each event; there can be multiple each frame
	while (SDL_PollEvent(&sdl.sdlEvent))
	{
		uint32_t mouseState =  SDL_GetMouseState(&mouseX, &mouseY);
		if (sdl.sdlEvent.key.repeat == 0)
		{
			switch (sdl.sdlEvent.type)
			{
			case SDL_QUIT: //User requests quit
				sdl.quit = true;
				break;
			case SDL_KEYDOWN: //Reads that a key has been pressed
				m_keyInput[sdl.sdlEvent.key.keysym.scancode] = true;
				break;
			case SDL_KEYUP: //Reads that a key has been released
				m_keyInput[sdl.sdlEvent.key.keysym.scancode] = false;
				break;
			case SDL_MOUSEBUTTONDOWN || SDL_MOUSEBUTTONUP: //Reads that a mouse button has been pressed
				//Find which mouse button
				mouseButton = sdl.sdlEvent.button;
				if (mouseButton.button == SDL_BUTTON_LEFT)
				{
					UpdateMouseState(0);
				}
				else if (mouseButton.button == SDL_BUTTON_RIGHT)
				{
					UpdateMouseState(1);
				}
				else if (mouseButton.button == SDL_BUTTON_MIDDLE)
				{
					UpdateMouseState(2);
				}
				break;
			default:
				break;
			}
		}
	}
}

void ResetMouse()
{
	mouseButtonPressed[0] = noEvent;
	mouseButtonPressed[1] = noEvent;
	mouseButtonPressed[2] = noEvent;
}
