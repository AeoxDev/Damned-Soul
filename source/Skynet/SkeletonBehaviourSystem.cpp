#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "DeltaTime.h"
#include <random>

float Calculate2dDistance(float pos1X, float pos1Z, float pos2X, float pos2Z)
{
	return sqrt((pos1X - pos2X) * (pos1X - pos2X) + (pos1Z - pos2Z) * (pos1Z - pos2Z));
}

void SmoothRotation(TransformComponent* ptc, float goalX, float goalZ)
{
	DirectX::XMVECTOR goalV = DirectX::XMVECTOR{ goalX, goalZ, 0.0f };
	DirectX::XMVECTOR length = DirectX::XMVector3Length(goalV);
	DirectX::XMFLOAT3 l;
	DirectX::XMStoreFloat3(&l, length);
	float angle = acosf(ptc->facingX);

	if (ptc->facingZ < 0.0f)
	{
		angle *= -1.0f;
	}

	if (l.x > 0.001f)
	{
		goalV = DirectX::XMVector2Normalize(goalV);

		DirectX::XMFLOAT3 goalFloats;
		DirectX::XMStoreFloat3(&goalFloats, goalV);
		goalX = goalFloats.x;
		goalZ = goalFloats.y;
		float goalAngle = acosf(goalX);
		if (goalZ < 0.0f)
		{
			goalAngle *= -1.0f;
		}
		//Check if shortest distance is right or left
		float orthogonalX = -ptc->facingZ;
		float orthogonalZ = ptc->facingX;
		//float dot = playerInputs[i].aimDirection.Dot(players[i].forward);
		float dot = goalX * ptc->facingX + goalZ * ptc->facingZ;
		float orthDot = goalX * orthogonalX + goalZ * orthogonalZ;
		if (orthDot > 0.0f)
		{//Om till vänster
			angle += GetDeltaTime() * (5.1f - dot);
		}
		else
		{
			angle -= GetDeltaTime() * (5.1f - dot);
		}
		ptc->facingX = cosf(angle);
		ptc->facingZ = sinf(angle);
		goalX = 0.0f;
		goalZ = 0.0f;
	}
}

void ChaseBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent)
{
	skeletonComponent->goalDirectionX = playerTransformCompenent->positionX - skeletonTransformComponent->positionX;
	skeletonComponent->goalDirectionZ = playerTransformCompenent->positionZ - skeletonTransformComponent->positionZ;
	
	SmoothRotation(skeletonTransformComponent, skeletonComponent->goalDirectionX, skeletonComponent->goalDirectionZ);
	float dirX = skeletonTransformComponent->facingX, dirZ = skeletonTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}


	//speed set to 10.0f, use enemy component later
	skeletonTransformComponent->positionX += dirX * 10.0f * GetDeltaTime();
	skeletonTransformComponent->positionZ += dirZ * 10.0f * GetDeltaTime();
}

void IdleBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, SkeletonBehaviour* skeletonComponent, TransformComponent* skeletonTransformComponent)
{
	skeletonComponent->timeCounter += GetDeltaTime();
	if (skeletonComponent->timeCounter >= skeletonComponent->updateInterval)
	{
		skeletonComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		skeletonComponent->goalDirectionX = randomX;
		skeletonComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
		skeletonComponent->updateInterval = randomInterval(gen);
	}
	
	SmoothRotation(skeletonTransformComponent, skeletonComponent->goalDirectionX, skeletonComponent->goalDirectionZ);

	
	//speed set to 10.0f, use enemy component later
	skeletonTransformComponent->positionX += skeletonTransformComponent->facingX * 5.0f * GetDeltaTime();
	skeletonTransformComponent->positionZ += skeletonTransformComponent->facingZ * 5.0f * GetDeltaTime();
	
}

void CombatBehaviour()
{

}

bool SkeletonBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	SkeletonBehaviour* skeletonComponent = nullptr;
	TransformComponent* skeletonTransformComponent = nullptr;
	
	for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	{
		playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
		playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
	}

	for (auto enemyEntity : View<SkeletonBehaviour, TransformComponent>(registry))
	{
		skeletonComponent = registry.GetComponent<SkeletonBehaviour>(enemyEntity);
		skeletonTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);

		if (skeletonComponent != nullptr && playerTransformCompenent!= nullptr && true )// check if enemy is alive, change later
		{
			float distance = Calculate2dDistance(skeletonTransformComponent->positionX, skeletonTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			if (distance < 2.5f)
			{
				CombatBehaviour();
			}
			else if (distance < 50) //hunting distance
			{
				ChaseBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent);
			}
			else // idle
			{
				IdleBehaviour(playerComponent, playerTransformCompenent, skeletonComponent, skeletonTransformComponent);
			}
		}
	}


	return true;
}