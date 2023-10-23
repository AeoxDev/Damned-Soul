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

#include "MemLib\ML_String.hpp"

#include "Level.h"

// Relic Stuff
#include "RelicFunctions.h"

void GameScene::Input(bool isShop)
{
	if (isShop)
	{
		if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
		{
			SetInMainMenu(true);
			SetInPlay(false);
			SetInShop(false);
			Unload();
			stateManager.menu.Setup();
		}

		if (keyState[SDL_SCANCODE_RETURN] == pressed)
		{
			SetInMainMenu(false);
			SetInPlay(true);
			SetInShop(false);
			Unload();
			LoadLevel(++stateManager.activeLevel);
		}
	}
	else
	{
		if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
		{
			SetInMainMenu(true);
			SetInPlay(false);
			SetInShop(false);
			Unload();
			stateManager.menu.Setup();
		}
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

void GameScene::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelBonelessComponent* modBonelessC = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(modBonelessC->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		ModelSkeletonComponent* modSkelC = registry.GetComponent<ModelSkeletonComponent>(entity);
		//Persistent Player Baby
		PlayerComponent* p = registry.GetComponent<PlayerComponent>(entity);
		if (!p)
			ReleaseModel(modSkelC->model); // Decrement and potentially release via refcount
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

	for (auto entity : View<UIPlayerRelicsComponent>(registry))
	{
		UIPlayerRelicsComponent* r = registry.GetComponent<UIPlayerRelicsComponent>(entity);
		r->baseImage.Release();

		for (uint32_t i = 0; i < r->relics.size(); i++)
		{
			r->relics[i].sprite.Release();
			r->relics[i].flavorTitleImage.Release();
		}

		r->relics.~ML_Vector();
	}

	for (auto entity : View<UIShopComponent>(registry))
	{
		UIShopComponent* sh = registry.GetComponent<UIShopComponent>(entity);
		sh->baseImage.Release();
	}

	for (auto entity : View<UIShopRelicWindowComponent>(registry))
	{
		UIShopRelicWindowComponent* sh = registry.GetComponent<UIShopRelicWindowComponent>(entity);
		sh->m_baseImage.Release();
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

	for (auto entity : View<ProximityHitboxComponent>(registry))
	{
		ProximityHitboxComponent* p = registry.GetComponent<ProximityHitboxComponent>(entity);
		p->pointList.~ML_Vector();
	}

	for (auto entity : View<UIRelicComponent>(registry))
	{
		UIRelicComponent* r = registry.GetComponent<UIRelicComponent>(entity);
		r->sprite.Release();
		r->flavorTitleImage.Release();
		r->flavorDescImage.Release();
	}

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		EntityID check = registry.entities.at(i).id;
		if (check.state == false)
		{
			//Persistent Player Baby
			PlayerComponent* p = registry.GetComponent<PlayerComponent>(check);
			if (!p)
			{
				//Remove everything that isn't player
				registry.DestroyEntity(check);
			}
			else
			{
				//Reset necessary player variables
				p->portalCreated = false;
				p->killingSpree = 0;
			}
				
		}
	}
}

void GameScene::GameOver()
{
	SetInMainMenu(true);
	SetInPlay(false);
	Unload();
	Relics::ClearRelicFunctions();
	stateManager.menu.Setup();
}
