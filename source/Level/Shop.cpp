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

void CreateUIRelics(UIComponent& uiComp, UIRelicWindowComponent& uiRelicComp, const Relics::RELIC_TYPE& type, DSFLOAT2 pos)
{
	ML_Array<float, 2> xPos;
	xPos[0] = pos.x - 0.05f;
	xPos[1] = pos.x + 0.05f;

	for (int i = 0; i < 2; i++)
	{
		const RelicData* relic = Relics::PickRandomRelic(type);
		uiComp.AddImage(relic->m_filePath, { xPos[i], pos.y - 0.025f }, { 1.5f, 1.5f }, false);
		uiRelicComp.shopRelics[i] = relic;
	}

	uiComp.AddImage("RelicIcons\\HoverRelic", { 0.0f, 0.0f }, { 1.5f, 1.5f }, false);
	uiComp.AddImage("RelicIcons\\HoverRelic", { 0.0f, 0.0f }, { 1.5f, 1.5f }, false);
	uiComp.m_Images[uiComp.m_Images.size() - 2].baseUI.SetVisibility(false);
	uiComp.m_Images[uiComp.m_Images.size() - 1].baseUI.SetVisibility(false);

};


void CreateRelicWindows()
{
	const char const texts[3][32] =
	{
		"Offence",
		"Defence",
		"Gadget"
	};

	const DSFLOAT2 const positions[3] =
	{
		{ 0.3f, 0.6f }, 
		{0.3f, 0.3f}, 
		{0.3f, 0.0f}
	};

	const Relics::RELIC_TYPE const type[3] =
	{
		Relics::RELIC_UNTYPED,
		Relics::RELIC_UNTYPED,
		Relics::RELIC_UNTYPED
	};

	for (int i = 0; i < 3; i++)
	{
		EntityID relicWindow = registry.CreateEntity(ENT_PERSIST_LEVEL);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);
		uiElement->Setup("TempRelicFlavorHolder", texts[i], positions[i]);

		UIRelicWindowComponent* uiRelicWindow = registry.AddComponent<UIRelicWindowComponent>(relicWindow);

		CreateUIRelics(*uiElement, *uiRelicWindow, type[i], positions[i]);

		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);
		OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);

		uiOnClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunc::SelectRelic);
		uiOnClick->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunc::SelectRelic);

		uiOnHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunc::HoverRelic);
		uiOnHover->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunc::HoverRelic);

	}

};

void CreateSingleWindows()
{

	const char const texts[4][32] =
	{
		"Heal",
		"Reroll",
		"Lock",
		"Buy"
	};

	const DSFLOAT2 const positions[4] =
	{
		{0.525f, 0.6f},
		{0.525f, 0.3f},
		{0.525f, 0.0f},
		{0.525f, -0.3f}
	};

	void(*const functions[4])(void*, int)  =
	{
		UIFunc::HealPlayer,
		UIFunc::RerollRelic,
		UIFunc::LockRelic,
		UIFunc::BuyRelic
	};

	for (int i = 0; i < 4; i++)
	{
		EntityID relicWindow = registry.CreateEntity(ENT_PERSIST_LEVEL);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);

		uiElement->Setup("", texts[i], positions[i]);
		uiElement->AddImage(texts[i], { positions[i].x, positions[i].y - 0.05f }, { 1.0f, 1.0f }, false);

		if (i == 1)
			registry.AddComponent<UIRerollComponent>(relicWindow);
		else
			registry.AddComponent<UIShopButtonComponent>(relicWindow);


		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);
		OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);

		uiOnClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), functions[i]);
		uiOnHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);
	}
}

void LoadShop()
{

	EntityID shopTitle = registry.CreateEntity();
	EntityID shopWeapon = registry.CreateEntity();
	EntityID shopNextLevel = registry.CreateEntity();
	
	EntityID impText = registry.CreateEntity();

	UIComponent* uiTitle = registry.AddComponent<UIComponent>(shopTitle);
	uiTitle->Setup("TempShopTitle", "Lil Devils Shop", { 0.3f, 0.8f });
	uiTitle->m_BaseImage.baseUI.SetVisibility(false);

	registry.AddComponent<UIShopTitleImpComponent>(shopTitle);
	
	CreateRelicWindows();

	CreateSingleWindows();

	UIComponent* uiWeapon = registry.AddComponent<UIComponent>(shopWeapon);
	uiWeapon->Setup("TempRelicFlavorHolder", "Weapon", { 0.3f, -0.3f });

	OnClickComponent* uiWeaponOnClick = registry.AddComponent<OnClickComponent>(shopWeapon);
	OnHoverComponent* uiWeaponOnHover = registry.AddComponent<OnHoverComponent>(shopWeapon);

	UIComponent* uiNextLevel = registry.AddComponent<UIComponent>(shopNextLevel);
	uiNextLevel->Setup("TempNextLevel", "", { 0.3f, -0.55f }, { 1.5f, 1.5f });

	OnClickComponent* uiNextLevelOnClick = registry.AddComponent<OnClickComponent>(shopNextLevel);
	OnHoverComponent* uiNextLevelOnHover = registry.AddComponent<OnHoverComponent>(shopNextLevel);

	uiNextLevelOnClick->Setup(uiNextLevel->m_BaseImage.baseUI.GetPixelCoords(), uiNextLevel->m_BaseImage.baseUI.GetBounds(), UIFunc::LoadNextLevel);

	UIComponent* uiImpText = registry.AddComponent<UIComponent>(impText);
	uiImpText->Setup("TempRelicHolder", "Hello There", { -0.2f, -0.1f }, { 2.0f, 2.0f });
	uiImpText->m_BaseImage.baseUI.SetVisibility(false);

	registry.AddComponent<UIShopImpComponent>(impText);

	SetInShop(true);
}

void ReloadShop()
{
	EntityID shopWeapon = registry.CreateEntity();
	EntityID shopNextLevel = registry.CreateEntity();

	EntityID impText = registry.CreateEntity();

	UIComponent* uiWeapon = registry.AddComponent<UIComponent>(shopWeapon);
	uiWeapon->Setup("TempRelicFlavorHolder", "Weapon", { 0.3f, -0.3f });

	OnClickComponent* uiWeaponOnClick = registry.AddComponent<OnClickComponent>(shopWeapon);
	OnHoverComponent* uiWeaponOnHover = registry.AddComponent<OnHoverComponent>(shopWeapon);

	UIComponent* uiNextLevel = registry.AddComponent<UIComponent>(shopNextLevel);
	uiNextLevel->Setup("TempNextLevel", "", { 0.3f, -0.55f }, { 1.5f, 1.5f });

	OnClickComponent* uiNextLevelOnClick = registry.AddComponent<OnClickComponent>(shopNextLevel);
	OnHoverComponent* uiNextLevelOnHover = registry.AddComponent<OnHoverComponent>(shopNextLevel);

	uiNextLevelOnClick->Setup(uiNextLevel->m_BaseImage.baseUI.GetPixelCoords(), uiNextLevel->m_BaseImage.baseUI.GetBounds(), UIFunc::LoadNextLevel);

	UIComponent* uiImpText = registry.AddComponent<UIComponent>(impText);
	uiImpText->Setup("TempRelicHolder", "Hello There", { -0.2f, -0.1f }, { 2.0f, 2.0f });
	uiImpText->m_BaseImage.baseUI.SetVisibility(false);

	registry.AddComponent<UIShopImpComponent>(impText);

	SetInShop(true);

	void* a = {};
	UIFunc::RerollRelic(a, -1);
}

