#pragma once
#include <d3d11.h>
#include <SDL.h>
#define DELTACAP 0.2f
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
void HandleInput();
bool SetupWindow();
SDL_Window* GetSDLWindow();