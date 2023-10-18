#include "Components/UIComponents/UIShopComponent.h"
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

	UIImage relicWindowBaseImage;
	relicWindowBaseImage.Setup("TempRelicFlavorHolder.png");
	UIImage relicImage;
	relicImage.Setup("icons/frostfire revision.png");
	UIText relicName;
	relicName.Setup(L"");

	UIShopRelicWindowComponent* relicWindowC = registry.AddComponent<UIShopRelicWindowComponent>(relicWindow);
	relicWindowC->Setup(relicName, relicWindowBaseImage, relicImage);
	relicWindowC->m_baseImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 7), spritePixelCoords.y - (0.1f * 3 * positionMultiplier) });
	relicWindowC->m_relicImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 7.5f), spritePixelCoords.y - (0.1f * 3 * positionMultiplier) });

	ChangeOffset(spritePositionOffset, startingSpritePosition, spritePixelCoords, *relicWindowC);

	UIButton* buyRelic = registry.AddComponent<UIButton>(relicButtonBuy);
	buyRelic->Setup("TempBuy.png", "TempBuy.png", L"", UIFunc::Shop_BuyRelic);
	buyRelic->SetRelicButton(true);
	buyRelic->SetPosition({ spritePixelCoords.x + 0.01f, spritePixelCoords.y - (0.1f * 0) });

	UIButton* lockRelic = registry.AddComponent<UIButton>(relicButtonLock);
	lockRelic->Setup("TempLock.png", "TempLock.png", L"", UIFunc::Shop_LockRelic);
	lockRelic->SetRelicButton(true);
	lockRelic->SetPosition({ spritePixelCoords.x + 0.01f, spritePixelCoords.y - (0.1f * 1) });

}


UIShopComponent::UIShopComponent()
{
	baseImage.Setup("TempShopWindow3.png", this->position, this->scale);
	playerInfo.Setup(L"");
}

void UIShopComponent::Setup(EntityID& shop)
{

	DirectX::XMFLOAT2 spritePositionOffset = { this->baseImage.m_UiComponent.m_CurrentBounds.right / (this->baseImage.m_UiComponent.m_CurrentBounds.right / 32.0f) ,
											   this->baseImage.m_UiComponent.m_CurrentBounds.bottom / (this->baseImage.m_UiComponent.m_CurrentBounds.bottom / 32.0f) };

	DirectX::XMFLOAT2 startingSpritePosition = { abs(this->baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
								   abs(this->baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
	DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
									-1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

	UIButton* rerollButton = registry.AddComponent<UIButton>(shop);
	rerollButton->Setup("TempReRoll.png", "TempReRoll.png", L"", UIFunc::Shop_ReRollRelic, { spritePixelCoords.x + (0.1f * 7), spritePixelCoords.y - (0.1f * 11) });

	CreateShopEntity(*this, 1);
	CreateShopEntity(*this, 2);
	CreateShopEntity(*this, 3);

}
