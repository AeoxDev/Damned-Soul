#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "Levels\LevelHelper.h"
#include "States/StateManager.h"
#include "UI/UIButton.h"
#include "UIButtonFunctions.h"

#include "UIComponents.h"

#include "States/StateEnums.h"

void LoadShop()
{

	EntityID shopTitle = registry.CreateEntity();
	EntityID shopOffence = registry.CreateEntity();
	EntityID shopDefence = registry.CreateEntity();
	EntityID shopGadget = registry.CreateEntity();
	EntityID shopWeapon = registry.CreateEntity();

	EntityID shopHeal = registry.CreateEntity();
	EntityID shopReroll = registry.CreateEntity();
	EntityID shopLock = registry.CreateEntity();
	EntityID shopConfirm = registry.CreateEntity();
	
	EntityID impText = registry.CreateEntity();

	EntityID shopNextLevel = registry.CreateEntity();

	//faster loading to shop for debug purposes
	EntityID player = registry.CreateEntity();
	registry.AddComponent<StatComponent>(player, 1250.f, 20.0f, 100.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	registry.AddComponent<PlayerComponent>(player)->UpdateSouls(20);

	UIComponent* uiTitle = registry.AddComponent<UIComponent>(shopTitle);
	uiTitle->Setup("TempShopTitle", "Lil Devils Shop", { 0.3f, 0.8f });
	uiTitle->m_BaseImage.baseUI.SetVisibility(false);

	UIComponent* uiOffence = registry.AddComponent<UIComponent>(shopOffence);
	uiOffence->Setup("TempRelicFlavorHolder", "Offence", { 0.3f, 0.6f });
	uiOffence->AddImage("RelicIcons/Adrenaline_Rush", { 0.25f, 0.6f }, { 1.0f, 1.0f }, false);
	uiOffence->AddImage("RelicIcons/Advanced_Fighting", { 0.35f, 0.6f }, { 1.0f, 1.0f }, false);

	OnClickComponent* uiOffenceOnClick = registry.AddComponent<OnClickComponent>(shopOffence);
	OnHoverComponent* uiOffenceOnHover = registry.AddComponent<OnHoverComponent>(shopOffence);

	//uiOffenceOnClick->Setup(uiOffence->m_Images[0].baseUI.GetPixelCoords(), uiOffence->m_Images[0].baseUI.GetBounds(), UIFunc::Shop_BuyRelic);
	//uiOffenceOnClick->Setup(uiOffence->m_Images[1].baseUI.GetPixelCoords(), uiOffence->m_Images[1].baseUI.GetBounds(), UIFunc::Shop_BuyRelic);

	UIComponent* uiHeal = registry.AddComponent<UIComponent>(shopHeal);
	uiHeal->Setup("", "Heal", { 0.525f, 0.6f });
	uiHeal->AddImage("Heal", { 0.525f, 0.55f }, { 1.0f, 1.0f }, false);

	OnClickComponent* uiHealOnClick = registry.AddComponent<OnClickComponent>(shopHeal);
	OnHoverComponent* uiHealOnHover = registry.AddComponent<OnHoverComponent>(shopHeal);

	UIComponent* uiDefence = registry.AddComponent<UIComponent>(shopDefence);
	uiDefence->Setup("TempRelicFlavorHolder", "Defence", { 0.3f, 0.3f });
	uiDefence->AddImage("RelicIcons/Soul_Health", { 0.25f, 0.3f }, { 1.0f, 1.0f }, false);
	uiDefence->AddImage("RelicIcons/Second_Wind", { 0.35f, 0.3f }, { 1.0f, 1.0f }, false);

	OnClickComponent* uiDefenceOnClick = registry.AddComponent<OnClickComponent>(shopDefence);
	OnHoverComponent* uiDefenceOnHover = registry.AddComponent<OnHoverComponent>(shopDefence);

	UIComponent* uiReroll = registry.AddComponent<UIComponent>(shopReroll);
	uiReroll->Setup("", "Reroll", { 0.525f, 0.3f });
	uiReroll->AddImage("Roll", { 0.525f, 0.25f }, { 1.0f, 1.0f }, false);

	OnClickComponent* uiRerollOnClick = registry.AddComponent<OnClickComponent>(shopReroll);
	OnHoverComponent* uiRerollOnHover = registry.AddComponent<OnHoverComponent>(shopReroll);

	UIComponent* uiGadget = registry.AddComponent<UIComponent>(shopGadget);
	uiGadget->Setup("TempRelicFlavorHolder", "Gadget", { 0.3f, 0.0f });
	uiGadget->AddImage("RelicIcons/Dash_Flash", { 0.25f, 0.0f }, { 1.0f, 1.0f }, false);
	uiGadget->AddImage("RelicIcons/Icy_Blade", { 0.35f, 0.0f }, { 1.0f, 1.0f }, false);

	OnClickComponent* uiGadgetOnClick = registry.AddComponent<OnClickComponent>(shopGadget);
	OnHoverComponent* uiGadgetOnHover = registry.AddComponent<OnHoverComponent>(shopGadget);

	UIComponent* uiLock = registry.AddComponent<UIComponent>(shopLock);
	uiLock->Setup("", "Lock", { 0.525f, 0.0f });
	uiLock->AddImage("Lock", { 0.525f, -0.05f }, { 1.0f, 1.0f }, false);

	OnClickComponent* uiLockOnClick = registry.AddComponent<OnClickComponent>(shopLock);
	OnHoverComponent* uiLockOnHover = registry.AddComponent<OnHoverComponent>(shopLock);

	UIComponent* uiWeapon = registry.AddComponent<UIComponent>(shopWeapon);
	uiWeapon->Setup("TempRelicFlavorHolder", "Weapon", { 0.3f, -0.3f });
	uiWeapon->AddImage("TempRelicFlavorHolder", { 0.3f, -0.3f });

	OnClickComponent* uiWeaponOnClick = registry.AddComponent<OnClickComponent>(shopWeapon);
	OnHoverComponent* uiWeaponOnHover = registry.AddComponent<OnHoverComponent>(shopWeapon);

	UIComponent* uiConfirm = registry.AddComponent<UIComponent>(shopConfirm);
	uiConfirm->Setup("", "Confirm", { 0.525f, -0.3f });

	OnClickComponent* uiConfirmOnClick = registry.AddComponent<OnClickComponent>(shopConfirm);
	OnHoverComponent* uiConfirmOnHover = registry.AddComponent<OnHoverComponent>(shopConfirm);

	UIComponent* uiNextLevel = registry.AddComponent<UIComponent>(shopNextLevel);
	uiNextLevel->Setup("TempNextLevel", "", { 0.3f, -0.55f }, { 1.5f, 1.5f });

	OnClickComponent* uiNextLevelOnClick = registry.AddComponent<OnClickComponent>(shopNextLevel);
	OnHoverComponent* uiNextLevelOnHover = registry.AddComponent<OnHoverComponent>(shopNextLevel);

	UIComponent* uiImpText = registry.AddComponent<UIComponent>(impText);
	uiImpText->Setup("TempRelicHolder", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla interdum, ante varius tempus rhoncus, mi sapien imperdiet nisi, ac tincidunt nisi ligula vel nibh. Donec eu mi eu quam volutpat lacinia. Lorem ipsum dolor sit amet, consectetur adipiscing elit.", 
		{ -0.2f, -0.1f }, { 2.0f, 2.0f });
	uiImpText->m_BaseImage.baseUI.SetVisibility(false);

}

void ReloadShop()
{
	/*EntityID playerUi = registry.CreateEntity();

	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DSFLOAT2(-0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f));
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->text.Setup("");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DSFLOAT2(-0.8f, 0.6f), DSFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup("");

	bool* ignore = {};
	UIFunc::Shop_ReRollRelic(ignore);
	SetInShop(true);*/
}

