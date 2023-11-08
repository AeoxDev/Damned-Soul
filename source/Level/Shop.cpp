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
	ML_Array<ML_String, 3> texts;
	texts[0] = "Offence";
	texts[1] = "Defence";
	texts[2] = "Gadget";

	ML_Array<DSFLOAT2, 3> positions;
	positions[0] = { 0.3f, 0.6f };
	positions[1] = { 0.3f, 0.3f };
	positions[2] = { 0.3f, 0.0f };

	ML_Array<Relics::RELIC_TYPE, 3> type;
	type[0] = Relics::RELIC_UNTYPED;
	type[1] = Relics::RELIC_UNTYPED;
	type[2] = Relics::RELIC_UNTYPED;


	for (int i = 0; i < 3; i++)
	{
		EntityID relicWindow = registry.CreateEntity();
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);
		uiElement->Setup("TempRelicFlavorHolder", texts[i].c_str(), positions[i]);

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
	ML_Array<ML_String, 4> texts;
	texts[0] = "Heal";
	texts[1] = "Reroll";
	texts[2] = "Lock";
	texts[3] = "Buy";

	ML_Array<DSFLOAT2, 4> positions;
	positions[0] = { 0.525f, 0.6f };
	positions[1] = { 0.525f, 0.3f };
	positions[2] = { 0.525f, 0.0f };
	positions[3] = { 0.525f, -0.3f };

	ML_Array<ML_String, 4> imageName;
	imageName[0] = "Heal";
	imageName[1] = "Reroll";
	imageName[2] = "Lock";
	imageName[3] = "Buy";

	ML_Array<void(*)(void*, int), 4> functions;
	functions[0] = UIFunc::HealPlayer;
	functions[1] = UIFunc::RerollRelic;
	functions[2] = UIFunc::LockRelic;
	functions[3] = UIFunc::BuyRelic;

	for (int i = 0; i < 4; i++)
	{
		EntityID relicWindow = registry.CreateEntity();
		UIComponent* uiElement = registry.AddComponent<UIComponent>(relicWindow);

		uiElement->Setup("", texts[i].c_str(), positions[i]);
		uiElement->AddImage(imageName[i].c_str(), { positions[i].x, positions[i].y - 0.05f }, { 1.0f, 1.0f }, false);

		if (i == 1)
			UIRerollComponent* uiReroll = registry.AddComponent<UIRerollComponent>(relicWindow);

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

	//faster loading to shop for debug purposes
	EntityID player = registry.CreateEntity();
	registry.AddComponent<StatComponent>(player, 1250.f, 20.0f, 100.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	registry.AddComponent<PlayerComponent>(player)->UpdateSouls(20);

	UIComponent* uiTitle = registry.AddComponent<UIComponent>(shopTitle);
	uiTitle->Setup("TempShopTitle", "Lil Devils Shop", { 0.3f, 0.8f });
	uiTitle->m_BaseImage.baseUI.SetVisibility(false);

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

	UIShopImpWindowComponent* uiImpTextWindow = registry.AddComponent<UIShopImpWindowComponent>(impText);
	SetInShop(true);
}

void ReloadShop()
{
	
}

