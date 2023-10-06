#include "States/Pause.h"
#include "Input.h"
#include "States\StateManager.h"
#include "UI/UIRenderer.h"

void PauseState::Setup()
{
	RedrawUI();

	m_active = true;
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
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false
}
