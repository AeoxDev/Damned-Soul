// DamnedSoul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDLhandler.h"
#include "MemLib/MemLib.hpp"
#include "EntityFramework.h"
#include "DeltaTime.h"
#include "States_&_Scenes\StateManager.h"
#include "GameRenderer.h"
#include "Model.h"
#include "ComponentHelper.h"
#include "UIRenderer.h"
#include "States_&_Scenes\StateManager.h"

void UpdateDebugWindowTitle(std::string& title);

int main(int argc, char* args[])
{
	MemLib::createMemoryManager();

	SetupWindow();
	std::string title = "Damned Soul";

	stateManager.Setup();
	
	while (!sdl.quit)
	{
		CountDeltaTime();
		
		Clear(backBufferRenderSlot);//Clear the render targets!
		
		stateManager.ComputeShaders();//First do compute shader work
		
		stateManager.Render();//Then render all registries that are active

		stateManager.Input();//Do all systems that are based on input
		
		UpdateDebugWindowTitle(title);//Update: CPU work. Do the CPU work after GPU calls for optimal parallelism
		
		stateManager.Update();//Lastly do the cpu work

		Present();//Present what was drawn during the update!
	
		MemLib::pdefrag();
		stateManager.EndFrame();
	}
	stateManager.UnloadAll();
	SDL_Quit();
	MemLib::destroyMemoryManager();
	return 0;
}

void UpdateDebugWindowTitle(std::string& title)
{
#ifdef _DEBUG
	if (sdl.windowFlags == 0 && NewSecond())
	{
		title = "Damned Soul " + std::to_string((int)(1000.0f * GetAverage())) + " ms (" + std::to_string(GetFPS()) + " fps)";
		//title+="";//Add more debugging information here, updates every second.
		SetWindowTitle(title);
	}
#endif // _DEBUG Debugging purposes, not compiled in release
}