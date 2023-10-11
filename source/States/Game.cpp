#include "States\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper.h"
#include "Particles.h"
#include "Components.h"
#include "Input.h"
#include "States\StateManager.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "CollisionFunctions.h"
#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Camera.h"

void GameScene::Setup(int scene)//Load
{
	RedrawUI();	
	if (scene == 0)
	{
		// Set active
		m_active = true;
		
		//Setup Game HUD

		Camera::ResetCamera();

		//Doggo
		//EntityID gameLevel = registry.CreateEntity();
		//EntityID dog = registry.CreateEntity();
		//EntityID stage = registry.CreateEntity();
		//EntityID player = registry.CreateEntity();
		//EntityID skeleton = registry.CreateEntity();
		//EntityID skeleton2 = registry.CreateEntity();
		//EntityID particle = registry.CreateEntity();

		///*ModelBonelessComponent* dogCo = */registry.AddComponent<ModelBonelessComponent>(dog, LoadModel("HellhoundDummy_PH.mdl"));
		///*ModelBonelessComponent* stageCo = */registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
		///*ModelSkeletonComponent* pmc = */registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
		///*AnimationComponent* pac = */registry.AddComponent<AnimationComponent>(player, AnimationComponent());
		///*ModelBonelessComponent* skelCo = */registry.AddComponent<ModelBonelessComponent>(skeleton, LoadModel("SkeletonOneDymmy.mdl"));
		///*ModelBonelessComponent* skelCo2 = */registry.AddComponent<ModelBonelessComponent>(skeleton2, LoadModel("SkeletonOneDymmy.mdl"));

		//// Dog
		//TransformComponent dogTransformComponent;
		//dogTransformComponent.facingX = 1.0f; dogTransformComponent.positionX = 20.0f; dogTransformComponent.facingX = 1.0f;
		///*TransformComponent* dtc = */registry.AddComponent<TransformComponent>(dog, dogTransformComponent);
		//// Stage (Default)
		///*TransformComponent* stc = */registry.AddComponent<TransformComponent>(stage);
		//// Player (Default)
		///*TransformComponent* ptc = */registry.AddComponent<TransformComponent>(player);
		//// First skeleton
		//TransformComponent fsTransformComponent;
		//fsTransformComponent.positionZ = 20.0f;
		///*TransformComponent* skeltc = */registry.AddComponent<TransformComponent>(skeleton, fsTransformComponent);
		//// Second skeleton
		//TransformComponent ssTransformComponent;
		//ssTransformComponent.positionZ = 15.0f;
		///*TransformComponent* skeltc2 = */registry.AddComponent<TransformComponent>(skeleton2, ssTransformComponent);



		///*StatComponent* ps = */registry.AddComponent<StatComponent>(player, 125.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
		///*PlayerComponent* pc = */registry.AddComponent<PlayerComponent>(player);

		///*StatComponent* ds = */registry.AddComponent<StatComponent>(dog, 50.f, 20.f, 25.f, 5.f);
		///*EnemyComponent* ec1 = */registry.AddComponent<EnemyComponent>(dog, 1);

		///*StatComponent* ss = */registry.AddComponent<StatComponent>(skeleton, 100.f, 10.f, 25.f, 5.f);
		///*EnemyComponent* ec2 = */registry.AddComponent<EnemyComponent>(skeleton, 2);

		///*StatComponent* ss2 = */registry.AddComponent<StatComponent>(skeleton2, 100.f, 10.f, 25.f, 5.f);
		///*EnemyComponent* ec3 = */registry.AddComponent<EnemyComponent>(skeleton2, 2);

		///*ControllerComponent* cc = */registry.AddComponent<ControllerComponent>(player);

		//		ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
		//PointOfInterestComponent poic;
		//poic.weight = 10.0f;
		///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);
		///*PointOfInterestComponent* dogPoi = */registry.AddComponent<PointOfInterestComponent>(dog);
		///*PointOfInterestComponent* skelPoi = */registry.AddComponent<PointOfInterestComponent>(skeleton);
		///*PointOfInterestComponent* skelPoi2 = */registry.AddComponent<PointOfInterestComponent>(skeleton2);

		UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
		pcUiHpC->image.Setup("ExMenu/FullHealth.png");
		pcUiHpC->text.Setup(L"");

		UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f));
		pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
		pcUiSC->text.Setup(L"");

		//Thing in the top right corner showing what level we're on
		std::string valueAsString = std::to_string(scene);
		std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());
		UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(gameLevel, DirectX::XMFLOAT2(0.9f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f));
		gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
		gameLevelUIc->text.Setup(valueAsWString);

		/*HellhoundBehaviour* hellhoundBehevCo = */registry.AddComponent<HellhoundBehaviour>(dog);
		/*SkeletonBehaviour* skeletonBehevCo = */registry.AddComponent<SkeletonBehaviour>(skeleton);
		/*SkeletonBehaviour* skeletonBehevCo2 = */registry.AddComponent<SkeletonBehaviour>(skeleton2);


		//RenderGeometryIndependentCollision(stage);
		////Finally set the collision boxes
		//SetupPlayerCollisionBox(player, 1.0f);

		//SetupEnemyCollisionBox(skeleton, 0.9f);
		//SetupEnemyCollisionBox(skeleton2, 0.9f);
		//SetupEnemyCollisionBox(dog, 1.0f);
	}
}

void GameScene::Input()
{
	if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
	{
		SetInMainMenu(true);
		SetInPlay(false);
		Unload();
		stateManager.menu.Setup();
	}
}

void GameScene::Update()
{
	
}

void GameScene::ComputeShaders()
{
	/*Particles::PrepareParticleCompute();
	Dispatch(100, 0, 0);
	Particles::FinishParticleCompute();*/
}

void GameScene::SetupText()
{
	auto t1 = registry.CreateEntity();
	auto tc1 = registry.AddComponent<UIText>(t1);
	tc1->Setup(L"This is the HUD!", { 0.0f, 0.6f });

	auto t2 = registry.CreateEntity();
	auto tc2 = registry.AddComponent<UIText>(t2);
	tc2->Setup(L"Press ECS to return to main menu!", { 0.0f, 0.4f });
}

void GameScene::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelBonelessComponent* dogCo = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<UIPlayerSoulsComponent>(registry))
	{
		UIPlayerSoulsComponent* ps = registry.GetComponent<UIPlayerSoulsComponent>(entity);
		ps->image.Release();
	}

	for (auto entity : View<UIGameLevelComponent>(registry))
	{
		UIGameLevelComponent* ps = registry.GetComponent<UIGameLevelComponent>(entity);
		ps->image.Release();
	}

	for (auto entity : View<UIHealthComponent>(registry))
	{
		UIHealthComponent* ph = registry.GetComponent<UIHealthComponent>(entity);
		ph->image.Release();
	}

	for (auto entity : View<UIButton>(registry))
	{
		UIButton* b = registry.GetComponent<UIButton>(entity);
		b->Release();
	}

	for (auto entity : View<UIImage>(registry))
	{
		UIImage* i = registry.GetComponent<UIImage>(entity);
		i->Release();
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		registry.DestroyEntity({ i, false });
	}
}

void GameScene::GameOver()
{
	SetInMainMenu(true);
	SetInPlay(false);
	Unload();
	stateManager.menu.Setup();
}
