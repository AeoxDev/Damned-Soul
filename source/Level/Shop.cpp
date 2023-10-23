#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "Levels\LevelHelper.h"

#include "UI/UIButton.h"
#include "UIButtonFunctions.h"

void LoadShop()
{

	EntityID shop = registry.CreateEntity();
	
	//faster loading to shop
	//EntityID player = registry.CreateEntity();
	//registry.AddComponent<StatComponent>(player, 1250.f, 20.0f, 100.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	//registry.AddComponent<PlayerComponent>(player);

	//Temp stuff for ui to not crash because saving between levels is not fully implemented
	EntityID playerUi = registry.CreateEntity();
	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f));

	UIShopComponent* shopComp = registry.AddComponent<UIShopComponent>(shop);
	shopComp->Setup();

}