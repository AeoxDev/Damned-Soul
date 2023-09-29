#include "States_&_Scenes\StateManager.h"
#include "Input.h"
#include "Model.h"
#include "Hitbox.h"
#include "Camera.h"
#include "Particles.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "UIRenderer.h"
State currentStates;
StateManager stateManager;

void SetInMainMenu(bool value)
{
	
	if (value)
	{
		currentStates = (State)(currentStates | State::InMainMenu);
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InMainMenu));
	}
}
void SetInPlay(bool value)
{

	if (value)
	{
		currentStates = (State)(currentStates | State::InPlay);
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InPlay));
	}
}
void SetInPause(bool value)
{

	if (value)
	{
		currentStates = (State)(currentStates | State::InPause);
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InPause));
	}
}
void SetInSettings(bool value)
{

	if (value)
	{
		currentStates = (State)(currentStates | State::InSettings);
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InSettings));
	}
}
void SetInShop(bool value)
{

	if (value)
	{
		currentStates = (State)(currentStates | State::InShop);
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InShop));
	}
}

void StateManager::Setup()
{
	Setup3dGraphics();
	ui.RenderSlot = SetupUIRenderState();

	ui.Setup();

	backBufferRenderSlot = SetupGameRenderer();
	currentStates = InMainMenu;
	//models.Initialize();
	menu.Setup();
	Camera::InitializeCamera();
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_VERTEX);

	Particles::InitializeParticles();
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_GEOMETRY);
	SetupTestHitbox();

	//Setup systems here
	systems.push_back(new RenderSystem());
}

void StateManager::Clear()
{
	//Find which registries for clear system
}

void StateManager::ComputeShaders()
{
	/*if (currentStates & State::InMainMenu)
	{
		menu.ComputeShaders();
	}
	if (currentStates & State::InPause)
	{
		pause.ComputeShaders();
	}
	if (currentStates & State::InSettings)
	{
		settings.ComputeShaders();
	}
	if (currentStates & State::InPlay)
	{
		levelScenes[activeLevelScene].ComputeShaders();
	}
	if (currentStates & State::InShop)
	{
		shop.ComputeShaders();
	}*/
}

void StateManager::Render()
{
	

	//Find which registries for render system
	/*if (currentStates & State::InMainMenu)
	{
		menu.Render();
	}
	if (currentStates & State::InPause)
	{
		pause.Render();
	}
	if (currentStates & State::InSettings)
	{
		settings.Render();
	}
	if (currentStates & State::InPlay)
	{
		levelScenes[activeLevelScene].Render();
	}
	if (currentStates & State::InShop)
	{
		shop.Render();
	}*/
}

void StateManager::Input()
{
	//First read the keys
	

	//Then go through the registries that are active
	/*if (currentStates & State::InMainMenu)
	{
		menu.Input();
	}
	if (currentStates & State::InPause)
	{
		pause.Input();
	}
	if (currentStates & State::InSettings)
	{
		settings.Input();
	}
	if (currentStates & State::InPlay)
	{
		levelScenes[activeLevelScene].Input();
	}
	if (currentStates & State::InShop)
	{
		shop.Input();
	}*/
}

void StateManager::Update()
{
	for (size_t i = 0; i < systems.size(); i++)
	{
		systems[i]->Update();
	}
	/*if (currentStates & State::InMainMenu)
	{
		menu.Update();
	}
	if (currentStates & State::InPause)
	{
		pause.Update();
	}
	if (currentStates & State::InSettings)
	{
		settings.Update();
	}
	if (currentStates & State::InPlay)
	{
		levelScenes[activeLevelScene].Update();
	}
	if (currentStates & State::InShop)
	{
		shop.Update();
	}*/
	//Find which registries to update
	//switch (m_currentState)
	//{
	//case State::Menu:

	//	if (m_menu.currentSubState == MenuState::Main)
	//		m_menu.Update();
	//	break;
	//case State::Game:

	//	if (m_game.currentSubState == GameState::Unpause)
	//	{
	//		//In the future, we may not want to update particles on every update call
	//		//Currently lacking an indicator as to when we want to update particles
	//		m_game.UpdateParticles();

	//		m_game.Update();
	//	}

	//	break;
	//}

	//SDL_GetMouseState(&mousePos.first, &mousePos.second);
	//if (mousePos != oldmousepos)
	//	//std::cout << mousePos.first << ", " << mousePos.second << std::endl;

	//oldmousepos = mousePos;
}
void StateManager::EndFrame()
{
	Present();//Present what was drawn during the update!
	ResetInput();
	GetInput();
	//MemLib::pdefrag();
}
void StateManager::UnloadAll()
{
	menu.Unload();
	settings.Unload();
	shop.Unload();
	levelScenes[0].Unload();
	levelScenes[1].Unload();
	DestroyHitboxVisualizeVariables();
	ReleaseUIRenderer();
	ui.Release();
	EndDirectX();
}