#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "Levels\LevelHelper.h"
#include "States/StateManager.h"
#include "UIButtonFunctions.h"
#include "UIRenderer.h"
#include "UIComponents.h"
#include "Model.h"
#include "States/StateEnums.h"
#include "Camera.h"
#include "EventFunctions.h"

#define SHOP_POSITION_X (-0.4f)
#define SHOP_OFFSET_X_Left (SHOP_POSITION_X - 0.225f)
#define SHOP_OFFSET_X_Right (SHOP_POSITION_X + 0.225f)

#define SHOP_POSITION_Y (0.55f)

#define SHOP_RELIC_WINDOWS (3)
#define SHOP_SINGLE_WINDOWS (5)

void ShopCutscene()
{
	//Create the imp
	EntityID imp = registry.CreateEntity();
	stateManager.cutsceneEnemy = imp;
	registry.AddComponent<ModelSkeletonComponent>(imp, LoadModel("Imp.mdl"));
	registry.AddComponent<AnimationComponent>(imp);
	TransformComponent* transform = registry.AddComponent<TransformComponent>(imp);
	transform->positionX = 13.0f;
	transform->positionY = 3.5f;
	transform->positionZ = -25.0f;
	transform->scaleY = 1.2f;
	transform->scaleX = 1.2f;
	transform->scaleZ = 1.2f;

	float impY = 0.0f;
	float impZ = -1.0f;

	EntityID impCutscene = registry.CreateEntity();
	CutsceneComponent* rotateImp = registry.AddComponent<CutsceneComponent>(imp);
	rotateImp->mode = (CutsceneMode)(Cutscene_Character_Idle | Transition_LookAt | Cutscene_Accelerating);
	CutsceneSetLookAt(imp, 0.0f, impY, -1.0f, -1.0f, impY, impZ);
	AddTimedEventComponentStartContinuousEnd(imp, 0.0f, BeginShopCutscene, CutsceneTransition, 5.0f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Keep Imp idle
	EntityID impIdleCutscene = registry.CreateEntity();
	CutsceneComponent* idleImp = registry.AddComponent<CutsceneComponent>(impIdleCutscene);
	idleImp->mode = (CutsceneMode)(Cutscene_Character_Idle | Transition_LookAt | Cutscene_Linear);
	CutsceneSetLookAt(impIdleCutscene, -1.0f, impY, impZ, -1.0f, impY, impZ);
	AddTimedEventComponentStartContinuousEnd(impIdleCutscene, 5.0f, BeginShopCutscene, StoredEnemyCutscene, 99999999999.0f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//Cutscene
	EntityID cutscene = registry.CreateEntity();
	CutsceneComponent* stillShot = registry.AddComponent<CutsceneComponent>(cutscene);
	stillShot->mode = (CutsceneMode)(Cutscene_Camera | Transition_LookAt | Transition_Position | Cutscene_Linear);
	CutsceneSetLookAt(cutscene, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	CutsceneSetPosition(cutscene, CAMERA_OFFSET_X, CAMERA_OFFSET_Y * 0.0f, CAMERA_OFFSET_Z * 0.5f, CAMERA_OFFSET_X, CAMERA_OFFSET_Y * 0.0f, CAMERA_OFFSET_Z * 0.5f);
	AddTimedEventComponentStartContinuousEnd(cutscene, 0.0f, BeginShopCutscene, CutsceneTransition, 99999999999999.0f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Move player
	CutsceneComponent* playerFallIn = registry.AddComponent<CutsceneComponent>(stateManager.player);
	playerFallIn->mode = (CutsceneMode)(Cutscene_Character_Fall | Transition_Position | Cutscene_Decelerating);
	CutsceneSetPosition(stateManager.player, 7.0f, 18.0f, 0.0f, 7.0f, 5.0f, 0.0f);
	AddTimedEventComponentStartContinuousEnd(stateManager.player, 0.0f, BeginShopCutscene, CutsceneTransition, 2.0f, LoopCutscenePlayerFallInPlace, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	StatComponent* stats = registry.GetComponent<StatComponent>(stateManager.player);
	stats->SetSpeedMult(0.0f);
};

void CreateUIRelics(UIComponent& uiComp, UIShopRelicComponent& uiRelicComp, const Relics::RELIC_TYPE& type, DSFLOAT2 pos)
{
	ML_Array<float, 2> xPos;
	xPos[0] = pos.x - 0.1f;
	xPos[1] = pos.x + 0.1f;

	float yPos = pos.y;

	for (int i = 0; i < 2; i++)
	{
		const RelicData* relic = Relics::PickRandomRelic(type);
		uiComp.AddImage(relic->m_filePath, { xPos[i], yPos }, { 1.5f, 1.5f }, false);
		char price[4];
		sprintf(price, "%i", relic->m_price);

		uiComp.AddText(price, uiComp.m_Images[uiComp.m_Images.size() - 1].baseUI.GetBounds(), {xPos[i], yPos - 0.075f});
		uiRelicComp.shopRelics[i] = relic;
	}

	uiComp.AddImage("RelicIcons\\HoverRelic", { 0.0f, 0.0f }, { 1.5f, 1.5f }, false);
	uiComp.AddImage("RelicIcons\\HoverRelic", { 0.0f, 0.0f }, { 1.5f, 1.5f }, false);
	uiComp.m_Images[uiComp.m_Images.size() - 2].baseUI.SetVisibility(false);
	uiComp.m_Images[uiComp.m_Images.size() - 1].baseUI.SetVisibility(false);

};


void CreateRelicWindows()
{
	const char texts[SHOP_RELIC_WINDOWS][32] =
	{
		"Offence",
		"Defence",
		"Gadget"
	};

	const DSFLOAT2 positions[SHOP_RELIC_WINDOWS] =
	{
		{ SHOP_OFFSET_X_Left, SHOP_POSITION_Y - 0.3f },
		{ SHOP_OFFSET_X_Left, SHOP_POSITION_Y - 0.75f },
		{ SHOP_OFFSET_X_Right, SHOP_POSITION_Y - 0.75f }
	};

	const Relics::RELIC_TYPE type[SHOP_RELIC_WINDOWS] =
	{
		Relics::RELIC_OFFENSE,
		Relics::RELIC_DEFENSE,
		Relics::RELIC_GADGET
	};

	for (int i = 0; i < SHOP_RELIC_WINDOWS; i++)
	{
		EntityID relicWindow = registry.CreateEntity();
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);
		uiElement->Setup("ExMenu/PanelSmall", texts[i], positions[i], DSFLOAT2(1.0f, 1.0f), 20.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		uiElement->m_BaseText.baseUI.SetPosition({ positions[i].x, positions[i].y - 0.05f});
		
		UIShopRelicComponent* uiRelicWindow = registry.AddComponent<UIShopRelicComponent>(relicWindow);

		CreateUIRelics(*uiElement, *uiRelicWindow, type[i], positions[i]);

		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);
		OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);

		uiOnClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnClick::None, UIFunctions::OnClick::None);
		uiOnClick->Add(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::OnClick::SelectRelic, UIFunctions::OnClick::None);
		uiOnClick->Add(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunctions::OnClick::SelectRelic, UIFunctions::OnClick::None);

		uiOnHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::None);
		uiOnHover->Add(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::OnHover::ShopRelic);
		uiOnHover->Add(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunctions::OnHover::ShopRelic);

		SoundComponent* sfx = registry.AddComponent<SoundComponent>(relicWindow);
		sfx->Load(SHOP);
	}

};

void CreateSingleWindows()
{

	const char texts[SHOP_SINGLE_WINDOWS][32] =
	{
		"Heal",
		"Reroll",
		"Buy",
		"Weapon",
		"Next Level"
	};

	const DSFLOAT2 positions[SHOP_SINGLE_WINDOWS] =
	{
		{ SHOP_OFFSET_X_Left, SHOP_POSITION_Y - 1.25f },
		{ SHOP_OFFSET_X_Left + 0.22f, SHOP_POSITION_Y - 1.25f  },
		{ SHOP_OFFSET_X_Left + 0.44f, SHOP_POSITION_Y - 1.25f  },
		{ SHOP_OFFSET_X_Right, SHOP_POSITION_Y - 0.3f },
		{ 0.8f, -0.75f }
	};

	const char filenames[SHOP_SINGLE_WINDOWS][32] =
	{
		"Heal",
		"Reroll",
		"Buy",
		"AxeMedium",
		""
	};

	void(*const functions[SHOP_SINGLE_WINDOWS])(void*, int)  =
	{
		UIFunctions::OnClick::HealPlayer,
		UIFunctions::OnClick::RerollRelic,
		UIFunctions::OnClick::BuyRelic,
		UIFunctions::OnClick::UpgradeWeapon,
		UIFunctions::Game::ExitShopCutscene
	};

	const char name[SHOP_SINGLE_WINDOWS][32] =
	{
		"Heal",
		"Reroll",
		"Buy",
		"Upgrade Weapon",
		""
	};

	const char description[SHOP_SINGLE_WINDOWS][64] =
	{
		"Recover 25% of max Health",
		"Reroll a new set of relics",
		"Buy the selected relic",
		"Upgrade your weapon",
		"Leave the shop"
	};

	uint8_t price[SHOP_SINGLE_WINDOWS] =
	{
		2,
		5,
		0,
		10,
		0,
	};


	for (int i = 0; i < SHOP_SINGLE_WINDOWS; i++)
	{
		EntityID relicWindow = registry.CreateEntity(ENT_PERSIST_LEVEL);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);

		if (i == 3)
		{
			uiElement->Setup("ExMenu/PanelSmall", texts[i], positions[i], DSFLOAT2(1.0f, 1.0f), 20.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		}
		else if (i == 4)
		{
			uiElement->Setup("ExMenu/ButtonSmall", texts[i], positions[i], DSFLOAT2(1.0f, 1.0f), 25.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
		else
			uiElement->Setup("ExMenu/ButtonSuperSmall", texts[i], positions[i], DSFLOAT2(1.0f, 1.0f), 25.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		if (i != 4)
		{
			uiElement->m_BaseText.baseUI.SetPosition({ positions[i].x, positions[i].y - 0.05f });
			uiElement->AddImage(filenames[i], { positions[i].x, positions[i].y - 0.0f }, { 1.0f, 1.0f }, false);
			if (price[i] > 0)
			{
				char Sprice[4];
				sprintf(Sprice, "%i", price[i]);

				uiElement->AddText(Sprice, uiElement->m_Images[uiElement->m_Images.size() - 1].baseUI.GetBounds(), { positions[i].x, positions[i].y - 0.1f });
			}
		}

		if (i == 4)
			uiElement->m_BaseImage.baseUI.SetVisibility(false);

		if (i == 0)
			registry.AddComponent<UIShopHealComponent>(relicWindow);
		else if (i == 1)
			registry.AddComponent<UIShopRerollComponent>(relicWindow);
		else if (i == 3)
			registry.AddComponent<UIShopUpgradeComponent>(relicWindow);


		UIShopButtonComponent* button = registry.AddComponent<UIShopButtonComponent>(relicWindow);
		button->Setup(name[i], description[i], price[i]);

		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);

		if (i != 4)
		{
			uiOnClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);
			uiOnClick->Add(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);
		}
		else
			uiOnClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);


		OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);
		uiOnHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::ShopButton);

		SoundComponent* sfx = registry.AddComponent<SoundComponent>(relicWindow);
		sfx->Load(SHOP);

	}
}

void CreateTextWindows()
{
	EntityID shopTitle = registry.CreateEntity();
	EntityID impText = registry.CreateEntity();
	EntityID statsText = registry.CreateEntity();

	UIComponent* uiTitle = registry.AddComponent<UIComponent>(shopTitle);
	uiTitle->Setup("ExMenu/ButtonMedium", "Lil\' Devil\'s Shop", { SHOP_POSITION_X, SHOP_POSITION_Y }, DSFLOAT2(1.0f, 1.0f), 25.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	registry.AddComponent<UIShopTitleImpComponent>(shopTitle);

	UIComponent* uiImpText = registry.AddComponent<UIComponent>(impText);
	uiImpText->Setup("ExMenu/PanelMediumShort", "Hello There", { 0.5f, 0.0f }, { 1.0f, 1.0f }, 20.0f);
	
	uiImpText->m_BaseText.baseUI.m_CurrentBounds.right *= 0.6;
	uiImpText->m_BaseText.baseUI.m_PositionBounds.right *= 0.6;
	uiImpText->m_BaseText.baseUI.m_OriginalBounds.right *= 0.6;

	uiImpText->m_BaseText.baseUI.SetPosition(DSFLOAT2(uiImpText->m_BaseImage.baseUI.GetPosition().x, 0.0f));

	registry.AddComponent<UIShopImpComponent>(impText);

	UIComponent* uiPlayerInfo = registry.AddComponent<UIComponent>(statsText);
	uiPlayerInfo->Setup("ExMenu/PanelSmall", "Player Stats", { 0.3f,  SHOP_POSITION_Y - 1.25f }, { 1.0f, 1.0f }, 20.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	uiPlayerInfo->m_BaseText.baseUI.SetPosition(DSFLOAT2(uiPlayerInfo->m_BaseImage.baseUI.GetPosition().x, uiPlayerInfo->m_BaseImage.baseUI.GetPosition().y - 0.05f));

	float offsets[3] = { 0.06f, 0.0f, -0.06f };

	for (int i = 0; i < 3; i++)
	{
		uiPlayerInfo->AddText("", uiPlayerInfo->m_BaseImage.baseUI.GetBounds(),
			DSFLOAT2(uiPlayerInfo->m_BaseImage.baseUI.GetPosition().x, uiPlayerInfo->m_BaseImage.baseUI.GetPosition().y + offsets[i] - 0.025f));
	}


	registry.AddComponent<UIShopPlayerStatsComponent>(statsText);

}

void LoadShop()
{
	CreateTextWindows();

	CreateRelicWindows();

	CreateSingleWindows();

	SetInShop(true);

	for (auto entity : View<OnClickComponent>(registry))
	{
		OnClickComponent* shopBuy = registry.GetComponent<OnClickComponent>(entity);

		if (shopBuy != nullptr)
		{
			for (int i = 0; i < (int)shopBuy->onClickFunctionsPressed.size(); i++)
			{
				if (shopBuy->onClickFunctionsPressed[i] == UIFunctions::OnClick::BuyRelic) //Purchase button found, play the first imp voice line.
				{
					SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
					if (sfx != nullptr) sfx->Play(Shop_FirstMeet, Channel_Extra);
				}
			}
		}
	}

	ShopCutscene();
	

}

void ReloadShop()
{
	CreateTextWindows();

	CreateRelicWindows();

	SetInShop(true);

	for (auto entity : View<UIShopRerollComponent>(registry))
	{
		UIFunctions::OnClick::RerollRelic((void*)&entity, -1);
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		if (sfx != nullptr) sfx->Stop(Channel_Base);
	}

	for (auto entity : View<OnClickComponent>(registry))
	{
		OnClickComponent* shopBuy = registry.GetComponent<OnClickComponent>(entity);
		if (shopBuy != nullptr)
		{
			for (int i = 0; i < (int)shopBuy->onClickFunctionsPressed.size(); i++)
			{
				if (shopBuy->onClickFunctionsPressed[i] == UIFunctions::OnClick::BuyRelic) //Purchase button found, play the correct sound based on the level.
				{
					SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
					if (sfx != nullptr)
					{
						switch (stateManager.activeLevel)
						{
						case 4:
							sfx->Play(Shop_BeforeLava, Channel_Extra);
							break;
						case 6:
							sfx->Play(Shop_BeforeSplitBoss, Channel_Extra);
							break;
						case 8:
						{
							StatComponent* currentStats = registry.GetComponent<StatComponent>(stateManager.player);
							if (currentStats != nullptr)
							{
								if (currentStats->GetHealthFraction() <= 0.25)
								{
									sfx->Play(Shop_LowHealth, Channel_Extra);
								}
							}
							break;
						}
						case 10:
						{
							StatComponent* currentStats = registry.GetComponent<StatComponent>(stateManager.player);
							if (currentStats != nullptr)
							{
								if (currentStats->GetHealthFraction() <= 0.25)
								{
									sfx->Play(Shop_LowHealth, Channel_Extra);
								}
							}
							break;
						}
						case 12:
							sfx->Play(Shop_BeforeIce, Channel_Extra);
							break;
						case 14:
						{
							StatComponent* currentStats = registry.GetComponent<StatComponent>(stateManager.player);
							if (currentStats != nullptr)
							{
								if (currentStats->GetHealthFraction() <= 0.25)
								{
									sfx->Play(Shop_LowHealth, Channel_Extra);
								}
							}
							break;
						}
						case 16:
							sfx->Play(Shop_BeforeLastBoss, Channel_Extra);
							break;
						}
					}
				}
			}
		}
	}

	ShopCutscene();

}

