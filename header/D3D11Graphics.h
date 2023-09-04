#pragma once
#include <d3d11.h>
//This is the back end

extern ID3D11DeviceContext* deviceContext;

/// <summary>/// /// </summary>/// <param name="window"> use sdl.window</param>/// <returns></returns>
int SetupDirectX(HWND& window);