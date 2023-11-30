#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "Model.h"

bool NavigationSystem::Update()
{
	for (auto navigationEntity : View<NavigationTrashComponentYouMustAccept>(registry))
	{
		StatComponent* enemyStats = nullptr;
		float closestDistance = FLT_MAX;
		float directionX = 0.f;
		float directionZ = 0.f;
		bool showHelper = false;

		TransformComponent* playerTransformCompenent = nullptr;
		TransformComponent* enemyTransformCompenent = nullptr;
		for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
			playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);

		for (auto enemyEntity : View<TransformComponent, StatComponent, EnemyComponent, SkeletonBehaviour>(registry))
		{
			enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
			if (enemyStats->GetHealth() > 0.f && playerTransformCompenent != nullptr)
			{
				enemyTransformCompenent = registry.GetComponent<TransformComponent>(enemyEntity);
				float distance = sqrt((playerTransformCompenent->positionX - enemyTransformCompenent->positionX) * (playerTransformCompenent->positionX - enemyTransformCompenent->positionX) + (playerTransformCompenent->positionZ - enemyTransformCompenent->positionZ) * (playerTransformCompenent->positionZ - enemyTransformCompenent->positionZ));
				if (distance < closestDistance)
				{
					closestDistance = distance;
					directionX = enemyTransformCompenent->positionX - playerTransformCompenent->positionX;
					directionZ = enemyTransformCompenent->positionZ - playerTransformCompenent->positionZ;

					float magnitude = sqrt(directionX * directionX + directionZ * directionZ);
					if (magnitude < 0.001f)
					{
						magnitude = 0.001f;
					}
					directionX /= magnitude;
					directionZ /= magnitude;
					showHelper = true;
				}
			}
		}

		if (showHelper)
		{

			//visualize
			TransformComponent* entTransform = registry.GetComponent<TransformComponent>(navigationEntity);
			entTransform->positionX = playerTransformCompenent->positionX  +directionX * 4.69f;
			entTransform->positionZ = playerTransformCompenent->positionZ +directionZ * 4.69f;
			SmoothRotation(entTransform, -directionX, -directionZ, 40.f);
			
		}
		else
		{
			for (auto navigationEntity : View<NavigationTrashComponentYouMustAccept>(registry))
			{
				auto toAppend = registry.GetComponent<ModelBonelessComponent>(navigationEntity);
				if (toAppend != nullptr)
				{
					ReleaseModel(toAppend->model);
					registry.RemoveComponent<ModelBonelessComponent>(navigationEntity);
				}
				registry.DestroyEntity(navigationEntity);
			}
		}
	}
	return true;
}
