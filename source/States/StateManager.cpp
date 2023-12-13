#include "States\StateManager.h"
#include "Input.h"
#include "Model.h"
#include "Hitbox.h"
#include "Camera.h"
#include "Particles.h"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "UI/UIRenderer.h"
#include "Particles.h"
#include "D3D11Helper\D3D11Graphics.h"
#include "Light.h"
#include "Registry.h"
#include "Components.h"
#include "DeltaTime.h"
#include "RenderDepthPass.h"
#include "OutlineHelper.h"
#include "Glow.h"
#include "AntiAlias.h"
#include "SkyPlane.h"
#include "Shatter.h"
#include "UIComponents.h"

//Cursed
#include "SDLHandler.h"
#include "Level.h"


State currentStates;
StateManager stateManager;

void SetInMainMenu(bool value)
{
	if (value)
	{
		if (currentStates != (State)(currentStates | State::InSettings) && currentStates != (State)(currentStates | State::InCredits)) //All main menu specific states.
		{
			for (auto entity : View<AudioEngineComponent>(registry))
			{
				SoundComponent* backgroundMusic = registry.GetComponent<SoundComponent>(entity);
				backgroundMusic->Stop(Channel_Base);
				backgroundMusic->Stop(Channel_Extra);
				AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
				audioJungle->HandleSound();
				backgroundMusic->Play(Music_Title, Channel_Base);
				audioJungle->HandleSound();
			}
		}

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
		TimedEventIgnoreGamespeed(false);
		gameSpeed = 0.0f;
		Camera::SetOffset(0.0f, 0.0f, 0.0f);//Reset offset to keep camera fromf moving during pause.
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InPause));
		if (Camera::InCutscene() > 0)
		{
			TimedEventIgnoreGamespeed(true);
			gameSpeed = 0.0f;
		}
		
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
		for (auto entity : View<AudioEngineComponent>(registry))
		{
			SoundComponent* backgroundMusic = registry.GetComponent<SoundComponent>(entity);
			backgroundMusic->Stop(Channel_Base);
			backgroundMusic->Stop(Channel_Extra);
			AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
			audioJungle->HandleSound();
			backgroundMusic->Play(Music_Shop, Channel_Base);
			audioJungle->HandleSound();
		}
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InShop));
	}
}
void SetInCredits(bool value)
{

	if (value)
	{
		currentStates = (State)(currentStates | State::InCredits);
	}
	else
	{
		currentStates = (State)(currentStates & (~State::InCredits));
	}
}

int StateManager::Setup()
{
#ifdef _DEBUG
	visualizeStage = true;
#endif
	bool loaded = Setup3dGraphics();
	if (!loaded)
	{
		return -1;
	}
	ui.RenderSlot = SetupUIRenderState();

	ui.Setup();

	// Audio Engine VERY IMPORTANT TO LOAD THIS FIRST BEFORE ANY SOUND COMPONENT OR ELSE THINGS WILL GO WHACK!!!!!!!!!!!!!
	EntityID audioJungle = registry.CreateEntity(ENT_PERSIST_GAME);
	AudioEngineComponent* audioEngine = registry.AddComponent<AudioEngineComponent>(audioJungle);
	audioEngine->Setup(audioJungle.index);

	// Background OST
	SoundComponent* titleTheme = registry.AddComponent<SoundComponent>(audioJungle);
	titleTheme->Load(MUSIC);

	backBufferRenderSlot = SetupGameRenderer();
	currentStates = InMainMenu;
	Camera::InitializeCamera();
	SetupHitboxVisualizer();
	menu.Setup();
	Shatter::Initialize();

	Particles::InitializeParticles();
	Outlines::InitializeOutlines();
	Glow::Initialize();
	InitializeSky();
	//SetupTestHitbox();
	RedrawUI();
	
	//Setup cursor here:
	stateManager.cursor = registry.CreateEntity(ENT_PERSIST_GAME);
	UIComponent* uiElement2 = registry.AddComponent<UIComponent>(stateManager.cursor);
	uiElement2->Setup("Cursor/DamnedSoul_CursorSmallBorder", "", "", DSFLOAT2(0.0f, 0.0f), DSFLOAT2(1.0f, 1.0f));
	uiElement2->m_BaseImage.baseUI.SetVisibility(true);
	uiElement2->m_BaseText.baseUI.SetVisibility(false);
	//uiElement2->m_BaseImage.baseUI.SetPosition();

	//Setup systems here

	// Compute
	systems.push_back(new ParticleSystemGPU());

	// Render/GPU
	systems.push_back(new ShadowSystem());
	systems.push_back(new RenderSystem());
	systems.push_back(new OutlineSystem());
	systems.push_back(new ParticleSystemCPU());
	systems.push_back(new ParticleSystem());
	systems.push_back(new GlowSystem());

	systems.push_back(new ShatterSystem());
#define CAPS
#ifdef CAPS
	systems[1]->timeCap = 1.f / 60.f;//ShadowSystem
	systems[2]->timeCap = 1.f / 60.f;//RenderSystem
	systems[4]->timeCap = 1.f / 60.f;//ParticleSystemCPU
	systems[6]->timeCap = 1.f / 60.f;//GlowSystem

#endif // _DEBUG

	systems.push_back(new UIGameSystem());
	systems.push_back(new UIRenderSystem());
	
	//Input based CPU 
	systems.push_back(new OnClickSystem());
	systems.push_back(new OnHoverSystem());

	// After all render passes, reset model colors
	systems.push_back(new ColorResetSystem());

	// Stat Calculatoins
	systems.push_back(new StatCalcSystem()); // Should be before behaviours and controllers so that the correct stats are applied

	//CPU WORK (ORDER IMPORTANT)
	//AI Systems
	systems.push_back(new SkeletonBehaviourSystem());
	systems.push_back(new HellhoundBehaviourSystem());
	systems.push_back(new EyeBehaviourSystem());
	systems.push_back(new MinotaurBehaviourSystem());
	systems.push_back(new TempBossBehaviourSystem());
	systems.push_back(new FrozenBehaviourSystem());
	systems.push_back(new LuciferBehaviourSystem());
	systems.push_back(new ProjectileSystem());
	
	//ORDER VERY IMPORTANT
	systems.push_back(new KnockBackSystem());
	systems.push_back(new CollisionSystem()); //Check collision before moving the player (Otherwise last position is wrong)
	systems.push_back(new ImpBehaviourSystem()); //Imp behavior needs to come after collision
	systems.push_back(new ZacBehaviourSystem());
	systems.push_back(new TransformSystem()); //Must be before controller
	systems.push_back(new FollowerSystem());
	systems.push_back(new ControllerSystem());
	systems.push_back(new EventSystem());//Must be after controller system for correct animations
	systems.push_back(new GeometryIndependentSystem());

	//Damage Over Time (Misc Combat Systems?)
	systems.push_back(new DebuffSystem());

	//CPU work that can affect rendering
	systems.push_back(new StateSwitcherSystem());
	systems.push_back(new PointOfInterestSystem());

	//Audio (Needs to be close to last)
	systems.push_back(new StageVoiceLineSystem());
	systems.push_back(new AudioSystem());

	// Updating UI Elements (Needs to be last)
	systems.push_back(new UIHealthSystem());
	systems.push_back(new UIPlayerSoulsSystem());
	systems.push_back(new UISliderSystem());
	
	systems.push_back(new UIShopSystem());
	systems.push_back(new NavigationSystem());

	return 0;
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
		scenes[activeLevelScene].Input();
	}
	if (currentStates & State::InShop)
	{
		scenes[activeLevelScene % 2 == 1].Input(true);
	}
	if (currentStates & State::InCredits)
	{
		credits.Input();
	}
}

void StateManager::Update()
{
	//Reset input values first
	float previousMouseX = mouseX;
	float previousMouseY = mouseY;
	ResetInput();
	GetInput();
	UIComponent* mouse = registry.GetComponent<UIComponent>(stateManager.cursor);
	mouse->m_BaseImage.baseUI.SetPosition((int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)) - 1, (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) - 1);

	
	if (mouseX != previousMouseX || mouseY != previousMouseY)
	{
		RedrawUI();
	}

	
	if (mouseButtonPressed[left] != ButtonState::noEvent || mouseButtonPressed[right] != ButtonState::noEvent)
	{
		if (mouseButtonDown[MouseButton::left] == down)
		{//uiElement2->Setup("Cursor/DamnedSoul_CursorSmall", "", DSFLOAT2(0.0f, 0.0f), DSFLOAT2(1.0f, 1.0f));
			mouse->m_BaseImage.SetImage("Cursor/DamnedSoul_CursorLightSmallBorder");//Brighten up
		}
		else if (mouseButtonDown[MouseButton::left] == up)
		{
			mouse->m_BaseImage.SetImage("Cursor/DamnedSoul_CursorSmallBorder");//Reset the mouse
		}
		if (mouseButtonDown[MouseButton::right] == down)
		{
			mouse->m_BaseImage.SetImage("Cursor/DamnedSoul_CursorSparkleSmallBorder");
		}
		RedrawUI();
	}

	for (size_t i = 0; i < systems.size(); i++)
	{
		systems[i]->timeElapsed += GetFrameTime(); //No longer deltatime, in case of game pause deltatime
		
		if (systems[i]->timeElapsed >= systems[i]->timeCap)
		{
			systems[i]->Update();
			if (systems.size() == 0)
				break;
			systems[i]->timeElapsed -= systems[i]->timeCap;
		}
	}

	Input();

}

void StateManager::UnloadAll()
{
	/*menu.Unload();
	settings.Unload();
	scenes[0].Unload();
	scenes[1].Unload();
	scenes[2].Unload();*/
	UnloadEntities(ENT_PERSIST_HIGHEST);
	Particles::ReleaseParticles();
	Light::FreeLight();
	ReleaseUIRenderer();
	ui.Release();
	ReleaseDepthPass();
	EndDirectX();
}

void StateManager::EndFrame()
{
	Present();//Present what was drawn during the update!
	
	//MemLib::pdefrag();
}

GameScene& StateManager::GetCurrentLevel()
{
	return scenes[activeLevelScene];
}