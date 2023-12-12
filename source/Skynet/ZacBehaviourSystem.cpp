#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Skynet\BehaviourHelper.h"




bool ZacBehaviourSystem::Update()
{
	//First find the skynet component
	ZacBehaviour* zacComponent = nullptr;
	TransformComponent* zacTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	EnemyComponent* enmComp = nullptr;
	AnimationComponent* enemyAnim = nullptr;



	for (auto enemyEntity : View<ZacBehaviour, TransformComponent, StatComponent, EnemyComponent>(registry))
	{
		zacComponent = registry.GetComponent<ZacBehaviour>(enemyEntity);
		zacTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		enmComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);




		if (zacComponent != nullptr && enmComp != nullptr && /*enemyAnim != nullptr &&*/ enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			if (Calculate2dDistance(zacTransformComponent->positionX, zacTransformComponent->positionZ, zacComponent->targetX, zacComponent->targetZ) < 1.f)
			{
				zacTransformComponent->positionX = zacComponent->targetX;
				zacTransformComponent->positionZ = zacComponent->targetZ;
			}
			else
			{
				float faceX = zacComponent->targetX - zacTransformComponent->positionX;
				float faceZ = zacComponent->targetZ - zacTransformComponent->positionZ;
				//SmoothRotation(zacTransformComponent, goalDirectionX, goalDirectionZ, 35.f);

				float dirX = faceX, dirZ = faceZ;
				float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
				if (magnitude > 0.001f)
				{
					dirX /= magnitude;
					dirZ /= magnitude;
				}
				zacTransformComponent->facingX = dirX;
				zacTransformComponent->facingZ = dirZ;

				//TransformAccelerate(enemyEntity, dirX, dirZ);
				zacTransformComponent->positionX += dirX * GetDeltaTime() * enemyStats->GetSpeed();
				zacTransformComponent->positionZ += dirZ * GetDeltaTime()* enemyStats->GetSpeed();

				if (zacComponent->zacIndex == 1) //Leg animation
				{
					enemyAnim->aAnim = ANIMATION_WALK;
					enemyAnim->aAnimIdx = 0;
					enemyAnim->aAnimTimeFactor = 2;
					ANIM_BRANCHLESS(enemyAnim);
					enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
				}
				else //Other animations
				{
					enemyAnim->aAnim = ANIMATION_WALK;
					enemyAnim->aAnimIdx = 0;

					ANIM_BRANCHLESS(enemyAnim);
					enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
				}
			}
		}
	}


	return true;
}