// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "Input.h"
#include "MemLib/MemLib.hpp"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include "EntityFramework.h"
#include "ConfigManager.h"
#include "DeltaTime.h"
#include "States.h"

int main(int argc, char* args[])
{
	MemLib::createMemoryManager();
	InitiateConfig();
    
    //std::cout << "Hello World!\n";
    SetupWindow();
	std::string title = "Damned Soul";
	//Hwnd = sdl.window

	int hr = SetupDirectX(sdl.window);

	RS_IDX rasterizer = CreateRasterizerState(true, true);
	bool s = SetRasterizerState(rasterizer);

	PS_IDX testPixelShader = LoadPixelShader("TestPS.cso");
	s = SetPixelShader(testPixelShader);
	VS_IDX testVertexShader = LoadVertexShader("TestVS.cso");
	s = SetVertexShader(testVertexShader);

	struct Vertex {
		float pos[4];
		float normal[4];
		float uv[2]; };
	Vertex triangle[3] = {
		0.9f, -0.9, 0.5f, 1.f,		/**/ 0, 0, -1.f, 0, /**/ 1, 0,
		-0.9f, -0.9, 0.5f, 1.f,		/**/ 0, 0, -1.f, 0, /**/ 0, 0,
		0, 0.9f, 0.5f, 1.f,			/**/ 0, 0, -1.f, 0, /**/ 0.5, 1 };
	VB_IDX vertexBuffer = CreateVertexBuffer(triangle, sizeof(Vertex), 3);
	uint32_t idxs[3] = { 0, 1, 2 };
	IB_IDX indexBuffer = CreateIndexBuffer(idxs, sizeof(uint32_t), 3);

	s = SetVertexBuffer(vertexBuffer);
	s = SetIndexBuffer(indexBuffer);

	d3d11Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a viewport
	VP_IDX vp = CreateViewport(sdl.WIDTH, sdl.HEIGHT);
	// Set the viewport
	s = SetViewport(vp);
	// Create a render target view
	RTV_IDX rtv = CreateRenderTargetView();
	// Create a depth stencil view
	DSV_IDX dsv = CreateDepthStencil(sdl.WIDTH, sdl.HEIGHT);
	// Set a render target view and depth stencil view
	s = SetRenderTargetViewAndDepthStencil(rtv, dsv);

	int i = 0;
	State currentMainState = State::Menu;

	Game game;
	Menu menu;
	Settings settings;

	settings.resolution.first = sdl.WIDTH;
	settings.resolution.second = sdl.HEIGHT;

	while (!sdl.quit)
	{
		CountDeltaTime();
		HandleInput();
#ifdef _DEBUG
		if (sdl.windowFlags == 0 && NewSecond())
		{
			title = "Damned Soul " + std::to_string((int)(1000.0f*GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps)";
			//title+="";//Add more debugging information here, updates every second.
			SetWindowTitle(title);
		}
#endif // _DEBUG Debugging purposes, not compiled in release

		
		
		//Update
		HandleStateInput(currentMainState, game, menu, settings);

		
		ClearRenderTargetView(rtv);
		ClearDepthStencilView(dsv);

		d3d11Data->deviceContext->DrawIndexed(3, 0, 0);
		//d3d11Data->deviceContext->Draw(3, 0);
		d3d11Data->swapChain->Present(0, 0);

		SDL_RenderClear(sdl.sdlRenderer);
		SDL_RenderPresent(sdl.sdlRenderer);
		MemLib::pdefrag();
	}

	EndDirectX();
	MemLib::destroyMemoryManager();
	SDL_Quit();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
