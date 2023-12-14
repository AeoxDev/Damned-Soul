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

#define SHOP_POSITION_X (-0.25f)
#define SHOP_POSITION_Y (0.65f)

#define SHOP_OFFSET_X_LEFT_RELICS (SHOP_POSITION_X - 0.265f)
#define SHOP_OFFSET_X_RIGHT_RELICS (SHOP_POSITION_X + 0.2925f)

#define SHOP_OFFSET_Y_UP_RELICS (SHOP_POSITION_Y - 0.35f)
#define SHOP_OFFSET_Y_DOWN_RELICS (SHOP_POSITION_Y - 0.8325f)

#define SHOP_OFFSET_X_RIGHT_WEAPON (SHOP_POSITION_X + 0.29f)
#define SHOP_OFFSET_Y_UP_WEAPON (SHOP_POSITION_Y - 0.4f)

#define SHOP_OFFSET_X_LEFT_HEAL (SHOP_POSITION_X - 0.405f)
#define SHOP_OFFSET_X_RIGHT_REROLL (SHOP_POSITION_X - 0.15f)

#define SHOP_OFFSET_Y_DOWN_BUTTONS (SHOP_POSITION_Y - 1.475f)

#define SHOP_OFFSET_X_RIGHT_STATS (SHOP_POSITION_X + 0.2725f)
#define SHOP_OFFSET_Y_DOWN_STATS (SHOP_POSITION_Y - 1.35f)


#define SHOP_RELIC_WINDOWS (3)
#define SHOP_SINGLE_WINDOWS (4)

void ShopCutscene()
{
	//Create the imp
	EntityID imp = registry.CreateEntity();
	stateManager.cutsceneEnemy = imp;
	ModelSkeletonComponent* model = registry.AddComponent<ModelSkeletonComponent>(imp, LoadModel("Imp.mdl"));
	model->shared.hasOutline = true;
	model->shared.colorMultiplicativeRed *= .9f;
	model->shared.colorAdditiveGreen *= 1.1f;
	registry.AddComponent<AnimationComponent>(imp);
	registry.AddComponent<GlowComponent>(imp, 1.375f, .715f, .885f);
	TransformComponent* transform = registry.AddComponent<TransformComponent>(imp);
	transform->positionX = 14.0f;
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
	CutsceneSetPosition(stateManager.player, 11.0f, 19.0f, 0.0f, 11.0f, 5.0f, 0.0f);
	AddTimedEventComponentStartContinuousEnd(stateManager.player, 0.0f, BeginShopCutscene, CutsceneTransition, 2.0f, LoopCutscenePlayerFallInPlace, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	StatComponent* stats = registry.GetComponent<StatComponent>(stateManager.player);
	stats->SetSpeedMult(0.0f);
};

void CreateUIRelics(UIComponent& uiComp, UIShopRelicComponent& uiRelicComp, const Relics::RELIC_TYPE& type, DSFLOAT2 pos)
{
	ML_Array<float, 2> xPos;
	xPos[0] = pos.x - 0.1f;
	xPos[1] = pos.x + 0.085f;
	float yPos = pos.y - 0.05f;

	float relicScale = 3.0f;

	float soulPos = yPos - 0.175f;

	for (int i = 0; i < 2; i++)
	{
		const RelicData* relic = Relics::PickRandomRelic(type);
		uiComp.AddImage(relic->m_filePath, { xPos[i], yPos }, { relicScale, relicScale }, false);
		char price[4];
		sprintf(price, "%i", relic->m_price);

		uiComp.AddText(price, uiComp.m_Images[uiComp.m_Images.size() - 1].baseUI.GetBounds(), { xPos[i] + 0.015f, soulPos });
		uiComp.AddImage("Soul_Stone", { xPos[i], soulPos }, { 1.0f, 1.0f }, false);
		uiComp.AddImage("RelicIcons\\HoverRelic", { 0.0f, 0.0f }, { relicScale, relicScale }, false);
		uiComp.m_Images[uiComp.m_Images.size() - 1].baseUI.SetVisibility(false);

		uiRelicComp.shopRelics[i] = relic;
	}
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
		{ SHOP_OFFSET_X_LEFT_RELICS, SHOP_OFFSET_Y_UP_RELICS },
		{ SHOP_OFFSET_X_LEFT_RELICS, SHOP_OFFSET_Y_DOWN_RELICS },
		{ SHOP_OFFSET_X_RIGHT_RELICS, SHOP_OFFSET_Y_DOWN_RELICS }
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
		uiElement->Setup("", "", texts[i], positions[i], DSFLOAT2(1.0f, 1.0f), 20.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		uiElement->m_BaseText.baseUI.SetPosition({ positions[i].x, positions[i].y + 0.10f});
		
		UIShopRelicComponent* uiRelicWindow = registry.AddComponent<UIShopRelicComponent>(relicWindow);

		CreateUIRelics(*uiElement, *uiRelicWindow, type[i], positions[i]);

		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);
		OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);

		uiOnClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::OnClick::SelectRelic, UIFunctions::OnClick::None);
		uiOnClick->Add(uiElement->m_Images[3].baseUI.GetPixelCoords(), uiElement->m_Images[3].baseUI.GetBounds(), UIFunctions::OnClick::SelectRelic, UIFunctions::OnClick::None);

		uiOnHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::OnHover::ShopRelic);
		uiOnHover->Add(uiElement->m_Images[3].baseUI.GetPixelCoords(), uiElement->m_Images[3].baseUI.GetBounds(), UIFunctions::OnHover::ShopRelic);

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
		"Weapon",
		"Next Level"
	};

	const DSFLOAT2 positions[SHOP_SINGLE_WINDOWS] =
	{
		{ SHOP_OFFSET_X_LEFT_HEAL, SHOP_OFFSET_Y_DOWN_BUTTONS },
		{ SHOP_OFFSET_X_RIGHT_REROLL, SHOP_OFFSET_Y_DOWN_BUTTONS  },
		{ SHOP_OFFSET_X_RIGHT_WEAPON, SHOP_OFFSET_Y_UP_WEAPON },
		{ 0.65f, -0.825 }
	};

	const DSFLOAT2 scales[SHOP_SINGLE_WINDOWS] =
	{
		{ 1.5f, 1.5f },
		{ 1.5f, 1.5f },
		{ 1.0f, 1.0f },
		{ 1.0f, 1.0f }
	};

	ML_String axeFile = "";
	ML_String axeFileHover = "";
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);

	if (player->weaponTier == 0)
	{
		axeFile = "Shop/Axe1";
		axeFileHover = "Shop/Axe1Hover";
	}
	else if (player->weaponTier == 1)
	{
		axeFile = "Shop/Axe2";
		axeFileHover = "Shop/Axe2Hover";
	}
	else
	{
		axeFile = "Shop/Axe3";
		axeFileHover = "Shop/Axe3Hover";
	}

	ML_String filenames[SHOP_SINGLE_WINDOWS] =
	{
		"Shop/Heal",
		"Shop/Reroll",
		axeFile,
		""
	};

	ML_String filenamesHover[SHOP_SINGLE_WINDOWS] =
	{
		"Shop/HealHover",
		"Shop/RerollHover",
		axeFileHover,
		""
	};

	void(*const functions[SHOP_SINGLE_WINDOWS])(void*, int)  =
	{
		UIFunctions::OnClick::HealPlayer,
		UIFunctions::OnClick::RerollRelic,
		UIFunctions::OnClick::UpgradeWeapon,
		UIFunctions::Game::ExitShopCutscene
	};

	const char name[SHOP_SINGLE_WINDOWS][32] =
	{
		"Heal",
		"Reroll",
		"Upgrade Weapon",
		""
	};

	const char description[SHOP_SINGLE_WINDOWS][64] =
	{
		"Recover 25% of max Health",
		"Reroll a new set of relics",
		"Increase your base damage by 25%",
		"Leave the shop"
	};

	uint8_t heal = 0;

	if (!player->healFreebie)
		heal = 3;

	uint8_t price[SHOP_SINGLE_WINDOWS] =
	{
		heal,
		4,
		5 * (player->weaponTier + 2),
		0,
	};

	float fontSize[SHOP_SINGLE_WINDOWS] =
	{
		20.0f,
		20.0f,
		20.0f,
		25.0f,
	};

	DWRITE_PARAGRAPH_ALIGNMENT pAling[SHOP_SINGLE_WINDOWS] =
	{
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		DWRITE_PARAGRAPH_ALIGNMENT_CENTER
	};

	for (int i = 0; i < SHOP_SINGLE_WINDOWS; i++)
	{
		EntityID relicWindow = registry.CreateEntity();
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);
		UIShopButtonComponent* button = registry.AddComponent<UIShopButtonComponent>(relicWindow);
		button->Setup(name[i], description[i], price[i]);

		if (i == 3)
		{
			uiElement->Setup("ButtonMedium", "ButtonMediumHover", texts[i], positions[i], { 1.0f, 1.0f }, fontSize[i], DWRITE_TEXT_ALIGNMENT_CENTER, pAling[i]);
			
		}
		else
		{
			uiElement->Setup("", "", texts[i], positions[i], { 1.0f, 1.0f } , fontSize[i], DWRITE_TEXT_ALIGNMENT_CENTER, pAling[i]);
			
			if (std::strcmp(texts[i], "Weapon") != 0)
				uiElement->m_BaseText.baseUI.SetPosition({ positions[i].x, positions[i].y + 0.10f });
			else
				uiElement->m_BaseText.baseUI.SetPosition({ positions[i].x, positions[i].y + 0.15f });

			
			uiElement->AddImage(filenames[i].c_str(), { positions[i].x, positions[i].y }, scales[i], false);
			uiElement->AddHoverImage(uiElement->m_Images[uiElement->m_Images.size() - 1], filenamesHover[i].c_str());

			DSFLOAT2 soulPos = { positions[i].x, positions[i].y - 0.08f };

			if (price[i] > 0)
			{
				char Sprice[4];
				sprintf(Sprice, "%i", price[i]);

				if (std::strcmp(texts[i], "Weapon") == 0)
				{
					uiElement->AddImage("Soul_Stone", { soulPos.x , soulPos.y - 0.095f }, {1.0f, 1.0f}, false);
					uiElement->AddText(Sprice, uiElement->m_Images[uiElement->m_Images.size() - 1].baseUI.GetBounds(), { soulPos.x + 0.015f, soulPos.y - 0.095f });
				}
				else
				{
					uiElement->AddImage("Soul_Stone", soulPos, { 1.0f, 1.0f }, false);
					uiElement->AddText(Sprice, uiElement->m_Images[uiElement->m_Images.size() - 1].baseUI.GetBounds(), { soulPos.x + 0.015f, soulPos.y });
				}
			}

			if (registry.GetComponent<PlayerComponent>(stateManager.player)->healFreebie && std::strcmp(texts[i], "Heal") == 0)
			{
				uiElement->AddImage("Soul_Stone", soulPos, { 1.0f, 1.0f }, false);
				uiElement->m_Images[uiElement->m_Images.size() - 1].baseUI.SetVisibility(false);
				uiElement->AddText("Free", uiElement->m_BaseImage.baseUI.GetBounds(), { soulPos.x, soulPos.y });
			}
		}

		if (i == 0)
			registry.AddComponent<UIShopHealComponent>(relicWindow);
		else if (i == 1)
			registry.AddComponent<UIShopRerollComponent>(relicWindow);
		else if (i == 2)
			registry.AddComponent<UIShopUpgradeComponent>(relicWindow);

		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);
		OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);

		if (i == 3)
		{
			uiOnClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);
			uiOnHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::ShopButton);
		}
		else
		{
			uiOnClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), functions[i], UIFunctions::OnClick::None);
			uiOnHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunctions::OnHover::ShopButton);
		}

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
	uiTitle->Setup("Shop/ImpSign", "", "Lil\' Devil\'s Shoppe", { SHOP_POSITION_X, SHOP_POSITION_Y }, DSFLOAT2(1.0f, 1.0f), 30.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	registry.AddComponent<UIShopTitleImpComponent>(shopTitle);

	UIComponent* uiImpText = registry.AddComponent<UIComponent>(impText);
	uiImpText->Setup("Shop/ImpNoteBook", "", "", { 0.65f, 0.0f }, { 1.0f, 1.0f }, 20.0f);
	
	uiImpText->m_BaseText.m_brush = Black;

	float scaler = 0.9;

	uiImpText->m_BaseText.baseUI.m_CurrentBounds.right *= scaler;
	uiImpText->m_BaseText.baseUI.m_PositionBounds.right *= scaler;
	uiImpText->m_BaseText.baseUI.m_OriginalBounds.right *= scaler;

	uiImpText->m_BaseText.baseUI.SetPosition(DSFLOAT2(uiImpText->m_BaseImage.baseUI.GetPosition().x, 0.0f));

	registry.AddComponent<UIShopImpComponent>(impText);

	UIComponent* uiPlayerInfo = registry.AddComponent<UIComponent>(statsText);
	uiPlayerInfo->Setup("", "", "Player Stats", { SHOP_OFFSET_X_RIGHT_STATS,  SHOP_OFFSET_Y_DOWN_STATS }, { 1.0f, 1.0f }, 30.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	for (int i = 0; i < 3; i++)
	{
		uiPlayerInfo->AddText(" ", uiPlayerInfo->m_BaseText.baseUI.GetBounds(),
			DSFLOAT2(uiPlayerInfo->m_BaseText.baseUI.GetPosition().x, uiPlayerInfo->m_BaseText.baseUI.GetPosition().y - ((i + 1) * 0.05f) - 0.05f),
			{ 1.0f, 1.0f }, 20.0f);
	}


	registry.AddComponent<UIShopPlayerStatsComponent>(statsText);

}

void LoadShop()
{
	EntityID shop = registry.CreateEntity();
	UIComponent* uiShop = registry.AddComponent<UIComponent>(shop);
	uiShop->Setup("Shop/ShopPanel", "", "", { SHOP_POSITION_X, SHOP_POSITION_Y - 0.875f });

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
				if (shopBuy->onClickFunctionsPressed[i] == UIFunctions::OnClick::UpgradeWeapon) //Changed to Upgrade weapon because buy was removed. //Purchase button found, play the first imp voice line.
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
	EntityID shop = registry.CreateEntity();
	UIComponent* uiShop = registry.AddComponent<UIComponent>(shop);
	uiShop->Setup("Shop/ShopPanel", "", "", { SHOP_POSITION_X, SHOP_POSITION_Y - 0.875f });

	CreateTextWindows();

	CreateRelicWindows();

	CreateSingleWindows();

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
				if (shopBuy->onClickFunctionsPressed[i] == UIFunctions::OnClick::UpgradeWeapon) //Changed to Upgrade weapon because buy was removed. //Purchase button found, play the correct sound based on the level.
				{
					SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
					if (sfx != nullptr)
					{
						switch (stateManager.activeLevel)
						{
						case 4:
							sfx->Play(Shop_BeforeLava, Channel_Extra);
							for (auto audio : View<AudioEngineComponent>(registry))
							{
								registry.GetComponent<AudioEngineComponent>(audio)->HandleSound();
							}
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

