// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include "EntityFramework.h"
#include "ConfigManager.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "GameRenderer.h"
#include "States_&_Scenes\StateManager.h"
#include "Model.h"


int main(int argc, char* args[])
{
	MemLib::createMemoryManager();
	InitiateConfig();
    SetupWindow();
	std::string title = "Damned Soul";

	int testRenderSlot = SetupGameRenderer();
	InitializeCamera();
	SetConstantBuffer(GetCameraBufferIndex());

	Model eyeModel;
	if (false == eyeModel.Load("FlyingEyeDummy.mdl"))
		return -1;
	Model dogModel;
	if (false == dogModel.Load("Hellhound.mdl"))
		return -1;
	//testModel.SetVertexAndIndexBuffersActive();

	StateManager stateManager; //Outside of memlib at the moment, might fix later if necessary.

	while (!sdl.quit)
	{
		CountDeltaTime();

		Clear(testRenderSlot);

		//Render: GPU calls. Always tell the GPU what to do first when possible for optimal parallelism
		eyeModel.SetVertexAndIndexBuffersActive();
		Render(eyeModel.m_bonelessModel->m_numIndices);

		dogModel.SetVertexAndIndexBuffersActive();
		Render(dogModel.m_bonelessModel->m_numIndices);

		Present();

		//Inputs: SDL readings of keyboard and mouse inputs
		stateManager.HandleInputs();

		//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		stateManager.Update();

#ifdef _DEBUG
		if (sdl.windowFlags == 0 && NewSecond())
		{
			title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps)";
			//title+="";//Add more debugging information here, updates every second.
			SetWindowTitle(title);
		}
#endif // _DEBUG Debugging purposes, not compiled in release

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
