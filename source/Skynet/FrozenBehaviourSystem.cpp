#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Skynet\BehaviourHelper.h"
#include "ParticleComponent.h"
#include "Particles.h"
#include "Levels\LevelHelper.h"
#include "EventFunctions.h"

bool FrozenBehaviourSystem::Update()
{
	FrozenBehaviour* frozenComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	TransformComponent* frozenTransformComponent = nullptr;

	for (auto enemyEntity : View<FrozenBehaviour, TransformComponent, StatComponent, EnemyComponent>(registry))
	{
		frozenComponent = registry.GetComponent<FrozenBehaviour>(enemyEntity);
		frozenTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		bool thanosSnap = false;
		frozenComponent->frozenCounter += GetDeltaTime();

		//Rasmus was here, updating glow color and adding shatter
		GlowComponent* GC = registry.GetComponent<GlowComponent>(enemyEntity);
		if (GC)
		{
			GC->m_g -= (1 / frozenComponent->frozenTimeAmount) * GetDeltaTime();
			ShatterComponent* SC = registry.GetComponent<ShatterComponent>(enemyEntity);
			if (GC->m_g < 0.02f && !SC)
			{
				registry.AddComponent<ShatterComponent>(enemyEntity, 5);
				ModelBonelessComponent* model = nullptr;
				model = registry.GetComponent<ModelBonelessComponent>(enemyEntity);
				if (model != nullptr) model->shared.hasOutline = false;

				ModelSkeletonComponent* model2 = nullptr;
				model2 = registry.GetComponent<ModelSkeletonComponent>(enemyEntity);
				if (model2 != nullptr) model2->shared.hasOutline = false;
			}
			if (GC->m_g < 0.035f && frozenComponent->type == EnemyType::frozenEye)
			{
				//translates the eye back up so it looks better when switching from ice to normal
				float timeWindow = 0.035 * frozenComponent->frozenTimeAmount;
				float timeLeft = frozenComponent->frozenTimeAmount - frozenComponent->frozenCounter;
				frozenTransformComponent->positionY = (-8 / timeWindow) * timeLeft;
			}
		}
		
		if (frozenComponent->frozenCounter >= frozenComponent->frozenTimeAmount)
		{
			if (enemyStats->GetHealth() > 0)
			{
				thanosSnap = true;
				if (frozenComponent->type == EnemyType::frozenHellhound)
				{
					SetupEnemy(EnemyType::empoweredHellhound, frozenTransformComponent->positionX, frozenTransformComponent->positionY, frozenTransformComponent->positionZ,
						6969, 6969.f, 6969.f, 6969.f, 6969.f, 0);
				}
				else if (frozenComponent->type == EnemyType::frozenImp)
				{
					SetupEnemy(EnemyType::empoweredImp, frozenTransformComponent->positionX, frozenTransformComponent->positionY, frozenTransformComponent->positionZ,
						6969, 6969.f, 6969.f, 6969.f, 6969.f, 0);
				}
				else if (frozenComponent->type == EnemyType::frozenEye)
				{
					//Rasmus was here, making sure y is properly reset after lowering it in LevelHelper.cpp
					frozenTransformComponent->positionY = 0;

					SetupEnemy(EnemyType::eye, frozenTransformComponent->positionX, frozenTransformComponent->positionY, frozenTransformComponent->positionZ,
						6969, 6969.f, 6969.f, 6969.f, 6969.f, 0);
				}
			}
		}
		if (thanosSnap)
		{
			RemoveEnemy(enemyEntity, 1);
		}

	}

	return true;
}