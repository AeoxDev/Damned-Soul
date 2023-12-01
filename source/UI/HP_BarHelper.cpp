#include "HP_BarHelper.h"
#include "UI\UI.h"
#include "ComponentHelper.h"
#include "UIComponents.h"
#include "Registry.h"
#include "States\StateManager.h"

static DSFLOAT2 bl(-0.73f, 0.85f);

//static uint32_t HP_BG;
static uint32_t HP_STRETCH;
static uint32_t HP_EDGE;
static uint32_t HP_LEFT;
static uint32_t HP_RIGHT;
static uint32_t HP_MID;
static uint32_t HP_START;
static uint32_t HP_END;
static uint32_t HP_TEXT;
static uint32_t SOUL_STONE;
static uint32_t SOUL_TEXT;
static uint32_t RC_ANCHOR;

#define BG_X (bl.x + .000f)
#define LOOP_LEFT_X (bl.x - .055f)
#define LOOP_RIGHT_X (bl.x + .056f)
#define LOOP_MID_X (bl.x + .000f)
#define RIGHT_END_X (bl.x + .132f)

#define _ADVANCED_HP_ROUND_PIXELS_UP(idx)																	\
uiElement->m_Images[idx].baseUI.m_PixelCoords.x = ceilf(uiElement->m_Images[idx].baseUI.m_PixelCoords.x);	\
uiElement->m_Images[idx].baseUI.m_PixelCoords.y = ceilf(uiElement->m_Images[idx].baseUI.m_PixelCoords.y);	\
uiElement->m_Images[idx].baseUI.UpdateTransform();															\

void SetUpAdvancedHealthBar(const EntityID player)
{

	// UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(player);

	//Setup + Health
	// Set background location!
	uiElement->Setup   ("HP_Bar/HP_BG", "",			DSFLOAT2(BG_X,			bl.y));
	uiElement->m_BaseImage.baseUI.m_PixelCoords.x = ceilf(uiElement->m_BaseImage.baseUI.m_PixelCoords.x);
	uiElement->m_BaseImage.baseUI.m_PixelCoords.y = ceilf(uiElement->m_BaseImage.baseUI.m_PixelCoords.y);
	uiElement->m_BaseImage.baseUI.UpdateTransform();

	HP_STRETCH = uiElement->m_Images.size();
	uiElement->AddImage("HP_Bar/HP_INDICATOR_STRETCH", DSFLOAT2(BG_X, bl.y));
	uiElement->m_BaseImage.baseUI.m_PixelCoords.x -= 2;
	_ADVANCED_HP_ROUND_PIXELS_UP(HP_STRETCH);
	HP_LEFT = uiElement->m_Images.size();
	uiElement->AddImage("HP_Bar/HP_LOOP",			DSFLOAT2(BG_X,	bl.y));
	uiElement->m_Images[HP_LEFT].baseUI.m_PixelCoords.x -= 50;
	_ADVANCED_HP_ROUND_PIXELS_UP(HP_LEFT);
	HP_RIGHT = uiElement->m_Images.size();
	uiElement->AddImage("HP_Bar/HP_LOOP",			DSFLOAT2(BG_X,	bl.y));
	uiElement->m_Images[HP_RIGHT].baseUI.m_PixelCoords.x += 50;
	_ADVANCED_HP_ROUND_PIXELS_UP(HP_RIGHT);
	HP_MID = uiElement->m_Images.size();
	uiElement->AddImage("HP_Bar/HP_MID",			DSFLOAT2(BG_X,	bl.y));
	_ADVANCED_HP_ROUND_PIXELS_UP(HP_MID);
	HP_START = uiElement->m_Images.size();
	uiElement->AddImage("HP_Bar/HP_LEFT_START",		DSFLOAT2(BG_X,	bl.y));
	uiElement->m_Images[HP_START].baseUI.m_PixelCoords.x -= 139;
	_ADVANCED_HP_ROUND_PIXELS_UP(HP_START);
	// Set HP Text location (I think?)
	uiElement->m_BaseText.baseUI.m_PixelCoords = uiElement->m_Images[HP_START].baseUI.m_PixelCoords;
	uiElement->m_BaseText.baseUI.UpdateTransform();
	// Create current health text
	HP_TEXT = uiElement->m_Texts.size();
	uiElement->AddText(" ", uiElement->m_Images[HP_TEXT].baseUI.GetOriginalBounds(), uiElement->m_Images[HP_START].baseUI.GetPosition());

	HP_END = uiElement->m_Images.size();
	uiElement->AddImage("HP_Bar/HP_RIGHT_END",		DSFLOAT2(BG_X,	bl.y));
	uiElement->m_Images[HP_END].baseUI.m_PixelCoords.x += 108;
	_ADVANCED_HP_ROUND_PIXELS_UP(HP_END);
	UIGameHealthComponent* uiHealth = registry.AddComponent<UIGameHealthComponent>(player);

	//Souls
	SOUL_STONE = uiElement->m_Images.size();
	uiElement->AddImage("Soul_Stone", DSFLOAT2(-0.91f, 0.75f));
	_ADVANCED_HP_ROUND_PIXELS_UP(SOUL_STONE)
	SOUL_TEXT = uiElement->m_Texts.size();
	uiElement->AddText(" ", uiElement->m_Images[SOUL_STONE].baseUI.GetOriginalBounds(), uiElement->m_Images[SOUL_STONE].baseUI.GetPosition());
	UIPlayerSoulsComponent* uiSouls = registry.AddComponent<UIPlayerSoulsComponent>(stateManager.player);

	//Relics
	RC_ANCHOR = uiElement->m_Images.size();
	uiElement->AddImage("RelicChain/RC_Anchor", uiElement->m_Images[SOUL_STONE].baseUI.GetPosition());
	uiElement->m_Images[RC_ANCHOR].baseUI.m_PixelCoords.y += 22;
	uiElement->m_Images[RC_ANCHOR].baseUI.m_PixelCoords.x += 5;
	_ADVANCED_HP_ROUND_PIXELS_UP(RC_ANCHOR)

	//uiElement->AddImage("TempRelicHolder11", DSFLOAT2(-0.95f, -0.1f));
	UIPlayerRelicsComponent* uiRelics = registry.AddComponent<UIPlayerRelicsComponent>(stateManager.player);
	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(stateManager.player);
}

void ScaleAdvancedHealthBar(const EntityID player)
{
	auto health = registry.GetComponent<UIGameHealthComponent>(player);
	auto uiElement = registry.GetComponent<UIComponent>(player);

	auto stats = registry.GetComponent<StatComponent>(player);
	int64_t maxHealth = stats->GetMaxHealth();
	int64_t currentHealth = stats->GetHealth();

	//float percentageHealth = (float)currentHealth / (float)maxHealth;

	health->value = currentHealth;
	int healthBoundsRight = (int)uiElement->m_Images[0].baseUI.m_OriginalBounds.right;

	uiElement->m_Images[0].baseUI.m_CurrentBounds.right = stats->GetHealth();

	if (player.index == stateManager.player.index)
	{
#define EDGE_FLAT_OFFSET (2.f)

		//stats->StealthilyModifyHealth(-0.05f);
		{
			UIBase& base = uiElement->m_Images[HP_STRETCH].baseUI;
			base.m_CurrentBounds.right = /*(base.m_OriginalBounds.right - base.m_OriginalBounds.left)*/200.f * (.01f * stats->GetHealth());
		}
		//{
		//	UIBase& base = uiElement->m_Images[HP_EDGE].baseUI;
		//	static FLOAT oiriginalX = base.m_PixelCoords.x - 105;
		//	base.m_PixelCoords.x = oiriginalX + currentHealth * 2;
		//	base.UpdateTransform();;
		//}

		// The additional health that the player has obtained
		float healthFactor = stats->GetMaxHealth() / 100.f;
		{
			UIBase& base = uiElement->m_BaseImage.baseUI;
			base.m_CurrentBounds.right = (base.m_OriginalBounds.right - base.m_OriginalBounds.left - EDGE_FLAT_OFFSET) * healthFactor;
		}
		{
			UIBase& base = uiElement->m_Images[HP_LEFT].baseUI;
			base.m_CurrentBounds.right = (base.m_OriginalBounds.right - base.m_OriginalBounds.left) * healthFactor;
		}
		{
			UIBase& base = uiElement->m_Images[HP_RIGHT].baseUI;
			base.m_CurrentBounds.left = base.m_OriginalBounds.left - EDGE_FLAT_OFFSET;
			base.m_CurrentBounds.right = 2.f * (stats->GetMaxHealth() - EDGE_FLAT_OFFSET) - 100;
		}
		{
			UIBase& base = uiElement->m_Images[HP_MID].baseUI;
			static FLOAT oiriginalX = base.m_PixelCoords.x - 100;
			base.m_PixelCoords.x = oiriginalX + stats->GetMaxHealth() - EDGE_FLAT_OFFSET;
			base.UpdateTransform();
		}
		{
			UIBase& base = uiElement->m_Images[HP_END].baseUI;
			static FLOAT oiriginalX = base.m_PixelCoords.x - 200;
			base.m_PixelCoords.x = oiriginalX + 2.f * (stats->GetMaxHealth() - EDGE_FLAT_OFFSET);
			base.UpdateTransform();
		}


		char temp[8] = "";
		sprintf(temp, "%lld", health->value);
		DSFLOAT2 position = uiElement->m_Images[HP_START].baseUI.GetPosition();
		//position.x -= .03f;
		uiElement->m_Texts[HP_TEXT].SetText(temp, uiElement->m_Texts[HP_TEXT].baseUI.GetBounds(), 16.f);
		uiElement->m_Texts[HP_TEXT].baseUI.Setup(position, uiElement->m_Images[HP_START].baseUI.GetScale(),
			uiElement->m_Images[HP_START].baseUI.GetRotation(), uiElement->m_Images[HP_START].baseUI.GetVisibility(),
			uiElement->m_Images[HP_START].baseUI.GetOpacity());

		uiElement->m_Texts[HP_TEXT].baseUI.m_PixelCoords = uiElement->m_Images[HP_START].baseUI.GetPixelCoords();
		uiElement->m_Texts[HP_TEXT].baseUI.m_PixelCoords.x -= 65;
		uiElement->m_Texts[HP_TEXT].baseUI.m_PixelCoords.y += 11;
		uiElement->m_Texts[HP_TEXT].baseUI.UpdateTransform();

		sprintf(temp, "%lld", stats->GetMaxHealth());
		position = uiElement->m_Images[HP_START].baseUI.GetPosition();
		//position.x += .01f;
		uiElement->m_BaseText.SetText(temp, uiElement->m_BaseText.baseUI.GetBounds(), 13.5f);
		uiElement->m_BaseText.baseUI.Setup(position, uiElement->m_Images[HP_START].baseUI.GetScale(),
										   uiElement->m_Images[HP_START].baseUI.GetRotation(), uiElement->m_Images[HP_START].baseUI.GetVisibility(),
										   uiElement->m_Images[HP_START].baseUI.GetOpacity());

		uiElement->m_BaseText.baseUI.m_PixelCoords = uiElement->m_Texts[HP_TEXT].baseUI.GetPixelCoords();
		uiElement->m_BaseText.baseUI.m_PixelCoords.x += 104;
		uiElement->m_BaseText.baseUI.m_PixelCoords.y += 17;
		//uiElement->m_BaseText.baseUI.m_PixelCoords.y += 11;
		uiElement->m_BaseText.baseUI.UpdateTransform();
	}
	else
	{
#ifdef DEBUG_HP
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);

		uiElement->m_Images[0].baseUI.SetPosition(DSFLOAT2(0.75f, 0.75f - (counter * 0.125f)));

		uiElement->m_BaseText.SetText(("Health: " + std::to_string(health->value)).c_str(), uiElement->m_Images[0].baseUI.GetBounds());
		uiElement->m_BaseText.baseUI.Setup(uiElement->m_Images[0].baseUI.GetPosition(), uiElement->m_Images[0].baseUI.GetScale(),
			uiElement->m_Images[0].baseUI.GetRotation(), uiElement->m_Images[0].baseUI.GetVisibility(), uiElement->m_Images[0].baseUI.GetOpacity());


		counter++;
#endif

	}
}

void UpdateSoulUI(const EntityID player)
{	
	auto pc = registry.GetComponent<PlayerComponent>(player);
	int	currentSouls = pc->GetSouls();

	auto souls = registry.GetComponent<UIPlayerSoulsComponent>(player);
	auto uiElement = registry.GetComponent<UIComponent>(player);

	souls->value = currentSouls;
		
	if (uiElement->m_Texts.size() > 0)
	{
		char temp[8] = "";
		sprintf(temp, /*"Health: %lld"*/"%ld", souls->value);
		uiElement->m_Texts[SOUL_TEXT].SetText(temp, uiElement->m_Texts[SOUL_TEXT].baseUI.GetBounds(), 15.f);
		DSFLOAT2 position = uiElement->m_Images[SOUL_STONE].baseUI.GetPosition();
		position.x += .017f;
		position.y -= .007f;
		uiElement->m_Texts[SOUL_TEXT].baseUI.Setup(position, uiElement->m_Images[SOUL_STONE].baseUI.GetScale(),
											uiElement->m_Images[SOUL_STONE].baseUI.GetRotation(), uiElement->m_Images[SOUL_STONE].baseUI.GetVisibility(), 
											uiElement->m_Images[SOUL_STONE].baseUI.GetOpacity());
	}
}

#include "Relics\RelicFunctions.h"

uint32_t AddNewRelicToUI(const EntityID player, const void* relic)
{
	auto uiElement = registry.GetComponent<UIComponent>(player);
	auto relicComponent = registry.GetComponent<UIPlayerRelicsComponent>(player);
	const RelicData* data = (const RelicData*)relic;
	DSFLOAT2 position = uiElement->m_Images[RC_ANCHOR].baseUI.GetPosition();

	uint32_t retVal = uiElement->m_Images.size();
	uint32_t latest = retVal;
	uiElement->AddImage(data->m_filePath, position, DSFLOAT2(1.0f, 1.0f), false);
	// Displace the image
	uiElement->m_Images[latest].baseUI.m_PixelCoords.y += 60 + 55 * relicComponent->currentRelics;
	uiElement->m_Images[latest].baseUI.m_PixelCoords.x += 5;
	_ADVANCED_HP_ROUND_PIXELS_UP(latest);	

	// Displace the overlay
	latest = uiElement->m_Images.size();
	uiElement->AddImage("RelicChain/RC_Link", position, DSFLOAT2(1.0f, 1.0f), false);
	uiElement->m_Images[latest].baseUI.m_PixelCoords.y += 56 + 55 * relicComponent->currentRelics;
	uiElement->m_Images[latest].baseUI.m_PixelCoords.x += 5;
	_ADVANCED_HP_ROUND_PIXELS_UP(latest);

	return retVal;
}