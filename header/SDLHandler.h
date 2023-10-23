#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <SDL.h>
#include <string>
struct SDL
{
	HWND window;
	SDL_Window* sdlWindow = nullptr;
	SDL_Surface* sdlSurface = nullptr;
	SDL_Renderer* sdlRenderer = nullptr;
	bool quit = false;
	SDL_Event sdlEvent;
	Uint32 WIDTH = 1600;
	Uint32 WINDOWED_WIDTH = 1600;
	Uint32 BASE_WIDTH = 1600;
	Uint32 HEIGHT = 900;
	Uint32 WINDOWED_HEIGHT = 900;
	Uint32 BASE_HEIGHT = 900;
	Uint32 windowFlags = 0;
};
extern SDL sdl;

/// <summary>
/// Sets up the window, returns true on success
/// </summary>
/// <returns>false on failure</returns>
bool SetupWindow();

/// <summary>///Alternatively accesssible through sdl.sdlWindow /// </summary>/// <returns></returns>
SDL_Window* GetSDLWindow();

void SetWindowTitle(std::string newTitle);