#include "Input.h"
#include "SDLHandler.h"

void HandleInput()
{
	while (SDL_PollEvent(&sdl.sdlEvent) != 0)
	{
		//User requests quit
		if (sdl.sdlEvent.type == SDL_QUIT)
		{
			sdl.quit = true;
		}
	}
}
