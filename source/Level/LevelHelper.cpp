#include "Levels\LevelHelper.h"
#include "Registry.h"
#include "Components.h"
#include "Hitbox.h"
#include "Model.h"

void SetupEnemy(EntityID& entity, enemyType eType, float positionX , float positionY , float positionZ , float mass , 
	float health , float moveSpeed , float damage, float attackSpeed , int soulWorth , float facingX ,
	float facingY , float facingZ , float scaleX , float scaleY, float scaleZ )
{

	TransformComponent transform;
	transform.facingX = facingX; transform.facingY = facingY; transform.facingZ = facingZ;
	transform.positionX = positionX; transform.positionY = positionY; transform.positionZ = positionZ;
	transform.mass = mass;
	transform.scaleX = scaleX; transform.scaleY = scaleY; transform.scaleZ = scaleZ;
	registry.AddComponent<TransformComponent>(entity, transform);

	registry.AddComponent<StatComponent>(entity, health, moveSpeed, damage, attackSpeed);
	registry.AddComponent<EnemyComponent>(entity, soulWorth);

	

	if (eType == enemyType::hellhound)
	{
		registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("HellhoundDummy_PH.mdl"));
		registry.AddComponent<HellhoundBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.3f);
	}
	else if (eType == enemyType::skeleton)
	{
		registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("SkeletonOneDymmy.mdl"));
		registry.AddComponent<SkeletonBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 0.9f);
	}
	else if (eType == enemyType::eye)
	{
		registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("FlyingEyeDymmy.mdl"));
		registry.AddComponent<EyeBehaviour>(entity);
		SetupEnemyCollisionBox(entity, 1.f, false);
	}
	else if (eType == enemyType::tempBoss)
	{
		registry.AddComponent<ModelBonelessComponent>(entity, LoadModel("PHBoss.mdl"));
		registry.AddComponent<TempBossBehaviour>(entity, 0, 0);
		SetupEnemyCollisionBox(entity, 1.4f * scaleX);
	}
}


