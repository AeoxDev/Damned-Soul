#include "States/Pause.h"
#include "Input.h"
#include "States\StateManager.h"
#include "UI/UIRenderer.h"
#include "Camera.h"
#include "DeltaTime.h"

void PauseState::Setup()
{
	RedrawUI();

	Camera::ResetCamera();

	m_active = true;
}

void PauseState::Input()
{
	if (keyState[SCANCODE_ESCAPE] == pressed)
	{
		SetInPause(false);
		SetInPlay(true);
		RedrawUI();
		gameSpeed = 1.0f;
		ResetInput();
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
