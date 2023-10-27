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
#include "D3D11Graphics.h"
#include "Light.h"
#include "Registry.h"
#include "Components.h"

//Cursed
#include "SDLHandler.h"
#include "Level.h"

State currentStates;
StateManager stateManager;

void SetInMainMenu(bool value)
{
	if (value)
	{
		currentStates = (State)(currentStates | State::InMainMenu);

		if (currentStates != (State)(currentStates | State::InSettings))
		{
			for (auto entity : View<AudioEngineComponent>(registry))
			{
				SoundComponent* backgroundMusic = registry.GetComponent<SoundComponent>(entity);
				backgroundMusic->Stop(Channel_Base);
				backgroundMusic->Stop(Channel_Extra);
				AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
				audioJungle->HandleSound();
				backgroundMusic->Play(Music_Title, Channel_Base);
			}
		}
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

int StateManager::Setup()
{
	bool loaded = Setup3dGraphics();
	if (!loaded)
	{
		return -1;
	}
	ui.RenderSlot = SetupUIRenderState();

	ui.Setup();

	// Audio Engine VERY IMPORTANT TO LOAD THIS FIRST BEFORE ANY SOUND COMPONENT OR ELSE THINGS WILL GO WHACK!!!!!!!!!!!!!
	EntityID audioJungle = registry.CreateEntity();
	AudioEngineComponent* audioEngine = registry.AddComponent<AudioEngineComponent>(audioJungle);
	audioEngine->Setup(audioJungle.index);

	// Background OST
	SoundComponent* titleTheme = registry.AddComponent<SoundComponent>(audioJungle);
	titleTheme->Load(MUSIC);
	titleTheme->Play(Music_Title, Channel_Base);

	backBufferRenderSlot = SetupGameRenderer();
	currentStates = InMainMenu;
	//models.Initialize();
	Camera::InitializeCamera();
	menu.Setup();

	Particles::InitializeParticles(); // THIS YIELDS MEMORY LEAK UNRELEASED OBJECT
	//SetupTestHitbox();
	RedrawUI();

	//Setup systems here

	// Compute
	systems.push_back(new ParticleSystemGPU());

	// Render/GPU
	systems.push_back(new UIRenderSystem());
	systems.push_back(new ParticleSystemCPU());
	systems.push_back(new RenderSystem());
	
	//Input based CPU 
	systems.push_back(new ButtonSystem());

	//CPU WORK (ORDER IMPORTANT)
	//AI Systems
	systems.push_back(new SkeletonBehaviourSystem());
	systems.push_back(new HellhoundBehaviourSystem());
	systems.push_back(new EyeBehaviourSystem());
	systems.push_back(new TempBossBehaviourSystem());
	//ORDER VERY IMPORTANT
	systems.push_back(new KnockBackSystem());
	systems.push_back(new CollisionSystem()); //Check collision before moving the player (Otherwise last position is wrong)
	systems.push_back(new TransformSystem()); //Must be before controller
	systems.push_back(new ControllerSystem());
	systems.push_back(new EventSystem());//Must be after controller system for correct animations
	systems.push_back(new GeometryIndependentSystem());

	//Damage Over Time (Misc Combat Systems?)
	systems.push_back(new DamageOverTimeSystem());

	//CPU work that can affect rendering
	systems.push_back(new StateSwitcherSystem());
	systems.push_back(new PointOfInterestSystem());

	//Audio (Needs to be close to last)
	systems.push_back(new AudioSystem());

	// Updating UI Elements (Needs to be last)
	systems.push_back(new UIHealthSystem());
	systems.push_back(new UIPlayerSoulsSystem());
	systems.push_back(new UIRelicsSystem());
	systems.push_back(new UIGameLevelSystem());
	systems.push_back(new UIShopSystem());

	return 0;
}


void StateManager::Input()
{
	//First read the keys
	

	//Then go through the registries that are mode
	if (currentStates & State::InMainMenu)
	{
		menu.Input();

		// :)
		//if (keyState[SDL_SCANCODE_RETURN] == pressed)
		//{
		//	//öhö
		//	SetInMainMenu(false);
		//	SetInPlay(true);
		//	SetInShop(false);

		//	menu.Unload();

		//	LoadLevel(++stateManager.activeLevel);

		//	//Ungodly amounts of cursed energy, update UI systems after the level has been loaded
		//	for (size_t i = 17; i < systems.size(); i++)
		//	{
		//		systems[i]->Update();
		//	}
		//}

		
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
		scenes[activeLevelScene].Input();
	}
	if (currentStates & State::InShop)
	{
		scenes[activeLevelScene % 2 == 1].Input(true);
	}
}


void StateManager::Update()
{
	for (size_t i = 0; i < systems.size(); i++)
	{
		if (!systems[i]->Update())
			break;
	}

	Input();
}

void StateManager::UnloadAll()
{
	menu.Unload();
	settings.Unload();
	scenes[0].Unload();
	scenes[1].Unload();
	scenes[2].Unload();
	UnloadEntities(3);
	Particles::ReleaseParticles();
	Light::FreeLight();
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

GameScene& StateManager::GetCurrentLevel()
{
	return scenes[activeLevelScene];
}
