#include "SDLHandler.h"
#include <iostream>
#include <d3d11.h>
#include "SDL.h"
#include "SDL_syswm.h"

using u32 = Uint32;

const u32 WIDTH = 1280;
const u32 HEIGHT = 720;

HWND window;
SDL_Window* sdlWindow = nullptr;
SDL_Surface* sdlSurface = nullptr;
SDL_Renderer* sdlRenderer = nullptr;
bool quit = false;
SDL_Event sdlEvent;
bool SetupWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	sdlWindow = SDL_CreateWindow("DV2556 Assignment 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (sdlWindow == nullptr)
	{
		std::cerr << "SDL: Window unsuccessfully created! SDL_GetError() Yields: " << SDL_GetError() << std::endl;
		return false;
	}
	for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
	{
		SDL_RendererInfo rendererInfo = {};
		SDL_GetRenderDriverInfo(i, &rendererInfo);
		//std::cout << rendererInfo.name << std::endl;
		if (rendererInfo.name == std::string("direct3d"))
		{
			sdlRenderer = SDL_CreateRenderer(sdlWindow, i, 0);
			break;
		}
	}
	if (sdlRenderer == nullptr)
	{
		std::cerr << "SDL: Renderer unsuccessfully created! SDL_GetError() Yields: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(sdlWindow, &wmInfo) == -1)
	{
		std::cerr << "SDL: Unable to retrieve HWND! SDL_GetError() Yields: " << SDL_GetError() << std::endl;
		return false;
	}
	window = wmInfo.info.win.window;
	return true;
}

void HandleInput()
{
	while (SDL_PollEvent(&sdlEvent) != 0)
	{

		//User requests quit
		if (sdlEvent.type == SDL_QUIT || sdlEvent.key.keysym.sym == SDLK_ESCAPE)
		{
			quit = true;
		}
	}
}

SDL_Window* GetSDLWindow()
{
	return sdlWindow;
}