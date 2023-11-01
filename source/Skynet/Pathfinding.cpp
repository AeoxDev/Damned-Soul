#include "Skynet\BehaviourHelper.h"

PathfindingMap CalculateGlobalMapValuesSkeleton(EntityID& mapID)
{
	GIMapData* mapGrid = nullptr;
	mapGrid = GetMapTexture(mapID); // get map from collision


	// 0 = non-walkable 
	// 1 = walkable
	// 2+ = lava

	PathfindingMap returnMap;
	for (int i = 0; i < GI_TEXTURE_DIMENSIONS; i++)
	{
		for (int j = 0; j < GI_TEXTURE_DIMENSIONS; j++)
		{
			returnMap.cost[i][j] = 0;
		}
	}

	for (int x = 0; x < GI_TEXTURE_DIMENSIONS; x++)
	{
		for (int z = 0; z < GI_TEXTURE_DIMENSIONS; z++)
		{
			// is it walkable?
			if (mapGrid->texture[x][z] == 0)
			{
				//not walkable, bad number
				returnMap.cost[x][z] = 10000;
			}
			else if (mapGrid->texture[x][z] == 1) // normal ground?
			{
				returnMap.cost[x][z] = 1;
			}
			else if (returnMap.cost[x][z] >= 2) // is the floor lava?
			{
				returnMap.cost[x][z] = 5;
			}
		}
	}
	
	for (auto enemyEntity : View<EnemyComponent, TransformComponent, StatComponent>(registry))
	{
		TransformComponent* enemyTransformCompenent = registry.GetComponent<TransformComponent>(enemyEntity);
		if (enemyTransformCompenent == nullptr)
			continue;
		
		// x z = functionCallFromElliot

		returnMap.cost[X][Z] += 8;
	}


	return returnMap;
}