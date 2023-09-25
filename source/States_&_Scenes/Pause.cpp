#include "States_&_Scenes/Pause.h"
#include "Input.h"
#include "States_&_Scenes\StateManager.h"

void PauseState::Setup()
{

}

void PauseState::Clear()
{
}

void PauseState::ComputeShaders()
{
}

void PauseState::Render()
{
}

void PauseState::Input()
{
	if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
	{
		SetInPause(false);
		SetInPlay(true);
		stateManager.levelScenes[0].Setup(0);
		Unload();
	}
}

void PauseState::Update()
{
}

void PauseState::Unload()
{
}
