#pragma once
#include <d3d11.h>
#include <SDL.h>
#define DELTACAP 0.2f
extern bool quit;
extern HWND window;
void HandleInput();
bool SetupWindow();
SDL_Window* GetSDLWindow();