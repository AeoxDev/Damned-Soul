#pragma once

struct EntityID
{
	int index = -1;
	bool state = false; //Boolean to check if the entity has been destroyed or not
	int persistentTier = 0;//will not get unloaded if above unload persistent tier.
};