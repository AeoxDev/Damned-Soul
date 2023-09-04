#pragma once
#include <d3d11.h>
#include <SDL.h>
extern bool quit;
extern HWND window;
void HandleInput();
bool SetupWindow();
SDL_Window* GetSDLWindow();