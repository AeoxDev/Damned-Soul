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
	while (SDL_PollEvent(&sdl.sdlEvent) != 0)
	{
		int keyInput[256]{ 0 };
		ReadKeyEvent(keyInput);

		//User requests quit
		if (sdl.sdlEvent.type == SDL_QUIT)
			sdl.quit = true;

		stateManager.HandleInputs(keyInput);

	}
	
}