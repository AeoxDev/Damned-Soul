#include "States\StateManager.h"
#include "Input.h"
#include "Model.h"
#include "Hitbox.h"
#include "Camera.h"
#include "Particles.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "UIRenderer.h"
#include "Particles.h"
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

	Particles::InitializeParticles();
	SetupTestHitbox();
	RedrawUI();

	//Setup systems here
	systems.push_back(new ParticleSystemGPU());
	systems.push_back(new RenderSystem());
	systems.push_back(new ButtonSystem());
	systems.push_back(new ControllerSystem());
	systems.push_back(new ParticleSystemCPU());
	systems.push_back(new GeometryIndependentSystem());
	systems.push_back(new PointOfInterestSystem());
	systems.push_back(new PlayerHealthUISystem());
	systems.push_back(new PlayerSoulsUISystem());
	systems.push_back(new UIRenderSystem());
}

void StateManager::Input()
{
	//First read the keys
	

	//Then go through the registries that are active
	if (currentStates & State::InMainMenu)
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
	}
}

void StateManager::Update()
{
	for (size_t i = 0; i < systems.size(); i++)
	{
		systems[i]->Update();
	}
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

void StateManager::UnloadAll()
{
	menu.Unload();
	settings.Unload();
	shop.Unload();
	levelScenes[0].Unload();
	levelScenes[1].Unload();

	Particles::ReleaseParticles();
	DestroyHitboxVisualizeVariables();
	ReleaseUIRenderer();
	ui.Release();
	EndDirectX();
}

void StateManager::EndFrame()
{
	Present();//Present what was drawn during the update!
	ResetInput();
	GetInput();
	//MemLib::pdefrag();
}
