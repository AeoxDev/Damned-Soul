#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UIRenderer.h"
#include "CollisionFunctions.h" //AttackCollision
#include "Model.h"
#include "Levels\LevelHelper.h"
#include "Skynet\BehaviourHelper.h"
#include "UIComponents.h"
#include "States\StateManager.h"

#define BOSS_RESPAWN_TIME 8.f;



//void EnemyExclusion(EntityID& entity)
//{
//	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
//	stat->performingDeathAnimation = true; //so we don't trigger death animation twice
//} not needed

void PlayDeathAnimation(EntityID& entity, const int& index)
{
	RemoveHitbox(entity, 0);
	RemoveHitbox(entity, 1);
	RemoveHitbox(entity, 2);
	auto transform = registry.GetComponent<TransformComponent>(entity);
	float offset = float(rand() % 2);
	offset -= 0.5f;
	transform->positionX += offset * 0.02f;
	offset = float(rand() % 2);
	offset -= 0.5f;
	transform->positionZ += offset * 0.02f;

	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	if (anim)
	{
		anim->aAnim = ANIMATION_DEATH;
		anim->aAnimIdx = 0;
		anim->aAnimTime = GetTimedEventElapsedTime(entity, index);
	}
	ParticleComponent* pc = registry.GetComponent<ParticleComponent>(entity);
	if (pc != nullptr)
	{
		pc->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}

	//Temp: Remove the light if dog dies during its flamethrower attack
	RemoveLight(entity);
	EnemyComponent* enmComp = registry.GetComponent<EnemyComponent>(entity);
	SetHitboxActive(entity, enmComp->specialHitBoxID, false);
	SetHitboxCanDealDamage(entity, enmComp->specialHitBoxID, false);


}

void CreateMini(const EntityID& original, const float xSpawn, const float zSpawn, const int zacIndex, const float health)
{	
	EntityID newMini = registry.CreateEntity();


	//Get components of original boss
	auto transform = registry.GetComponent<TransformComponent>(original);
	auto bossStats = registry.GetComponent<StatComponent>(original);
	auto bossBehev = registry.GetComponent<TempBossBehaviour>(original);

	
	
	

	// now we need the speed for the blob, to make sure it ends up in middle after X seconds
	float dista = Calculate2dDistance(xSpawn, zSpawn, transform->positionX, transform->positionZ);
	//float dista = 15.f;
	float speeeeeed = dista / BOSS_RESPAWN_TIME;

	//Set stats of new boss based on original
	//float bossHP = bossStats->GetMaxHealth() / 2.f;
	float bossSpeed = speeeeeed /*bossStats->GetSpeed() / 2.f */;
	float bossDamage = bossStats->GetDamage();
	float bossAttackSpeed = bossStats->GetAttackSpeed();
	StatComponent* stat = registry.AddComponent<StatComponent>(newMini, health * 1.5f, bossSpeed, bossDamage, bossAttackSpeed );
	// change health depending on balance. health = original max health
	stat->hazardModifier = 0;
	stat->baseHazardModifier = 0;
	stat->lavaAccelFactor = 1.f;
	stat->lavaAnimFactor = 1.f;
	//Set transform
	TransformComponent transComp;
	float newScaleSize = 0.3f; // change as see fit
	//float offsetX = transform->facingX;
	//float offsetZ = -transform->facingZ;
	//float magnitude = sqrtf(offsetX * offsetX + offsetZ * offsetZ);
	//if (magnitude > 0.001f)
	//{
	//	offsetX /= magnitude;
	//	offsetZ /= magnitude;
	//}
	transComp.positionX = xSpawn;
	transComp.positionZ = zSpawn;
	transComp.scaleX = transform->scaleX * newScaleSize;
	transComp.scaleY = transform->scaleY * newScaleSize;
	transComp.scaleZ = transform->scaleZ * newScaleSize;
	
	
	transComp.mass = transform->mass;
	registry.AddComponent<TransformComponent>(newMini, transComp); 
	int soulWorth = 3;

	if (bossBehev->worthLess)
	{
		soulWorth = 1;
	}
	registry.AddComponent<EnemyComponent>(newMini, soulWorth, -1);
	registry.AddComponent<ModelBonelessComponent>(newMini, LoadModel("Skeleton.mdl"));

	//Sounds (Added by Joaquin)
	SoundComponent* scp = registry.AddComponent<SoundComponent>(newMini);
	scp->Load(MINIBOSS);

#ifdef DEBUG_HP
	// UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(newMini);
	UIGameHealthComponent* uiHealth = registry.AddComponent<UIGameHealthComponent>(newMini);
	uiElement->Setup("ExMenu/EmptyHealth", "", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
	uiElement->AddImage("ExMenu/FullHealth", DSFLOAT2(1.5f, 1.5f), DSFLOAT2(1.0f, 1.0f));
#endif

	////Set hitbox
	//float newScaleHitBox = 0.9f;
	//float mini = 1.f;
	//if (deathC >= 2)
	//{
	//	mini = 1.4f;
	//}
	//else if (deathC == 3)
	//{
	//	newScaleHitBox = 1.2f;
	//	mini = 1.5f;
	//}
	
	//SetupEnemyCollisionBox(newMini, GetHitboxRadius(original, bossBehev->hitBoxID) * newScaleHitBox);

	float radius = GetHitboxRadius(original, bossBehev->hitBoxID) /** newScaleHitBox*/;

	AddHitboxComponent(newMini);
	EnemyComponent* enemyComp = registry.GetComponent<EnemyComponent>(newMini);
	int hID = CreateHitbox(newMini, radius * /*mini **/ 0.7f, 0.f, 0.f);
	SetCollisionEvent(newMini, hID, HardCollision);
	SetHitboxIsEnemy(newMini, hID);
	SetHitboxHitPlayer(newMini, hID);
	SetHitboxHitEnemy(newMini, hID);
	SetHitboxActive(newMini, hID);
	SetHitboxIsMoveable(newMini, hID, false);

	int sID = CreateHitbox(newMini, radius, 0.f, 0.f);
	SetCollisionEvent(newMini, sID, SoftCollision);
	SetHitboxIsEnemy(newMini, sID);
	SetHitboxHitPlayer(newMini, sID);
	SetHitboxHitEnemy(newMini, sID);
	SetHitboxActive(newMini, sID);
	SetHitboxIsMoveable(newMini, sID, false);
	SetHitboxHitStaticHazard(newMini, sID, false);
	SetHitboxCanTakeDamage(newMini, sID, true);

	SetHitboxCanDealDamage(newMini, sID, false);

	enemyComp->attackHitBoxID = CreateHitbox(newMini, radius * 1.5f, 0.f, -1.5f);
	SetCollisionEvent(newMini, enemyComp->attackHitBoxID, AttackCollision);
	//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
	SetHitboxHitPlayer(newMini, enemyComp->attackHitBoxID);
	SetHitboxActive(newMini, enemyComp->attackHitBoxID);
	SetHitboxIsMoveable(newMini, enemyComp->attackHitBoxID, false);
	SetHitboxCanTakeDamage(newMini, enemyComp->attackHitBoxID, false);
	SetHitboxCanDealDamage(newMini, enemyComp->attackHitBoxID, false);


	registry.AddComponent<ZacBehaviour>(newMini, zacIndex, transform->positionX, transform->positionZ);
	
}

void CreateNewSplitZac(EntityID &ent, const int& index)
{
	TransformComponent* zacTransform = nullptr;
	zacTransform = registry.GetComponent<TransformComponent>(ent);
	bool zacIndex[5] = { false, false, false, false, false };
	bool shouldSpawn = false;
	bool worthless = true;
	for (auto enemyEntity : View<ZacBehaviour, TransformComponent, StatComponent, EnemyComponent>(registry))
	{
		StatComponent* enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
		if (enemyStats->GetHealth() > 0)
		{
			ZacBehaviour* zacComponent = registry.GetComponent<ZacBehaviour>(enemyEntity);
			zacIndex[zacComponent->zacIndex] = true;
			EnemyComponent* enemyComp = registry.GetComponent<EnemyComponent>(enemyEntity);
			if (enemyComp->soulCount == 3)
			{
				worthless = false;
			}
			enemyComp->soulCount = 0;
			RemoveEnemy(enemyEntity, 69);
			shouldSpawn = true;
		}
	}
	
	if (shouldSpawn)
	{
		SetupEnemy(EnemyType::tempBoss, zacTransform->positionX, 0.f, zacTransform->positionZ, 0, 6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 2.f, 2.f, 2.f,
			0.f, 0.f, -1.f, zacIndex[0], zacIndex[1], zacIndex[2], zacIndex[3], zacIndex[4], worthless);
	}

	registry.DestroyEntity(ent);
}

void SplitBoss(EntityID& entity, const int& index)
{
	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));
	CalculateGlobalMapValuesImp(valueGrid);
	TransformComponent* aiTransform = nullptr;
	aiTransform = registry.GetComponent<TransformComponent>(entity);
	TempBossBehaviour* tempBossComponent = nullptr;
	tempBossComponent = registry.GetComponent<TempBossBehaviour>(entity);
	StatComponent* originalStats = registry.GetComponent<StatComponent>(entity);

	float health = 0.f;
	int partsAlive = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (tempBossComponent->parts[i])
		{
			partsAlive++;
		}
	}
	health = (float)originalStats->GetMaxHealth(); // 40, 80, 120, 160 or 200
	health = health / (float)partsAlive;

	for (int i = 0; i < 5; ++i)
	{
		if (tempBossComponent->parts[i] )
		{
			TransformComponent tran = FindRetreatTile(valueGrid, aiTransform, 25.f, 45.f);
			CreateMini(entity, tran.positionX, tran.positionZ, i, health);
			CalculateGlobalMapValuesImp(valueGrid);
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		TransformComponent tran = FindRetreatTile(valueGrid, aiTransform, 25.f, 45.f);
		SetupEnemy(EnemyType::skeleton, tran.positionX, 0.f, tran.positionZ, 0);
		CalculateGlobalMapValuesImp(valueGrid);
	}



	free(valueGrid);
	EntityID trashEntity = registry.CreateEntity();
	TransformComponent* transformZac = registry.AddComponent<TransformComponent>(trashEntity);
	transformZac->positionX = aiTransform->positionX;
	transformZac->positionZ = aiTransform->positionZ;
	float time = (float)BOSS_RESPAWN_TIME;
	AddTimedEventComponentStartEnd(trashEntity, 0.0f, nullptr, time - 0.5f, CreateNewSplitZac, 0, 2);

	RemoveEnemy(entity, index);
}

void EnemyAttackFlash(EntityID& entity, const int& index)
{
	//Function runs when we pause the attack animation
	//Halfway through the pause we make enemy glow yellow, then we reset the color towards the end
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	if (skelel)
	{
		//Get enemytype
		uint32_t condition = GetTimedEventCondition(entity, index);

		if (GetTimedEventElapsedTime(entity, index) >= GetTimedEventTotalTime(entity, index) * 0.9f) //Reset before the attack
		{
			skelel->shared.ResetTempColor();
		}
		
		else if (condition == EnemyType::hellhound || condition == EnemyType::empoweredHellhound) //Hellhound glows immediately because there's no windup on the attack
		{
			skelel->shared.bcaR_temp += 0.8f;
			skelel->shared.bcaG_temp += 0.8f;
			skelel->shared.bcaB_temp += 0.5f;
		}

		else if (GetTimedEventElapsedTime(entity, index) >= GetTimedEventTotalTime(entity, index) * 0.5f) //Glow halfway through the pause
		{
			skelel->shared.bcaR_temp += 0.8f;
			skelel->shared.bcaG_temp += 0.8f;
			skelel->shared.bcaB_temp += 0.5f;
		}	
	}

	AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
	if (anim)
	{
		anim->aAnimTimeFactor = 0.0f; //If we get hit while our animation is paused, hitstop will do a quick pause of its own and reset aAnimTimeFactor back to 1 afterwards, and we don't want that
	}
}

void EnemyAttackGradient(EntityID& entity, const int& index)
{
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);

	if (skelel)
	{
		if (GetTimedEventElapsedTime(entity, index) >= GetTimedEventTotalTime(entity, index) * 0.95f) //Reset
		{
			//skelel->shared.colorAdditiveRed = 0.0f;
			//skelel->shared.colorAdditiveGreen = 0.0f;
			//skelel->shared.colorAdditiveBlue = 0.0f;
			/*skelel->shared.bcaR_temp = 0.0f;
			skelel->shared.bcaG_temp = 0.0f;
			skelel->shared.bcaB_temp = 0.0f;*/

			AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity); //Make animation faster because we're about to schwing
			if (anim)
				anim->aAnimTimeFactor = 2.0f;
		}
		else if (GetTimedEventElapsedTime(entity, index) >= GetTimedEventTotalTime(entity, index) * 0.375f) //Only start increasing gradient after 0.3 seconds
		{
			//skelel->shared.colorAdditiveRed  += GetDeltaTime();
			//skelel->shared.colorAdditiveGreen+= GetDeltaTime();
			//skelel->shared.colorAdditiveBlue += GetDeltaTime();
			skelel->shared.bcaR_temp += GetDeltaTime();
			skelel->shared.bcaG_temp += GetDeltaTime();
			skelel->shared.bcaB_temp += GetDeltaTime();
		}
	}
}

void EnemyAttack(EntityID& entity, const int& index)
{
	if (GetTimedEventElapsedTime(entity, index) >= GetTimedEventTotalTime(entity, index) * 0.95f) //End of the event
		EnemyEndAttack(entity, index);
	else if (GetTimedEventElapsedTime(entity, index) >= GetTimedEventTotalTime(entity, index) * 0.05f) //Start of the event
		EnemyBeginAttack(entity, index);
}

void EnemyBeginAttack(EntityID& entity, const int& index)
{
	//Activate attack hitbox
	EnemyComponent* comp = registry.GetComponent<EnemyComponent>(entity);
	if (comp)
	{
		SetHitboxActive(entity, comp->attackHitBoxID, true);
		SetHitboxCanDealDamage(entity, comp->attackHitBoxID, true); //why isn't this enabled by default
	}

	uint32_t condition = GetTimedEventCondition(entity, index);
	if (condition == EnemyType::hellhound || condition == EnemyType::empoweredHellhound) //Dogs do big knockback on their headbutt
	{
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		if(stats)
			stats->SetKnockbackMultiplier(8.0f);
	}
}

void EnemyEndAttack(EntityID& entity, const int& index)
{
	//Deactivate attack hitbox
	EnemyComponent* comp = registry.GetComponent<EnemyComponent>(entity);
	if (comp)
	{
		SetHitboxActive(entity, comp->attackHitBoxID, false);
		SetHitboxCanDealDamage(entity, comp->attackHitBoxID, false);
	}

	//Get enemytype
	uint32_t condition = GetTimedEventCondition(entity, index);

	if (condition == EnemyType::skeleton)
	{
		SkeletonBehaviour* skeleton = registry.GetComponent<SkeletonBehaviour>(entity);
		if (skeleton)
		{
			skeleton->attackTimer = 0.0f;

			//Play Sound Effect (Added by Joaquin)
			SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
			if (sfx)
			{
				for (auto audioEngine : View<AudioEngineComponent>(registry))
				{
					bool isPlaying = false;
					registry.GetComponent<AudioEngineComponent>(audioEngine)->channels[sfx->channelIndex[Channel_Base]]->isPlaying(&isPlaying);
					if(!isPlaying) sfx->Play(Skeleton_Attack, Channel_Base);
				}
			}
		}
	}

	else if (condition == EnemyType::tempBoss)
	{
		TempBossBehaviour* tempBoss = registry.GetComponent<TempBossBehaviour>(entity);
		if (tempBoss)
		{
			tempBoss->attackTimer = 0.0f;
			tempBoss->isAttacking = false;

			//Play Sound Effect (Added by Joaquin)
			SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
			if (sfx)
			{
				for (auto audioEngine : View<AudioEngineComponent>(registry))
				{
					bool isPlaying = false;
					registry.GetComponent<AudioEngineComponent>(audioEngine)->channels[sfx->channelIndex[Channel_Base]]->isPlaying(&isPlaying);
					if (!isPlaying) sfx->Play(Miniboss_Attack, Channel_Base);
				}
			}
		}

	}

	else if (condition == EnemyType::hellhound || condition == EnemyType::empoweredHellhound) //Reset big dog knockback after hit
	{
		HellhoundBehaviour* doggo = registry.GetComponent<HellhoundBehaviour>(entity);
		if (doggo)
			doggo->attackTimer = 0.0f;
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);
		if (stats)
			stats->SetKnockbackMultiplier(1.0f);
	}


	else if (condition == EnemyType::lucifer)
	{
		LuciferBehaviour* lucifer = registry.GetComponent<LuciferBehaviour>(entity);
		if (lucifer)
		{
			lucifer->attackTimer = 0.0f;
			lucifer->isAttacking = false;
		}
			
	}
}

void EnemyBecomeStunned(EntityID& entity, const int& index)
{
	//Find the enemycomponent and stun based on its values, start by getting condition to see what enemytype it is
	uint32_t condition = GetTimedEventCondition(entity, index);

	if (condition == EnemyType::skeleton)
	{
		SkeletonBehaviour* skeleton = registry.GetComponent<SkeletonBehaviour>(entity);
		if (skeleton != nullptr)
		{
			skeleton->attackStunDurationCounter = 0.0f;
		}
	}

	else if (condition == EnemyType::tempBoss)
	{
		TempBossBehaviour* tempBoss = registry.GetComponent<TempBossBehaviour>(entity);
		if (tempBoss != nullptr)
		{
			tempBoss->attackStunDurationCounter = 0.0f;
		}
	}

	else if (condition == EnemyType::hellhound || condition == EnemyType::empoweredHellhound)
	{
		HellhoundBehaviour* doggo = registry.GetComponent<HellhoundBehaviour>(entity);
		if (doggo != nullptr)
		{
			doggo->attackStunDurationCounter = 0.0f;
		}
	}

	else if (condition == EnemyType::lucifer)
	{
		LuciferBehaviour* lucifer = registry.GetComponent<LuciferBehaviour>(entity);
		if (lucifer != nullptr)
		{
			lucifer->attackStunDurationCounter = 0.0f;
		}
	}
}

void DogBeginWait(EntityID& entity, const int& index)
{

}

void DogEndWait(EntityID& entity, const int& index)
{
	HellhoundBehaviour* hc = registry.GetComponent<HellhoundBehaviour>(entity);
	if (hc)
	{
		hc->isWating = false;

		//SetInfiniteDirection() function from HellhoundBehaviourSystem
		TransformComponent* htc = registry.GetComponent<TransformComponent>(entity);
		if (htc)
		{
			float x = hc->lastPositionX - htc->positionX;
			float z = hc->lastPositionZ - htc->positionZ;
			float magnitude = (float)sqrt(x * x + z * z);
			if (magnitude > 0.001f)
			{
				x /= magnitude;
				z /= magnitude;
			}
			hc->cowardDirectionX = x;
			hc->cowardDirectionZ = z;
		}
		
		hc->retreat = true;
		hc->updatePathCounter = 20.f;
		hc->hasMadeADecision = false;
	}
	
}

void BossShockwaveStart(EntityID& entity, const int& index)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	SetHitboxActive(entity, enemy->specialHitBoxID, true);//Set false somewhere
	SetHitboxCanDealDamage(entity, enemy->specialHitBoxID, true);
	SetHitboxRadius(entity, enemy->specialHitBoxID, 0.0f);

	//Play sound effect (Added by Joaquin)
	if (enemy->type == EnemyType::tempBoss)
	{
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		if (sfx) sfx->Play(Miniboss_Shockwave, Channel_Base);
	}
	else if (enemy->type == EnemyType::minotaur)
	{
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		if (sfx) sfx->Play(Minotaur_Slam, Channel_Base);
	}
	else if (enemy->type == EnemyType::lucifer)
	{
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		if (sfx) sfx->Play(Boss_Slam, Channel_Base);
	}
}

void BossShockwaveExpand(EntityID& entity, const int& index)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	float radius = GetHitboxRadius(entity, enemy->specialHitBoxID);
	float growthSpeed = 30.0f;
	radius += GetDeltaTime() * growthSpeed;
	SetHitboxRadius(entity, enemy->specialHitBoxID, radius);
}

void BossShockwaveEnd(EntityID& entity, const int& index)
{
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	SetHitboxActive(entity, enemy->specialHitBoxID, false);//Set false somewhere
	SetHitboxCanDealDamage(entity, enemy->specialHitBoxID, false);

	ParticleComponent* pc = registry.GetComponent<ParticleComponent>(entity);
	if (pc != nullptr)
	{
		pc->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}
}

void BossSpawnwaveEnd(EntityID& entity, const int& index)
{
 	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);

	ParticleComponent* pc = registry.GetComponent<ParticleComponent>(entity);
	if (pc != nullptr)
	{
		pc->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}
}

void ChargeColorFlash(EntityID& entity, const int& index)
{
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);
	float frequency = 10.0f; //Higher frequency = faster flashing lights
	float cosineWave = cosf(GetTimedEventElapsedTime(entity, index) * frequency) * cosf(GetTimedEventElapsedTime(entity, index) * frequency);
	if (skelel)
	{
		skelel->shared.bcaR_temp += cosineWave;
		skelel->shared.bcaG_temp += cosineWave;
		//skelel->shared.colorAdditiveRed = cosineWave;
		//skelel->shared.colorAdditiveGreen = cosineWave;
	}
	if (bonel)
	{
		bonel->shared.bcaR_temp += cosineWave;
		bonel->shared.bcaG_temp += cosineWave;
		//bonel->shared.colorAdditiveRed = cosineWave;
		//bonel->shared.colorAdditiveGreen = cosineWave;
	}
}

void BossBlinkBeforeShockwave(EntityID& entity, const int& index)
{
	TempBossBehaviour* tempBoss = registry.GetComponent<TempBossBehaviour>(entity);
	if (tempBoss)
		tempBoss->isBlinking = true;

	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	if (skelel)
	{
		skelel->shared.bcaR_temp += 0.8f;
		skelel->shared.bcaG_temp += 0.8f;
		skelel->shared.bcaB_temp += 0.5f;
	}
}

void BossResetBeforeShockwave(EntityID& entity, const int& index)
{
	TempBossBehaviour* tempBoss = registry.GetComponent<TempBossBehaviour>(entity);
	if (tempBoss)
		tempBoss->isBlinking = false;

	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	if (skelel)
	{
		/*skelel->shared.colorAdditiveRed = 0.0f;
		skelel->shared.colorAdditiveGreen = 0.0f;
		skelel->shared.colorAdditiveBlue = 0.0f;*/
	}
}

void RemoveLandingIndicator(EntityID& entity, const int& index)
{
	if (entity.isDestroyed == true)
	{
		return;
	}
	registry.DestroyEntity(entity, ENT_PERSIST_HIGHEST);
}

void IncreaseLandingIndicatorMinotaur(EntityID& entity, const int& index)
{
	TransformComponent* landingTransform = registry.GetComponent<TransformComponent>(entity);
	landingTransform->positionY += 3.0f * GetDeltaTime();
}

void IncreaseLandingIndicatorLucifer(EntityID& entity, const int& index)
{
	TransformComponent* landingTransform = registry.GetComponent<TransformComponent>(entity);
	landingTransform->positionY += 5.0f * GetDeltaTime();
}

void CreateLandingIndicator(EntityID& entity, const int& index)
{
	TransformComponent* origin = registry.GetComponent<TransformComponent>(entity);
	int condition = GetTimedEventCondition(entity, index);
	EntityID landingSpot = registry.CreateEntity();
	TransformComponent* landingTransform = registry.AddComponent<TransformComponent>(landingSpot);
	landingTransform->positionX = origin->positionX;
	landingTransform->positionY = 1.0f;
	landingTransform->positionZ = origin->positionZ;

	CreateSpotLight(landingSpot, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 24.0f, 0.9f, 0.0f, -1.0f, 0.0f, 30);
	//AddTimedEventComponentStartEnd(landingSpot, 0.0f, nullptr, 2.0f, RemoveLandingIndicator);
	switch (condition)
	{
	case invalidType:
		break;
	case minotaur:
		AddTimedEventComponentStartContinuousEnd(landingSpot, 0.0f, nullptr, IncreaseLandingIndicatorMinotaur, 2.0f, RemoveLandingIndicator);
		break;
	case lucifer:
		AddTimedEventComponentStartContinuousEnd(landingSpot, 0.0f, nullptr, IncreaseLandingIndicatorLucifer, 2.5f, RemoveLandingIndicator);
		break;
	default:
		break;
	}
	
}

void PlayBossIntroVoiceLine(EntityID& entity, const int& index)
{
	SoundComponent* bossSound = registry.GetComponent<SoundComponent>(entity);
	//bossSound->Play();
	//Play sounds here
	int randomLine = rand();
}

void PlayBossIntroSlam(EntityID& entity, const int& index)
{
	//Play slam sound here when landing
	SoundComponent* bossSound = registry.GetComponent<SoundComponent>(entity);
	//bossSound->Play();
}

void PlayImpIntroTeleport(EntityID& entity, const int& index)
{
	SoundComponent* impSound = registry.GetComponent<SoundComponent>(entity);
	//impSound->Play();
}

void PlayImpIntroLaugh(EntityID& entity, const int& index)
{
	SoundComponent* impSound = registry.GetComponent<SoundComponent>(entity);
	//impSound->Play();
}

void PlayMinotaurIntroCharge(EntityID& entity, const int& index)
{
	SoundComponent* minotaurSound = registry.GetComponent<SoundComponent>(entity);
	//minotaurSound->Play();
}

void RemoveEnemy(EntityID& entity, const int& index)
{


	// Eat them souls
	for (auto player : View<PlayerComponent>(registry))
	{
		PlayerComponent* pc = registry.GetComponent<PlayerComponent>(player);
		EnemyComponent* ec = registry.GetComponent<EnemyComponent>(entity);
		if (ec != nullptr)
		{
			pc->UpdateSouls(ec->soulCount);
		}
 		
	}
	PlayerComponent* pc = registry.GetComponent<PlayerComponent>(entity);
	if (pc != nullptr)
	{
		registry.RemoveComponent<PlayerComponent>(entity);
	}
	// I am inevitable 
	// *le snap*
	auto toAppend = registry.GetComponent<ModelBonelessComponent>(entity);
	if (toAppend != nullptr)
	{
		ReleaseModel(toAppend->model);
		registry.RemoveComponent<ModelBonelessComponent>(entity);
	}


	auto toAppend2 = registry.GetComponent<ModelSkeletonComponent>(entity);
	if (toAppend2 != nullptr)
	{
		ReleaseModel(toAppend2->model);
		registry.RemoveComponent<ModelSkeletonComponent>(entity); 
	}

	SoundComponent* s = registry.GetComponent<SoundComponent>(entity);
	if (s != nullptr)
	{
		s->Unload();
	}

	TimedEventComponent* timed = registry.GetComponent<TimedEventComponent>(entity);
	if (timed != nullptr)
	{
		ReleaseTimedEvents(entity);
	}

	if (entity.index != -1)
	{
		registry.DestroyEntity(entity, ENT_PERSIST_HIGHEST);
	}
	
}

void RemoveCutsceneEnemy(EntityID& entity, const int& index)
{
	if (entity.index != -1)
	{
		registry.DestroyEntity(entity, ENT_PERSIST_HIGHEST);
	}
}

void SpawnMainMenuEnemy(EntityID& entity, const int& index)
{
	int condition = GetTimedEventCondition(entity, index);
	
	RandomPlayerEnemy((EnemyType)condition);

	//switch (condition)
	//{
	//case invalidType:
	//	break;
	//case hellhound:
	//	RandomPlayerEnemy(hellhound);
	//	break;
	//case skeleton:
	//	RandomPlayerEnemy(skeleton);
	//	break;
	//case eye:
	//	RandomPlayerEnemy(eye);
	//	break;
	//case imp:
	//	RandomPlayerEnemy(imp);
	//	break;
	//case tempBoss:
	//	RandomPlayerEnemy(tempBoss);
	//	break;
	//default:
	//	break;
	//}
}

void LoopSpawnMainMenuEnemy(EntityID& entity, const int& index)
{
	int rarity = 0;
	EnemyType type = skeleton;
	rarity = rand() % 16;
	if (rarity == 0)
	{
		type = hellhound;
	}
	rarity = rand() % 16;
	if (rarity == 0)
	{
		type = imp;
	}
	rarity = rand() % 64;
	if (rarity == 0)
	{
		type = eye;
	}
	rarity = rand() % 64;
	if (rarity == 0)
	{
		type = minotaur;
	}
	rarity = rand() % 64;
	if (rarity == 0)
	{
		type = empoweredSkeleton;
	}
	rarity = rand() % 64;
	if (rarity == 0)
	{
		type = empoweredImp;
	}
	rarity = rand() % 64;
	if (rarity == 0)
	{
		type = empoweredSkeleton;
	}
	rarity = rand() % 4096;
	if (rarity == 0)
	{
		type = tempBoss;
	}
	rarity = rand() % 4096;
	if (rarity == 0)
	{
		type = lucifer;
	}
	float time = 0.05f * (float)(rand() % 1024);
	AddTimedEventComponentStartEnd(entity, 0.0f, SpawnMainMenuEnemy,time + 1.0f, LoopSpawnMainMenuEnemy, (unsigned)type, 2);
}

void DestroyAcidHazard(EntityID& entity, const int& index)
{
	if (entity.isDestroyed == true)
	{
		return;
	}
	//ModelBonelessComponent* model = registry.GetComponent<ModelBonelessComponent>(entity);
	//if (model != nullptr)
	//{
	//	ReleaseModel(model->model);
	//	registry.RemoveComponent<ModelBonelessComponent>(entity);
	//}

	//Remove Particle
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr)
	{
		particle->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}

	RemoveHitbox(entity, 0);
	registry.DestroyEntity(entity, ENT_PERSIST_HIGHEST);
}

void CreateAcidHazard(EntityID& entity, const int& index)
{
	TransformComponent* origin = registry.GetComponent<TransformComponent>(entity);
	
	EntityID acidHazard = registry.CreateEntity();
	//ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(acidHazard, LoadModel("LavaPlaceholder.mdl"));
	//hazardModel->shared.colorAdditiveRed = 0.1f;
	//hazardModel->shared.colorAdditiveGreen = 0.9f;
	//hazardModel->shared.colorAdditiveBlue = 0.2f;
	//hazardModel->shared.gammaCorrection = 1.5f;
	//hazardModel->castShadow = false;

	ParticleComponent* particle = registry.AddComponent<ParticleComponent>(acidHazard, 2.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 2, VFX_PATTERN::ACIDGROUND);


	float scaling = 5.0f;

	TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(acidHazard);
	hazardTransform->positionX = origin->positionX;
	hazardTransform->positionY = 0.2f;
	hazardTransform->positionZ = origin->positionZ;
	hazardTransform->scaleX = scaling;
	hazardTransform->scaleY = 1.0f;
	hazardTransform->scaleZ = scaling;
	hazardTransform->facingX = cosf((float)rand());
	hazardTransform->facingZ = sinf((float)rand());
	AddStaticHazard(acidHazard, HAZARD_ACID);

	registry.AddComponent<StaticHazardComponent>(acidHazard, StaticHazardType::HAZARD_ACID);

	float radius = 5.0f;
	AddHitboxComponent(acidHazard);
	int hitboxID = CreateHitbox(acidHazard, radius * 0.5f, 0.f, 0.f);
	SetCollisionEvent(acidHazard, hitboxID, HazardAttackCollision);
	SetHitboxHitPlayer(acidHazard, hitboxID);
	SetHitboxHitEnemy(acidHazard, hitboxID);
	SetHitboxActive(acidHazard, hitboxID);
	SetHitboxCanDealDamage(acidHazard, hitboxID, true);
	
	AddTimedEventComponentStartEnd(acidHazard, 5.0f, DestroyAcidHazard, 5.5f, EndDestroyProjectile, 0, 2);//Double destroy hazard for bug fixing.
}

void BeginDestroyProjectile(EntityID& entity, const int& index)
{
	ProjectileComponent* proj = registry.GetComponent<ProjectileComponent>(entity);
	
	if (entity.isDestroyed == true)
	{
		return;
	}

	////Remove Model
	//ModelBonelessComponent* model = registry.GetComponent<ModelBonelessComponent>(entity);
	//if (model != nullptr)
	//{
	//	ReleaseModel(model->model);
	//	registry.RemoveComponent<ModelBonelessComponent>(entity);
	//}
	
	//Remove Particle
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr)
	{
		particle->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}

	//Remove shadow light
	RemoveLight(entity);

	if (proj->type == eye)
	{
		CreateAcidHazard(entity, index);
		proj->type = imp;
	}
		

	RemoveHitbox(entity, 0);
	RemoveHitbox(entity, 1);
}

void EndDestroyProjectile(EntityID& entity, const int& index)
{
	if (entity.isDestroyed == true)
	{
		return;
	}
	registry.DestroyEntity(entity, ENT_PERSIST_HIGHEST);
}

void SpawnProjectileShadow(EntityID& entity, const int& index)
{
	CreateSpotLight(entity, -0.5f, -0.5f, -0.5f, 0.0f, 6.0f, -1.0f, 24.0f, 0.9f, 0.0f, -1.0f, 0.0f, 20);
}
