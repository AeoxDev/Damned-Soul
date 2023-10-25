#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "Levels\LevelHelper.h"

#include "UI/UIButton.h"
#include "UIButtonFunctions.h"

#include "UIComponents.h"

void LoadShop()
{

	EntityID shop = registry.CreateEntity();
	
	//faster loading to shop
	//EntityID player = registry.CreateEntity();
	//registry.AddComponent<StatComponent>(player, 1250.f, 20.0f, 100.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	//registry.AddComponent<PlayerComponent>(player)->UpdateSouls(20);

	//Temp stuff for ui to not crash because saving between levels is not fully implemented
	EntityID playerUi = registry.CreateEntity();

	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DSFLOAT2(-0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f));
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->text.Setup("");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DSFLOAT2(-0.8f, 0.6f), DSFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup("");

	UIShopComponent* shopComp = registry.AddComponent<UIShopComponent>(shop);
	shopComp->Setup();

	UIText* shopText = registry.AddComponent<UIText>(shop);
	shopText->Setup("Shop");
	shopText->m_UiComponent.SetPosition({ 0.0f, 0.8f });
	shopText->m_UiComponent.SetScale({ 1.5f, 1.5f });
}