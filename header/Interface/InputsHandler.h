#pragma once
#include "SDLHandler.h"
#include "StateManager.h"

void ReadKeyEvent(int keyInput[])
{
	//Reads that a key has been pressed
	if (sdl.sdlEvent.type == SDL_KEYDOWN)
		keyInput[sdl.sdlEvent.key.keysym.scancode] = true;

	//Reads that a key has been released
	if (sdl.sdlEvent.type == SDL_KEYUP)
		keyInput[sdl.sdlEvent.key.keysym.scancode] = false;
}

void HandleInputs(StateManager& stateManager)
{
	while (SDL_PollEvent(&sdl.sdlEvent))
	{
		int keyInput[256]{ 0 };
		//ReadKeyEvent(keyInput);

		switch (sdl.sdlEvent.type)
		{
		case SDL_QUIT: //User requests quit
			sdl.quit = true;
			break;
		case SDL_KEYDOWN: //Reads that a key has been pressed
			keyInput[sdl.sdlEvent.key.keysym.scancode] = true;
			stateManager.HandleKeyInputs(keyInput);
			break;
		case SDL_KEYUP: //Reads that a key has been released
			keyInput[sdl.sdlEvent.key.keysym.scancode] = false;
			break;
		case SDL_MOUSEBUTTONDOWN: //Reads that a mouse button has been pressed
			stateManager.HandleMouseInputs(sdl.sdlEvent.button);
			break;
		case SDL_MOUSEBUTTONUP: //Reads that a mouse button has been released

			break;
		}
	}
	
}