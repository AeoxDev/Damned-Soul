#include "States\StateManager.h"
#include "Input.h"
#include "Model.h"
#include "Hitbox.h"
#include "Camera.h"
#include "Particles.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "UI/UIRenderer.h"
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
	Camera::InitializeCamera();
	menu.Setup();

	//Particles::InitializeParticles(); // THIS YIELDS MEMORY LEAK UNRELEASED OBJECT
	//SetupTestHitbox();
	RedrawUI();

	//Setup systems here

	//// Compute
	//systems.push_back(new ParticleSystemGPU());

	// Render/GPU
	systems.push_back(new UIRenderSystem());
	systems.push_back(new RenderSystem());

	// CPU
	systems.push_back(new ButtonSystem());

	//Input based CPU
	systems.push_back(new ControllerSystem());
	//systems.push_back(new ParticleSystemCPU());
	systems.push_back(new GeometryIndependentSystem());
	systems.push_back(new SkeletonBehaviourSystem());
	systems.push_back(new PointOfInterestSystem());
	systems.push_back(new HellhoundBehaviourSystem());
	systems.push_back(new TransformSystem());
	systems.push_back(new CollisionSystem());
	systems.push_back(new EventSystem());

	// Updating UI Elements (Needs to be last)
	systems.push_back(new UIHealthSystem());
	systems.push_back(new UIPlayerSoulsSystem());
	systems.push_back(new UIGameLevelSystem());

}



void StateManager::Input()
{
	//First read the keys
	

	//Then go through the registries that are mode
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
	Input();
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
