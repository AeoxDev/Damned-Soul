#include "SDLHandler.h"
#include <d3d11.h>
#include "SDL.h"
#include "SDL_syswm.h"
#include <assert.h>


SDL sdl;
bool SetupWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	sdl.sdlWindow = SDL_CreateWindow("Damned Soul", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl.WIDTH, sdl.HEIGHT, SDL_WINDOW_SHOWN);
	assert(sdl.sdlWindow != nullptr);
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
	assert(sdl.sdlRenderer != nullptr);
	SDL_SysWMinfo wmInfo{0};
	SDL_VERSION(&wmInfo.version);
	SDL_bool succeded = SDL_GetWindowWMInfo(sdl.sdlWindow, &wmInfo);
	assert(succeded == SDL_bool::SDL_TRUE);
	if (succeded == SDL_FALSE)
	{
		return false;
	}
	sdl.window = wmInfo.info.win.window;
	//Set windowed mode
	SDL_SetWindowFullscreen(sdl.sdlWindow, sdl.windowFlags);
	return true;
}

SDL_Window* GetSDLWindow()
{
	return sdl.sdlWindow;
}

void SetWindowTitle(const char* newTitle)
{
	SDL_SetWindowTitle(sdl.sdlWindow, newTitle);
}
