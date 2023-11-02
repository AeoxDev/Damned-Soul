#pragma once

enum ENTITY_PERSISTENCY_TIER
{
	ENT_PERSIST_LOWEST, //Note to future self: If we end up needing another persist lower than this one, give herman a pat on the back
	ENT_PERSIST_LEVEL, //Persists between levels, for example player
	ENT_PERSIST_GAME, //Persists throughout the game, for example the audio engine
	ENT_PERSIST_HIGHEST // This is always meant to be the highest tier. No entity should have this tier really, its just meant to force a clear on everything
};

struct EntityID
{
	int index = -1;
	bool isDestroyed = false; //Boolean to check if the entity has been destroyed or not
	ENTITY_PERSISTENCY_TIER persistentTier = ENT_PERSIST_LOWEST; //will not get unloaded if above unload persistent tier.
};