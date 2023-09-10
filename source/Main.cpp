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
#include "Camera.h"
#include "GameRenderer.h"

#include "ExampleMenu.h"
#include "D2D1Graphics.h"
#include <iostream>

int main(int argc, char* args[])
{
	MemLib::createMemoryManager();
	InitiateConfig();
    SetupWindow();
	std::string title = "Damned Soul";

	int testRenderSlot = SetupGameRenderer();
	InitializeCamera();
	SetConstantBuffer(GetCameraBufferIndex());

	/////////// REMOVE //////////////
	ID3D11Texture2D* GUIBuffer;
	ID3D11RenderTargetView* GUIRTV;

	if (FAILED(d3d11Data->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&GUIBuffer))))
	{
		std::cerr << "Failed to create GUI Buffer\n";
		return false;
	}
	
	hr = d3d11Data->device->CreateRenderTargetView(GUIBuffer, nullptr, &GUIRTV);
	if (FAILED(hr))
	{
		std::cout << "Failed to create GUI Render Target View\n";
		return false;
	}

	auto hres = GUIBuffer->QueryInterface<IDXGISurface>(&UISurface);
	if (FAILED(hres))
	{
		printf("Failed to create UI Surface\n");
		return -1;
	}

	PoolPointer<UI> ui = MemLib::palloc(sizeof(UI));
	*ui = UI();
	PoolPointer<ExMenu>apa = MemLib::palloc(sizeof(ExMenu));
	*apa = ExMenu(ui);
	
	
	/////////// REMOVE //////////////

	int i = 0;
	while (!sdl.quit)
	{
		CountDeltaTime();

		//Render: GPU calls. Always tell the GPU what to do first for optimal parallelism
		Render(testRenderSlot);

		//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		HandleInput();
#ifdef _DEBUG
		if (sdl.windowFlags == 0 && NewSecond())
		{
			title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps)";
			//title+="";//Add more debugging information here, updates every second.
			SetWindowTitle(title);
		}
#endif // _DEBUG Debugging purposes, not compiled in release

		
		
		//Update
		/////////// REMOVE //////////////
		apa->Run(ui, 1.0f);
		/////////// REMOVE //////////////'

		ClearRenderTargetView(rtv);
		ClearDepthStencilView(dsv);

		d3d11Data->deviceContext->DrawIndexed(3, 0, 0);
		//d3d11Data->deviceContext->Draw(3, 0);
		d3d11Data->swapChain->Present(0, 0);

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
