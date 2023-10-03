#include "States/Pause.h"
#include "Input.h"
#include "States\StateManager.h"

void PauseState::Setup()
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

void PauseState::ComputeShaders()
{

}

void PauseState::Unload()
{
}
