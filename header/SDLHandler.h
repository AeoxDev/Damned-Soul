#pragma once
#include <d3d11.h>
#include <SDL.h>
struct SDL
{
	HWND window;
	SDL_Window* sdlWindow = nullptr;
	SDL_Surface* sdlSurface = nullptr;
	SDL_Renderer* sdlRenderer = nullptr;
	bool quit = false;
	SDL_Event sdlEvent;
	Uint32 WIDTH = 1280;
	Uint32 HEIGHT = 720;
};
extern SDL sdl;

/// <summary>
/// Sets up the window, returns true on success
/// </summary>
/// <returns>false on failure</returns>
bool SetupWindow();

/// <summary>///Alternatively accesssible through sdl.sdlWindow /// </summary>/// <returns></returns>
SDL_Window* GetSDLWindow();