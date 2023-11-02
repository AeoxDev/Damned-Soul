#include "Skynet\BehaviourHelper.h"
#include "MemLib\ML_Array.hpp"
#include "MemLib\MemLib.hpp"

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
			else if (mapGrid->texture[x][z] >= 2) // is the floor lava?
			{
				returnMap.cost[x][z] = 40000; // this doesn't trigger yet. ELLIOT FIX gosh darn it
			}
		}
	}
	
	for (auto enemyEntity : View<EnemyComponent, TransformComponent, StatComponent>(registry))
	{
		TransformComponent* enemyTransformCompenent = registry.GetComponent<TransformComponent>(enemyEntity);
		if (enemyTransformCompenent == nullptr)
			continue;
		
		// x z = functionCallFromElliot
		GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(mapID);
		GridPosition pos = PositionOnGrid(GIcomponent, enemyTransformCompenent); // grid position of an AI, trust the math, we're engineers

		returnMap.cost[pos.x][pos.z] += 8;
	}


	return returnMap;
}


bool AreWeThereYet(int x, int z, Node goal)
{
	if (x == goal.x && z == goal.z)
	{
		return true;
	}
	return false;
}

bool IsCellValid(int x, int z)
{
	if (x >= 0 && x <= GI_TEXTURE_DIMENSIONS)
	{
		if (z >= 0 && z <= GI_TEXTURE_DIMENSIONS)
		{
			return true;
		}
	}
	return false;
}

float CalculateEuclideanDistance(int x, int z, Node goal)
{
	float dist = (sqrt((x - goal.x) * (x - goal.x)+ (z - goal.z) * (z - goal.z)));
	return dist;
}

Node CreateNewNode(PathfindingMap gridValues, int x, int z, Node parent)
{
	Node returnNode;
	returnNode.x = x;
	returnNode.z = z;
	returnNode.g = gridValues.cost[x][z];
	returnNode.parentX = parent.x;
	returnNode.parentZ = parent.z;
	return returnNode;
}

ML_Vector<Node> TracePath(Node endNode, Node goal, Node nodeMap[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS], Node start)
{
	ML_Vector<Node> theWay;
	ML_Vector<Node> theReverseWay; // trust me, we need this. temp

	Node tempNode = endNode;

	while (tempNode.g != 0)
	{
		theReverseWay.push_back(tempNode);
		tempNode = nodeMap[tempNode.parentX][tempNode.parentZ];
	}
	theWay.push_back(start);
	for (int i = theReverseWay.size() - 1; i >= 0; i--)
	{
		theWay.push_back(theReverseWay[i]);
	}
	return theWay;
}


ML_Vector<Node> CalculateAStarPath(EntityID& mapID, PathfindingMap gridValues, TransformComponent* enemyTransform, TransformComponent* playerTransform)
{
	Node start; 
	Node goal;
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(mapID); //just need GIcomp
	GridPosition enemyPos = PositionOnGrid(GIcomponent, enemyTransform); // grid position
	GridPosition playerPos = PositionOnGrid(GIcomponent, playerTransform); // grid position
	GridPosition tempPush; // used for pushing stuff, don't mind this one....but we need it

	start.x = enemyPos.x;
	start.z = enemyPos.z;
	goal.x = playerPos.x;
	goal.z = playerPos.z;

	

	//Node nodeMap[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS]; // this is for keeping track of all grid values
	PoolPointer<Node[GI_TEXTURE_DIMENSIONS]> nodeMap = MemLib::palloc(sizeof(Node) * GI_TEXTURE_DIMENSIONS * GI_TEXTURE_DIMENSIONS); // this is for keeping track of all grid values
	for (int x = 0; x < GI_TEXTURE_DIMENSIONS; x++)
	{
		for (int z = 0; z < GI_TEXTURE_DIMENSIONS; z++)
		{
			nodeMap[x][z].f = FLT_MAX;
			nodeMap[x][z].g = gridValues.cost[x][z];
			nodeMap[x][z].h = FLT_MAX;
			nodeMap[x][z].parentX = -1;
			nodeMap[x][z].parentZ = -1;
		}
	}


	ML_Vector<Node> returnVector; // this vector will be returned in the end
	if (AreWeThereYet(start.x, start.z, goal))
	{
		MemLib::pfree(nodeMap);
		return returnVector; // You're the destination, Harry!
	}
	ML_Vector<GridPosition> openList; // nodes that are "alive"
	bool closedList[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS] = { false }; // dead nodes we don't follow anymore, if true it is dead. if false, it is alive
	start.parentX = start.x;
	start.parentZ = start.z;
	start.f = 0.f;
	start.g = 0.f;
	start.h = 0.f;

	nodeMap[start.x][start.z] = start;
	tempPush.x = start.x;
	tempPush.z = start.z;
	openList.push_back(tempPush);

	while (!openList.size() == 0 ) //this is where A* starts
	{
		GridPosition currPos; //= *openList.begin();
		float cheapest = FLT_MAX;
		int index = 0;
		for (int i = 0; i < openList.size(); i++)
		{
			if (nodeMap[openList[i].x][openList[i].z].f < cheapest)
			{
				cheapest = nodeMap[openList[i].x][openList[i].z].f;
				currPos = openList[i];
				index = i;
			}
		}

		Node currentNode = nodeMap[currPos.x][currPos.z];
		 
		openList.erase(index);
	
		closedList[currentNode.x][currentNode.z] = true;
		/*
		Generating all the 8 successor of this cell

			 N.W   N   N.E
			   \   |   /
				\  |  /
			 W----Cell----E
				  / | \
				/   |  \
			 S.W    S   S.E

		N.W--> North-West  (i-1, j-1)
		N -->  North       (i, j-1)
		N.E--> North-East  (i+1, j-1)
		W -->  West           (i-1, j)
		Cell-->Popped Cell (i, j)
		E -->  East        (i+1, j)
		S.W--> South-West  (i-1, j+1)
		S -->  South       (i, j+1)
		S.E--> South-East  (i+1, j+1)
		*/
		

		

		//generating first Indiana Jones (explorer)
		if (IsCellValid(currentNode.x - 1, currentNode.z - 1)) // NW
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x - 1, currentNode.z - 1, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if(closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g

				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
			}
		}








		if (IsCellValid(currentNode.x, currentNode.z - 1)) // N
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x, currentNode.z - 1, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if (closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g

				
				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
				
			}
		}






		if (IsCellValid(currentNode.x + 1, currentNode.z - 1)) // NE 
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x + 1, currentNode.z - 1, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if (closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g

				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
			}
		}





		if (IsCellValid(currentNode.x - 1, currentNode.z)) // W
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x - 1, currentNode.z, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if (closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g

				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
			}
		}






		if (IsCellValid(currentNode.x + 1, currentNode.z)) // E
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x + 1, currentNode.z, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if (closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g

				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
			}
		}




		if (IsCellValid(currentNode.x - 1, currentNode.z + 1)) // SW
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x - 1, currentNode.z + 1, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if (closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g

				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
			}
		}





		if (IsCellValid(currentNode.x, currentNode.z + 1)) // S
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x, currentNode.z + 1, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if (closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g

				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
			}
		}




		if (IsCellValid(currentNode.x + 1, currentNode.z + 1)) // SE
		{
			Node newNode = CreateNewNode(gridValues, currentNode.x + 1, currentNode.z + 1, currentNode);
			if (AreWeThereYet(newNode.x, newNode.z, goal))
			{
				// goal found, code later
				returnVector = TracePath(newNode, goal, nodeMap, start);
				MemLib::pfree(nodeMap);
				return returnVector;
			}
			else if (closedList[newNode.x][newNode.z] == false && newNode.g < 10000)//10000 is what unwalkable gets
			{
				newNode.h = CalculateEuclideanDistance(newNode.x, newNode.z, goal);
				//calc total cost
				newNode.f = newNode.g + newNode.h; // top g
				if (nodeMap[newNode.x][newNode.z].f == FLT_MAX || nodeMap[newNode.x][newNode.z].f > newNode.f) // if not explored or we found a cheaper way
				{
					tempPush.x = newNode.x;
					tempPush.z = newNode.z;
					openList.push_back(tempPush);

					//update value of grid position 
					nodeMap[newNode.x][newNode.z] = newNode;
				}
			}
		}

	} // END OF WHILE LOOP 

			
	// could not find path, return empty vector
	// "This bitch empty, YEET"
	// - Ghengis Kahn, probably
	MemLib::pfree(nodeMap);
	return returnVector;
}