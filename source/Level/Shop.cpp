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

#define SHOP_POSITION_X (-0.35f)
#define SHOP_OFFSET_X (SHOP_POSITION_X + 0.225f)

#define SHOP_RELIC_WINDOWS (3)
#define SHOP_SINGLE_WINDOWS (6)

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
	const char texts[SHOP_RELIC_WINDOWS][32] =
	{
		"Offence",
		"Defence",
		"Gadget"
	};

	const DSFLOAT2 positions[SHOP_RELIC_WINDOWS] =
	{
		{ SHOP_POSITION_X, 0.6f }, 
		{ SHOP_POSITION_X, 0.3f }, 
		{ SHOP_POSITION_X, 0.0f }
	};

	const Relics::RELIC_TYPE type[SHOP_RELIC_WINDOWS] =
	{
		Relics::RELIC_UNTYPED,
		Relics::RELIC_UNTYPED,
		Relics::RELIC_UNTYPED
	};

	for (int i = 0; i < SHOP_RELIC_WINDOWS; i++)
	{
		EntityID relicWindow = registry.CreateEntity(ENT_PERSIST_LEVEL);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);
		uiElement->Setup("TempRelicFlavorHolder", texts[i], positions[i]);

		UIRelicWindowComponent* uiRelicWindow = registry.AddComponent<UIRelicWindowComponent>(relicWindow);

		CreateUIRelics(*uiElement, *uiRelicWindow, type[i], positions[i]);

		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);
		OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);

		uiOnClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, UIFunc::EmptyOnClick);
		uiOnClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), 1, UIFunc::SelectRelic);
		uiOnClick->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), 1, UIFunc::SelectRelic);

		uiOnHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::EmptyOnHover);
		uiOnHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunc::HoverShopRelic);
		uiOnHover->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunc::HoverShopRelic);

	}

};

void CreateSingleWindows()
{

	const char texts[SHOP_SINGLE_WINDOWS][32] =
	{
		"Heal",
		"Reroll",
		"Lock",
		"Buy",
		"Weapon",
		""
	};

	const DSFLOAT2 positions[SHOP_SINGLE_WINDOWS] =
	{
		{SHOP_OFFSET_X, 0.6f},
		{SHOP_OFFSET_X, 0.3f},
		{SHOP_OFFSET_X, 0.0f},
		{SHOP_OFFSET_X, -0.3f},
		{SHOP_POSITION_X, -0.3f},
		{0.8f, -0.75f }
	};

	//doesnt work in setup image, "" != "" is true
	const char baseFilenames[SHOP_SINGLE_WINDOWS][32] =
	{
		"",
		"",
		"",
		"",
		"TempRelicFlavorHolder",
		"TempRelicFlavorHolder"
	};

	const char filenames[SHOP_SINGLE_WINDOWS][32] =
	{
		"Heal",
		"Reroll",
		"Lock",
		"Buy",
		"Axe",
		"TempNextLevel"
	};

	void(*const functions[SHOP_SINGLE_WINDOWS])(void*, int)  =
	{
		UIFunc::HealPlayer,
		UIFunc::RerollRelic,
		UIFunc::LockRelic,
		UIFunc::BuyRelic,
		UIFunc::EmptyOnClick,
		UIFunc::LoadNextLevel
	};

	for (int i = 0; i < SHOP_SINGLE_WINDOWS; i++)
	{
		EntityID relicWindow = registry.CreateEntity(ENT_PERSIST_LEVEL);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);

		if (i == 4)
			uiElement->Setup("TempRelicFlavorHolder", texts[i], positions[i]);
		else
			uiElement->Setup("", texts[i], positions[i]);

		uiElement->AddImage(filenames[i], { positions[i].x, positions[i].y - 0.05f }, { 1.0f, 1.0f }, false);

		if (i == 5)
			uiElement->m_BaseImage.baseUI.SetVisibility(false);

		if (i == 1)
			registry.AddComponent<UIRerollComponent>(relicWindow);
		else
			registry.AddComponent<UIShopButtonComponent>(relicWindow);


		OnClickComponent* uiOnClick = registry.AddComponent<OnClickComponent>(relicWindow);

		uiOnClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, UIFunc::EmptyOnClick);
		uiOnClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), 1, functions[i]);

		if (i != 5)
		{
			OnHoverComponent* uiOnHover = registry.AddComponent<OnHoverComponent>(relicWindow);
			uiOnHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);
		}
	}
}

void LoadShop()
{

	EntityID shopTitle = registry.CreateEntity(ENT_PERSIST_LEVEL);
	EntityID impText = registry.CreateEntity(ENT_PERSIST_LEVEL);

	UIComponent* uiTitle = registry.AddComponent<UIComponent>(shopTitle);
	uiTitle->Setup("TempShopTitle", "Lil\' Devil\'s Shop", { SHOP_POSITION_X, 0.8f });
	uiTitle->m_BaseImage.baseUI.SetVisibility(false);

	registry.AddComponent<UIShopTitleImpComponent>(shopTitle);
	
	CreateRelicWindows();

	CreateSingleWindows();

	UIComponent* uiImpText = registry.AddComponent<UIComponent>(impText);
	uiImpText->Setup("TempRelicHolder", "\n\nHello There", { 0.3f, 0.1f }, { 2.0f, 2.0f });
	uiImpText->m_BaseImage.baseUI.SetVisibility(false);

	registry.AddComponent<UIShopImpComponent>(impText);

	SetInShop(true);
}

void ReloadShop()
{
	SetInShop(true);
	void* a = {};
	UIFunc::RerollRelic(a, -1);

}

