#include "Components/UIComponents/UIShopComponent.h"
#include "Components/UIComponents/UIRelicComponent.h"
#include "SDLHandler.h"
#include "UI/UIRenderer.h"



void ChangeOffset(DirectX::XMFLOAT2& spritePositionOffset, DirectX::XMFLOAT2& startingSpritePosition, DirectX::XMFLOAT2& spritePixelCoords, UIShopRelicWindowComponent& relicWindowC)
{
	spritePositionOffset = { relicWindowC.m_baseImage.m_UiComponent.m_CurrentBounds.right / (relicWindowC.m_baseImage.m_UiComponent.m_CurrentBounds.right / 32.0f) ,
											   relicWindowC.m_baseImage.m_UiComponent.m_CurrentBounds.bottom / (relicWindowC.m_baseImage.m_UiComponent.m_CurrentBounds.bottom / 32.0f) };

	startingSpritePosition = { abs(relicWindowC.m_baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
						   abs(relicWindowC.m_baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
	spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
									-1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };
}

void CreateShopEntity(const UIShopComponent& shop, int positionMultiplier)
{
	auto relicWindow = registry.CreateEntity();
	auto relicButtonBuy = registry.CreateEntity();
	auto relicButtonLock = registry.CreateEntity();

	DirectX::XMFLOAT2 spritePositionOffset = { shop.baseImage.m_UiComponent.m_CurrentBounds.right / (shop.baseImage.m_UiComponent.m_CurrentBounds.right / 32.0f) ,
											   shop.baseImage.m_UiComponent.m_CurrentBounds.bottom / (shop.baseImage.m_UiComponent.m_CurrentBounds.bottom / 32.0f) };

	DirectX::XMFLOAT2 startingSpritePosition = { abs(shop.baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
								   abs(shop.baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
	DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
									-1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

	UIImage relicWindowBaseImage, relicImage, relicFlavorImage;
	relicWindowBaseImage.Setup("TempRelicFlavorHolder.png");

	relicImage.Setup("RelicIcons/Empty_Relic.png");
	relicImage.m_UiComponent.SetScale({1.25f, 1.25f });
	
	relicFlavorImage.Setup("TempRelicFlavorHolder2.png");
	relicFlavorImage.m_UiComponent.SetVisibility(false);
	relicFlavorImage.m_UiComponent.SetScale({ 1.2f, 1.0f });

	UIText relicName, relicDesc;
	ML_String name = "No Relic";
	ML_String desc = "Emptiness all around us";

	relicName.Setup(name);
	relicDesc.Setup(desc);

	relicName.m_UiComponent.SetVisibility(false);
	relicDesc.m_UiComponent.SetVisibility(false);

	UIShopRelicWindowComponent* relicWindowC = registry.AddComponent<UIShopRelicWindowComponent>(relicWindow);
	relicWindowC->Setup(relicWindowBaseImage);
	relicWindowC->m_baseImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 7), spritePixelCoords.y - (0.1f * 3 * positionMultiplier) });
	ChangeOffset(spritePositionOffset, startingSpritePosition, spritePixelCoords, *relicWindowC);

	relicImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 1.5f), spritePixelCoords.y - 0.05f });
	
	relicFlavorImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 1.5f), spritePixelCoords.y + 0.1f });
	relicName.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 1.5f), spritePixelCoords.y + 0.1f });

	relicDesc.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 1.5f), spritePixelCoords.y + 0.1f });

	UIRelicComponent* relicComp = registry.AddComponent<UIRelicComponent>(relicWindow, relicImage, relicFlavorImage, relicName, relicDesc);

	UIButton* buyRelic = registry.AddComponent<UIButton>(relicButtonBuy);
	buyRelic->Setup("TempBuy.png", "TempBuy.png", "", UIFunc::Shop_BuyRelic);
	buyRelic->shopPosition = positionMultiplier;
	buyRelic->SetPosition({ spritePixelCoords.x + 0.01f, spritePixelCoords.y - (0.1f * 0) });

	UIButton* lockRelic = registry.AddComponent<UIButton>(relicButtonLock);
	lockRelic->Setup("TempLock.png", "TempLock.png", "", UIFunc::Shop_LockRelic);
	lockRelic->shopPosition = positionMultiplier;
	lockRelic->SetPosition({ spritePixelCoords.x + 0.01f, spritePixelCoords.y - (0.1f * 1) });

}


UIShopComponent::UIShopComponent()
{
	baseImage.Setup("TempShopWindow3.png", this->position, this->scale);
	playerInfo.Setup("");

	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0: relics.push_back(Relics::DemonBonemarrow(false)); break;
		case 1: relics.push_back(Relics::FlameWeapon(false)); break;
		case 2: relics.push_back(Relics::SoulPower(false)); break;
		case 3: relics.push_back(Relics::DemonHeart(false)); break;
		case 4: relics.push_back(Relics::FrostFire(false)); break;
		case 5: relics.push_back(Relics::SoulHealth(false)); break;
		default:
			break;
		}
		
	}

}

void UIShopComponent::Setup()
{

	DirectX::XMFLOAT2 spritePositionOffset = { this->baseImage.m_UiComponent.m_CurrentBounds.right / (this->baseImage.m_UiComponent.m_CurrentBounds.right / 32.0f) ,
											   this->baseImage.m_UiComponent.m_CurrentBounds.bottom / (this->baseImage.m_UiComponent.m_CurrentBounds.bottom / 32.0f) };

	DirectX::XMFLOAT2 startingSpritePosition = { abs(this->baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
								   abs(this->baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
	DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
									-1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };


	auto rerollEntity = registry.CreateEntity();
	UIButton* rerollButton = registry.AddComponent<UIButton>(rerollEntity);
	rerollButton->Setup("TempReRoll.png", "TempReRoll.png", "", UIFunc::Shop_ReRollRelic, { spritePixelCoords.x + (0.1f * 7), spritePixelCoords.y - (0.1f * 11) });

	auto nextLevelEntity = registry.CreateEntity();
	UIButton* nextLevelButton = registry.AddComponent<UIButton>(nextLevelEntity);
	nextLevelButton->Setup("TempNextLevel.png", "TempNextLevel.png", "", UIFunc::LoadNextLevel, { spritePixelCoords.x + (0.1f * 1), spritePixelCoords.y - (0.1f * 11) });
	
	for (int i = 0; i < 3; i++)
	{
		CreateShopEntity(*this, i + 1);
	};

	UIFunc::Shop_ReRollRelic(nullptr);
}
