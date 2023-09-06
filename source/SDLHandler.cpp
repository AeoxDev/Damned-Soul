#include "SDLHandler.h"
#include <iostream>
#include <d3d11.h>
#include "SDL.h"
#include "SDL_syswm.h"


SDL sdl;
bool SetupWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	sdl.sdlWindow = SDL_CreateWindow("Damned Soul", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl.WIDTH, sdl.HEIGHT, SDL_WINDOW_SHOWN);
	if (sdl.sdlWindow == nullptr)
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
			sdl.sdlRenderer = SDL_CreateRenderer(sdl.sdlWindow, i, 0);
			break;
		}
	}
	if (sdl.sdlRenderer == nullptr)
	{
		std::cerr << "SDL: Renderer unsuccessfully created! SDL_GetError() Yields: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(sdl.sdlWindow, &wmInfo) == -1)
	{
		std::cerr << "SDL: Unable to retrieve HWND! SDL_GetError() Yields: " << SDL_GetError() << std::endl;
		return false;
	}
	sdl.window = wmInfo.info.win.window;
	return true;
}

SDL_Window* GetSDLWindow()
{
	return sdl.sdlWindow;
}

void SetWindowTitle(std::string newTitle)
{
	SDL_SetWindowTitle(sdl.sdlWindow, newTitle.c_str());
}
