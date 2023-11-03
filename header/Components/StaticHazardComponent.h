#pragma once
#include <EntityID.h>
#include <cinttypes>

#define HAZARD_LAVA_UPDATE_TIME  0.25f
#define HAZARD_LAVA_DAMAGE 4
enum StaticHazardType
{
	NO_HAZARD_TYPE,
	HAZARD_LAVA = 2,
	HAZARD_CRACK = 3,
	HAZARD_ICE = 4
};

//This does not need to be in the same entity for GI to see it.
struct StaticHazardComponent
{
	StaticHazardType type;
};

void AddStaticHazard(EntityID& entity, const StaticHazardType& type = HAZARD_LAVA);
void AddStaticHazardTexture(EntityID& entity, char* crackTexture, char* lavaTexture, char* iceTexture);

//This needs to be in the same Entity as the stage for it to be seen by GI
struct StaticHazardTextureComponent
{
	int16_t crackTextureID = -1;
	int16_t lavaTextureID = -1;
	int16_t iceTextureID = -1;
};