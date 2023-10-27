#pragma once

enum ENTITY_PERSISTENCY_TIER
{
	ENT_PERSIST_BASIC,
	ENT_PERSIST_PLAYER,
	ENT_PERSIST_AUDIO,
	ENT_PERSIST_HIGHEST_TIER // This is always meant to be the highest tier. No entity should have this tier really, its just meant to force a clear on everything
};

struct EntityID
{
	int index = -1;
	bool state = false; //Boolean to check if the entity has been destroyed or not
	ENTITY_PERSISTENCY_TIER persistentTier = ENT_PERSIST_BASIC;//will not get unloaded if above unload persistent tier.
};