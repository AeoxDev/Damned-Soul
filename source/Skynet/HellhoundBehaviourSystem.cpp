#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include <random>
#include "Skynet\BehaviourHelper.h"
#include "ParticleComponent.h"
#include "Particles.h"



void CombatBehaviour(HellhoundBehaviour* hc, StatComponent* enemyStats, StatComponent* playerStats, TransformComponent* ptc, TransformComponent* htc, EntityID& ent, EnemyComponent* enmComp, AnimationComponent* animComp)
{
	hc->attackTimer += GetDeltaTime() * animComp->aAnimTimeFactor;
	hc->goalDirectionX = ptc->positionX - htc->positionX;
	hc->goalDirectionZ = ptc->positionZ - htc->positionZ;
	
	animComp->aAnim = ANIMATION_ATTACK;
	animComp->aAnimIdx = 0;
	//Elliot: Change in calculations for attack timer:
	animComp->aAnimTime = 0.5f * hc->attackTimer / (0.0001f + enemyStats->GetAttackSpeed());
	ANIM_BRANCHLESS(animComp);
	
	//impose timer so they cannot run and hit at the same time (frame shit) also not do a million damage per sec
	if (hc->attackTimer >= enemyStats->GetAttackSpeed()) // yes, we can indeed attack. 
	{
		enemyStats->SetKnockbackMultiplier(8.0f);
		SetHitboxActive(ent, enmComp->attackHitBoxID, true);
		SetHitboxCanDealDamage(ent, enmComp->attackHitBoxID, true);
		
		hc->attackTimer = 0;
		hc->attackStunDurationCounter = 0;
		
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(ent);
		sfx->Play(Hellhound_Attack, Channel_Base);
		RedrawUI();
	}
}
//
//void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, HellhoundBehaviour* hc, TransformComponent* htc, StatComponent* enemyStats)
//{
//	float relativePosX = ptc->positionX - htc->positionX;
//	float relativePosZ = ptc->positionZ - htc->positionZ;
//
//	float relativeDirectionX = ptc->facingX - htc->facingX;
//	float relativeDirectionZ = ptc->facingZ - htc->facingZ;
//
//	//this will be used to determine if we are exactly
//	 // a = spelare. b = hellhound
//	float playerToHellhoundX = htc->positionX - ptc->positionX;
//	float playerToHellhoundZ = htc->positionZ - ptc->positionZ;
//	float behindDot = playerToHellhoundX * ptc->facingX + playerToHellhoundZ * ptc->facingZ;
//	float magHellhound = sqrt(playerToHellhoundX * playerToHellhoundX + playerToHellhoundZ * playerToHellhoundZ);
//	float magPlayer = sqrt(ptc->facingX * ptc->facingX + ptc->facingZ * ptc->facingZ);
//
//	float tolerance = 0.7f; // THIS IS FOR ANGLE SMOOTHING
//	if (std::abs((behindDot / (magHellhound * magPlayer) + 1)) < tolerance) // are we behind player back? (trust the magic math, please)
//	{
//		hc->isBehind = true;
//		hc->isBehindCounter += GetDeltaTime();
//	}
//	else
//	{
//		ResetHellhoundVariables(hc, true, true);
//	}
//
//
//	float dot = relativePosX * relativeDirectionZ - relativePosZ * relativeDirectionX;
//
//
//	float magnitude = 0.f;
//	float dirX = 0.f;
//	float dirZ = 0.f;
//	if (!hc->circleBehaviour)
//	{
//		hc->circleBehaviour = true;
//		if (dot < 0) // clockwise
//		{
//			hc->clockwiseCircle = true;
//		}
//		else // counter clockwise
//		{
//			hc->clockwiseCircle = false;
//		}
//	}
//
//	if (hc->clockwiseCircle) //clockwise
//	{
//		dirX = -hc->goalDirectionZ;
//		dirZ = hc->goalDirectionX;
//		
//	}
//	else // counter clockwise
//	{
//		dirX = hc->goalDirectionZ;
//		dirZ = -hc->goalDirectionX;
//	}
//	magnitude = sqrt(dirX * dirX + dirZ * dirZ);
//	SmoothRotation(htc, dirX, dirZ, 40.f);
//	if (magnitude > 0.001f)
//	{
//		dirX /= magnitude;
//		dirZ /= magnitude;
//	}
//	htc->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
//	htc->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
//	hc->goalDirectionX = ptc->positionX - htc->positionX;
//	hc->goalDirectionZ = ptc->positionZ - htc->positionZ;
//}


void ChaseBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, HellhoundBehaviour* hellhoundComponent, TransformComponent*  hellhoundTransformComponent, StatComponent* enemyStats, AnimationComponent* enemyAnim, float goalDirectionX, float goalDirectionZ, bool path)
{
	if (path)
	{
		hellhoundComponent->goalDirectionX = goalDirectionX;
		hellhoundComponent->goalDirectionZ = goalDirectionZ;
	}
	else
	{
		hellhoundComponent->goalDirectionX = playerTransformCompenent->positionX - hellhoundTransformComponent->positionX;
		hellhoundComponent->goalDirectionZ = playerTransformCompenent->positionZ - hellhoundTransformComponent->positionZ;
	}




	enemyAnim->aAnim = hellhoundComponent->charge ? ANIMATION_ATTACK : ANIMATION_WALK;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor * (1 + hellhoundComponent->charge);
	ANIM_BRANCHLESS(enemyAnim);

	hellhoundComponent->goalDirectionX = playerTransformCompenent->positionX - hellhoundTransformComponent->positionX;
	hellhoundComponent->goalDirectionZ = playerTransformCompenent->positionZ - hellhoundTransformComponent->positionZ;

	SmoothRotation(hellhoundTransformComponent, hellhoundComponent->goalDirectionX, hellhoundComponent->goalDirectionZ, 35.f);
	float dirX = hellhoundTransformComponent->facingX, dirZ = hellhoundTransformComponent->facingZ;
	float magnitude = sqrt(dirX * dirX + dirZ * dirZ);
	if (magnitude > 0.001f)
	{
		dirX /= magnitude;
		dirZ /= magnitude;
	}

	//speed set to 10.0f, use enemy component later
	float speedMultiplier = 1.f;
	if (hellhoundComponent->charge)
	{
		hellhoundComponent->chargeCounter += GetDeltaTime();
		if (hellhoundComponent->chargeCounter >= hellhoundComponent->chargeTimeLimit)
		{
			hellhoundComponent->hasMadeADecision = false;
			hellhoundComponent->charge = false;
			hellhoundComponent->chargeCounter = 0.f;
			hellhoundComponent->retreat = true;
		}
		speedMultiplier = 2.5f; 
		enemyStats->SetSpeedMult(2.5f);
		if (enemyStats->m_acceleration == enemyStats->m_baseAcceleration)
		{
			enemyStats->m_acceleration = 4.0f;
		}
		
	}
	//hellhoundTransformComponent->positionX += dirX * enemyStats->GetSpeed() * speedMultiplier * GetDeltaTime();
	//hellhoundTransformComponent->positionZ += dirZ * enemyStats->GetSpeed() * speedMultiplier * GetDeltaTime();
	TransformAccelerate(enemy, dirX * speedMultiplier, dirZ * speedMultiplier);
}

void IdleBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, HellhoundBehaviour* hellhoundComponent, TransformComponent* hellhoundTransformComponent, StatComponent* enemyStats, AnimationComponent* enemyAnim)
{
	enemyAnim->aAnim = ANIMATION_WALK;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
	ANIM_BRANCHLESS(enemyAnim);

	hellhoundComponent->timeCounter += GetDeltaTime();

	bool okayDirection = false;
	while (!okayDirection)
	{
		if (hellhoundComponent->timeCounter >= hellhoundComponent->updateInterval)
		{
			hellhoundComponent->timeCounter = 0.f;
			std::random_device rd;
			std::mt19937 gen(rd());
			// Define a uniform distribution for the range [-1.0, 1.0]
			std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
			float randomX = distribution(gen);
			float randomZ = distribution(gen);
			hellhoundComponent->goalDirectionX = randomX;
			hellhoundComponent->goalDirectionZ = randomZ;
			std::uniform_real_distribution<float> randomInterval(0.6f, 1.2f);
			hellhoundComponent->updateInterval = randomInterval(gen);
		}

		SmoothRotation(hellhoundTransformComponent, hellhoundComponent->goalDirectionX, hellhoundComponent->goalDirectionZ, 35.1f);
		float oldX = hellhoundTransformComponent->positionX;
		float oldZ = hellhoundTransformComponent->positionZ;
		float bias = 1.f;


		//hellhoundTransformComponent->positionX += hellhoundTransformComponent->facingX * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
		//hellhoundTransformComponent->positionZ += hellhoundTransformComponent->facingZ * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
		TransformAccelerate(enemy, hellhoundTransformComponent->facingX * 0.5f, hellhoundTransformComponent->facingZ * 0.5f);
		if ((hellhoundTransformComponent->positionX >= oldX + bias || hellhoundTransformComponent->positionZ >= oldZ + bias) && hellhoundTransformComponent->positionX <= oldX - bias || hellhoundTransformComponent->positionZ <= oldZ - bias)
		{
			//not good direction
			hellhoundTransformComponent->positionX = oldX;
			hellhoundTransformComponent->positionZ = oldZ;
			hellhoundComponent->timeCounter = hellhoundComponent->updateInterval + 1.f;
		}
		else
		{
			// good direction
			okayDirection = true;
		}
	}
}

void FixShootingTargetPosition(TransformComponent* ptc, TransformComponent* htc, HellhoundBehaviour* hc, EntityID& dog)
{	
	hc->isShooting = true;
	//Temp: Create SMALL spotlight when dog prepares to flame
	
	CreateSpotLight(dog, 8.0f, 4.0f, 1.0f,
		0.0f, 1.0f, -0.25f,
		hc->offsetForward + 1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 33.0f);
	hc->currentShootingAttackRange = 1.f;
	SoundComponent* sfx = registry.GetComponent<SoundComponent>(dog);
	sfx->Play(Hellhound_Inhale, Channel_Base);
	//from hound  to player
	float dx = ptc->positionX - htc->positionX;
	float dz = ptc->positionZ - htc->positionZ;
	//Set the facing direction towards the player
	hc->facingX = dx;
	hc->facingZ = dz;
	float dist = sqrtf(dx * dx + dz * dz);
	htc->facingX = dx / dist;
	htc->facingZ = dz / dist;

	
	float magnitude = sqrt(dx * dx + dz * dz);
	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
	}

	float orthoX = -dz;
	float orthoZ = dx;

	dx /= magnitude;
	dz /= magnitude;


	float targetX = htc->positionX + dx * hc->offsetForward;
	float targetZ = htc->positionZ +  dz  * hc->offsetForward;
	magnitude = sqrt(orthoX * orthoX + orthoZ * orthoZ);

	if (magnitude < 0.001f)
	{
		magnitude = 0.001f;
	}

	orthoX /= magnitude;
	orthoZ /= magnitude;

	hc->shootingSideTarget1X = targetX + orthoX * hc->offsetSide;
	hc->shootingSideTarget1Z = targetZ + orthoZ * hc->offsetSide;
	hc->shootingSideTarget2X = targetX - orthoX * hc->offsetSide;
	hc->shootingSideTarget2Z = targetZ - orthoZ * hc->offsetSide;
	hc->shootingStartX = htc->positionX;
	hc->shootingStartZ = htc->positionZ;
}

bool IsPlayerHitByFlameThrower(float p1X, float p1Z, float p2X, float p2Z, float p3X, float p3Z, float playerX, float playerZ)
{
	float denominator = ((p2Z - p3Z) * (p1X - p3X) + (p3X - p2X) * (p1Z - p3Z));
	float alpha = ((p2Z - p3Z) * (playerX - p3X) + (p3X - p2X) * (playerZ - p3Z)) / denominator;
	float beta = ((p3Z - p1Z) * (playerX - p3X) + (p1X - p3X) * (playerZ - p3Z)) / denominator;
	float gamma = 1.0f - alpha - beta;

	return alpha >= 0 && beta >= 0 && gamma >= 0 && alpha + beta + gamma == 1.0;
}

void ShootingBehaviour( TransformComponent* ptc, HellhoundBehaviour* hc, StatComponent* enemyStats, StatComponent* playerStats, EntityID& dog, EnemyComponent* enemy/*, TransformComponent* testing, TransformComponent* testing2*/)
{
	// create a hitbox from doggo position, as a triangle, with 2 corners expanding outwards with the help of variables in behavior. 
	//once max range has been reached, this function will reset all stats and let the doggo go on with a different behavior. 
	// each check until then will create a slightly longer (not wider) triangle hit box to simulate a flamethrower. Each check, the player will take damage if it
	// make sure to balance the damage so it's not a fast one-shot thing

	AnimationComponent* enemyAnim = registry.GetComponent<AnimationComponent>(dog);
	enemyAnim->aAnim = ANIMATION_ATTACK;
	enemyAnim->aAnimIdx = 1;
	enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
	ANIM_BRANCHLESS(enemyAnim);

	hc->currentShootingAttackRange += GetDeltaTime() * hc->shootingAttackSpeedForHitbox * (float)(hc->currentShootingAttackRange < hc->offsetForward); //updates the range of the "flamethrower"
	hc->shootingTimer += GetDeltaTime();
	//Temp: Create BIG spotlight when dog flame
		//Temp: Create point light to indicate that we're going to do flamethrower
	CreateSpotLight(dog, 10.0f, 0.0f, 0.0f,
		0.0f, 1.0f, -0.25f,
		hc->currentShootingAttackRange + 1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 33.0f);

	
	//auto tempTransform = registry.AddComponent<TransformComponent>(tempEntity, ptc);
	float  cornersX[3] = {0.0f, hc->currentShootingAttackRange * (hc->offsetSide / hc->offsetForward), -hc->currentShootingAttackRange * (hc->offsetSide / hc->offsetForward) };//Counter clockwise
	float  cornersZ[3] = {-1.0f, -hc->currentShootingAttackRange, -hc->currentShootingAttackRange};//Counter clockwise
	SetHitboxCorners(dog,enemy->specialHitBoxID, 3, cornersX, cornersZ);
	SetHitboxCanDealDamage(dog, enemy->specialHitBoxID, false);//Reset hitbox
	SetHitboxActive(dog, enemy->specialHitBoxID, true);
	SetHitboxCanDealDamage(dog, enemy->specialHitBoxID, true);

	if (registry.GetComponent<ParticleComponent>(dog) == nullptr)
	{
		registry.AddComponent<ParticleComponent>(dog, 1.0f, 0.0f, 0.5f,
			0.0f, 2.5f, 3.0f, 0.0f,
			cornersX[0], cornersZ[0], cornersX[1], cornersZ[1], cornersX[2], cornersZ[2], FLAMETHROWER);
	}
	else
	{
		ParticleComponent* pc = registry.GetComponent<ParticleComponent>(dog);
		Particles::UpdateMetadata(pc->metadataSlot, cornersX[0], cornersZ[0], cornersX[1], cornersZ[1], cornersX[2], cornersZ[2]);
	}

	//if (IsPlayerHitByFlameThrower(hc->shootingStartX, hc->shootingStartZ, hc->shootingSideTarget1X, hc->shootingSideTarget1Z, hc->shootingSideTarget2X, hc->shootingSideTarget2Z, ptc->positionX, ptc->positionZ))
	//{
	//	// player is inside max triangle, is player within range ?
	//	float dx = ptc->positionX - hc->shootingStartX;
	//	float dz = ptc->positionZ - hc->shootingStartZ;
	//	float distance = sqrt(dx * dx + dz * dz);
	//	if (distance <= hc->currentShootingAttackRange)
	//	{
	//		//yes, player should get hit. Take damage
	//		playerStats->UpdateHealth(-hc->flameDamage, true); // DEFINITELY MODIFY THIS LATER, very likely too much damage
	//		RedrawUI();
	//	}
	//}


	//end of function. check if current range >= max, that would end the attacks
	//Elliot: Use timer instead of range check
	if (hc->shootingTimer >= hc->shootingMaxTime)
	{
		hc->isShooting = false;
		hc->flameSoundsStartedPlaying = false;
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(dog);
		sfx->Stop(Channel_Base);
		hc->shootingCounter = 0.0f;
		hc->shootingCooldownCounter = 0.0f;
		hc->currentShootingAttackRange = 1.f;
		SetHitboxActive(dog, enemy->specialHitBoxID, false);
		SetHitboxCanDealDamage(dog, enemy->specialHitBoxID, false);
		//Recreate light again
		CreatePointLight(dog, 0.7f, 0.7f, 0.7f, 0.0f, 0.5f, 0.0f, 2.0f, 1.0f);
		hc->shootingTimer = 0.0f;

		ParticleComponent* pc = registry.GetComponent<ParticleComponent>(dog);
		if (pc!= nullptr)
		{
			pc->Release();
			registry.RemoveComponent<ParticleComponent>(dog);
		}
		hc->hasMadeADecision = false;
		hc->retreat = true;
	}
}

void SetInfiniteDirection(TransformComponent* htc, HellhoundBehaviour* hc)
{
	float x = hc->lastPositionX - htc->positionX;
	float z = hc->lastPositionZ - htc->positionZ;
	float magnitude = sqrt(x * x + z * z);
	if (magnitude > 0.001f)
	{
		x /= magnitude;
		z /= magnitude;
	}
	hc->cowardDirectionX = x;
	hc->cowardDirectionZ = z;
}


void TacticalRetreatBehaviour(EntityID& enemy, TransformComponent* htc, HellhoundBehaviour* hc, StatComponent* enemyStats, AnimationComponent* enemyAnim)
{
	enemyAnim->aAnim = ANIMATION_WALK;
	enemyAnim->aAnimIdx = 0;
	enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
	ANIM_BRANCHLESS(enemyAnim);

	float newGoalX = htc->positionX + hc->cowardDirectionX * 100.f;
	float newGoalZ = htc->positionZ + hc->cowardDirectionZ * 100.f; 
	SmoothRotation(htc, newGoalX, newGoalZ, 35.f);

	//htc->positionX += hc->cowardDirectionX * enemyStats->GetSpeed() * GetDeltaTime();
	//htc->positionZ += hc->cowardDirectionZ * enemyStats->GetSpeed() * GetDeltaTime();
	enemyStats->SetSpeedMult(1.0f);
	TransformAccelerate(enemy, hc->cowardDirectionX, hc->cowardDirectionZ);
	hc->cowardCounter += GetDeltaTime();

	if (hc->cowardCounter >= hc->cowardDuration) // have we been tactically retreating long enough?
	{
		hc->cowardCounter = 0.f;
		hc->retreat = false;
		hc->charge = false;
	}
}

void MakeBehaviourDecision(TransformComponent* htc, TransformComponent* ptc, HellhoundBehaviour* hc)
{
	float playerToHellhoundX = htc->positionX - ptc->positionX;
	float playerToHellhoundZ = htc->positionZ - ptc->positionZ;
	float behindDot = playerToHellhoundX * ptc->facingX + playerToHellhoundZ * ptc->facingZ;
	float magHellhound = sqrt(playerToHellhoundX * playerToHellhoundX + playerToHellhoundZ * playerToHellhoundZ);
	float magPlayer = sqrt(ptc->facingX * ptc->facingX + ptc->facingZ * ptc->facingZ);

	float tolerance = 0.7f; // THIS IS FOR ANGLE SMOOTHING
	hc->hasMadeADecision = true;
	if (std::abs((behindDot / (magHellhound * magPlayer) + 1)) < tolerance) // are we behind player back? (trust the magic math, please)
	{
		//we are behind, more likely to charge
		int randomNumber = rand() % 10 + 1; // between 1-10
		if (randomNumber < 3 && (!hc->isShooting && hc->shootingCooldownCounter >= hc->shootingCooldown))
		{
			hc->wantsToShoot = true;
		}
		else
		{
			hc->wantsToShoot = false;
		}
	}
	else
	{
		//we are not behind, more likely to flame
		int randomNumber = rand() % 10 + 1; // between 1-10
		if (randomNumber >= 3 && (!hc->isShooting && hc->shootingCooldownCounter >= hc->shootingCooldown))
		{
			hc->wantsToShoot = true;
		}
		else
		{
			hc->wantsToShoot = false;
		}
	}
}


bool HellhoundBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	HellhoundBehaviour* hellhoundComponent = nullptr;
	TransformComponent* hellhoundTransformComponent = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enmComp = nullptr;
	AnimationComponent* enemyAnim = nullptr;

	bool updateGridOnce = true;

	PathfindingMap* valueGrid = (PathfindingMap*)malloc(sizeof(PathfindingMap));//(PathfindingMap*)MemLib::spush(sizeof(PathfindingMap));
	//*valueGrid = PathfindingMap();

	//for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
	//{
	//	playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
	//	playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
	//	playerStats = registry.GetComponent< StatComponent>(playerEntity);
	//}


	// FOR TESTING
	/*int i = 0; 
	TransformComponent* stc = nullptr;
	TransformComponent* stcTwo = nullptr;
	for (auto enemyEntity : View<SkeletonBehaviour, TransformComponent, StatComponent>(registry))
	{
		if (i == 0)
		{
			stc = registry.GetComponent<TransformComponent>(enemyEntity);
			i++;
		}
			
		if(i == 1)
			stcTwo = registry.GetComponent<TransformComponent>(enemyEntity);
	}*/


	for (auto enemyEntity : View<HellhoundBehaviour, TransformComponent, StatComponent, EnemyComponent, AnimationComponent>(registry))
	{
		hellhoundComponent = registry.GetComponent<HellhoundBehaviour>(enemyEntity);
		hellhoundTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent< StatComponent>(enemyEntity);
		enmComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);
		
		//Find a player to kill.
		if (enmComp->lastPlayer.index == -1)
		{
			for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
			{
				if (enemyEntity.index == playerEntity.index)
				{
					continue;
				}
				playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
				playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
				playerStats = registry.GetComponent< StatComponent>(playerEntity);
				enmComp->lastPlayer = playerEntity;
				if (rand() % 2)
				{
					break;
				}
			}
		}
		else
		{
			playerComponent = registry.GetComponent<PlayerComponent>(enmComp->lastPlayer);
			playerTransformCompenent = registry.GetComponent<TransformComponent>(enmComp->lastPlayer);
			playerStats = registry.GetComponent< StatComponent>(enmComp->lastPlayer);
			if (playerComponent == nullptr)
			{
				for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
				{
					enmComp->lastPlayer.index = -1;
				}
			}
		}

		if (hellhoundComponent != nullptr && playerTransformCompenent != nullptr && enmComp != nullptr && enemyAnim != nullptr && enemyStats->GetHealth() > 0)// check if enemy is alive, change later
		{
			ML_Vector<Node> finalPath;
			hellhoundComponent->updatePathCounter += GetDeltaTime();


			float distance = Calculate2dDistance(hellhoundTransformComponent->positionX, hellhoundTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			hellhoundComponent->attackStunDurationCounter += GetDeltaTime();
			hellhoundComponent->shootingCooldownCounter += GetDeltaTime();


			if (hellhoundComponent->retreat)
			{
				if (hellhoundComponent->updatePathCounter >= hellhoundComponent->updatePathLimit * 3.f)
				{
					hellhoundComponent->updatePathCounter = 0;
					if (playerComponent != nullptr && updateGridOnce)
					{
						updateGridOnce = false;
						CalculateGlobalMapValuesHellhound(valueGrid);
						if (valueGrid->cost[0][0] == -69.f)
						{
							updateGridOnce = true;
							continue;
						}
					}

					
					TransformComponent tran = FindRetreatTile(valueGrid, hellhoundTransformComponent, 20.f, 40.f);
					finalPath = CalculateAStarPath(valueGrid, hellhoundTransformComponent, &tran);
					

					// goal (next node) - current
					if (finalPath.size() > 2 && hellhoundComponent->followPath)
					{
						hellhoundComponent->fx = finalPath[0].fx;
						hellhoundComponent->fz = finalPath[0].fz;
						hellhoundComponent->dirX = (float)finalPath[1].x - (float)finalPath[0].x;
						hellhoundComponent->dirZ = (float)-(finalPath[1].z - (float)finalPath[0].z);
						hellhoundComponent->dir2X = (float)finalPath[2].x - (float)finalPath[1].x;
						hellhoundComponent->dir2Z = (float)-(finalPath[2].z - (float)finalPath[1].z);
						hellhoundComponent->followPath = true;
					}
					else
					{
						hellhoundComponent->followPath = false;
					}
				}

				if (hellhoundComponent->followPath == true && hellhoundComponent->updatePathCounter >= hellhoundComponent->updatePathLimit / 2.f)
				{
					hellhoundComponent->dirX = hellhoundComponent->dir2X;
					hellhoundComponent->dirZ = hellhoundComponent->dir2Z;
				}

				SetHitboxCanDealDamage(enemyEntity, enmComp->attackHitBoxID, false);
				SetHitboxActive(enemyEntity, enmComp->attackHitBoxID, false);
				TacticalRetreatBehaviour(enemyEntity, hellhoundTransformComponent, hellhoundComponent, enemyStats, enemyAnim);
			}
			else if (hellhoundComponent->isShooting) //currently charging his ranged attack, getting ready to shoot
			{

				hellhoundComponent->shootingCounter += GetDeltaTime();
				if (hellhoundComponent->shootingCounter >= hellhoundComponent->shootingChargeUpTime) // have we charged long enough?
				{
					//it seems we have. Time to start shooting behaviour
					if (!hellhoundComponent->flameSoundsStartedPlaying)
					{
						SoundComponent* sfx = registry.GetComponent<SoundComponent>(enemyEntity);
						sfx->Play(Hellhound_Flame, Channel_Base);
						hellhoundComponent->flameSoundsStartedPlaying = true;

					}
					
					ShootingBehaviour(playerTransformCompenent, hellhoundComponent, enemyStats, playerStats, enemyEntity, enmComp/*, stc, stcTwo*/); //this is damage thing
				}
				//else we do nothing, we're just charging the flames.
			}
			else if (hellhoundComponent->attackStunDurationCounter <= hellhoundComponent->attackStunDuration)
			{
				enemyStats->SetKnockbackMultiplier(1.0f);
				enemyAnim->aAnim = ANIMATION_ATTACK;
				enemyAnim->aAnimIdx = 2;
				enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
				ANIM_BRANCHLESS(enemyAnim);

				// do nothing, stand like a bad doggo and be ashamed. You hit the player, bad doggo...
				hellhoundComponent->isWating = true;
			}
			else if (hellhoundComponent->isWating)// maybe retreat to shoot again, eh?
			{
				hellhoundComponent->isWating = false;
				SetInfiniteDirection(hellhoundTransformComponent, hellhoundComponent);
				hellhoundComponent->retreat = true;
				hellhoundComponent->hasMadeADecision = false;
			}
			else if (distance < hellhoundComponent->meleeDistance || hellhoundComponent->attackTimer > 0.0f) // fight club and not currently shooting
			{
				hellhoundComponent->charge = false;
				hellhoundComponent->chargePrepareDurationCounter = 0.0f;
				CombatBehaviour(hellhoundComponent, enemyStats, playerStats, playerTransformCompenent, hellhoundTransformComponent, enemyEntity, enmComp, enemyAnim);
			}
			//else if (distance <= 15 + hellhoundComponent->circleBehaviour) // time to  circle?
			//{
			//	if (hellhoundComponent->isBehind && hellhoundComponent->isBehindCounter >= 0.15f) // attack the back
			//	{
			//		hellhoundComponent->charge = true;
			//		ChaseBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats, enemyAnim, hellhoundComponent->dirX, hellhoundComponent->dirZ, hellhoundComponent->followPath);
			//	}
			//	else //keep circling
			//	{
			//		CircleBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats);
			//	}
			//}
			else if (hellhoundComponent->hasMadeADecision && !hellhoundComponent->wantsToShoot) // CHARGE
			{
				hellhoundComponent->charge = true;
				hellhoundComponent->chargePrepareDurationCounter += GetDeltaTime();
				if(hellhoundComponent->chargePrepareDurationCounter >= hellhoundComponent->chargePrepareDuration) // to make it feel fair
					ChaseBehaviour(enemyEntity,playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats, enemyAnim, hellhoundComponent->dirX, hellhoundComponent->dirZ, hellhoundComponent->followPath);
			}
			else if (hellhoundComponent->hasMadeADecision && hellhoundComponent->wantsToShoot) // shoot
			{
				hellhoundComponent->lastPositionX = hellhoundTransformComponent->positionX;
				hellhoundComponent->lastPositionZ = hellhoundTransformComponent->positionZ;
				FixShootingTargetPosition(playerTransformCompenent, hellhoundTransformComponent, hellhoundComponent, enemyEntity); //set a target for the ranged attack
			
			}
			else if (distance <= 17 && hellhoundComponent->hasMadeADecision == false) // time to make a decision
			{
				MakeBehaviourDecision(hellhoundTransformComponent, playerTransformCompenent, hellhoundComponent);
			}
			//else if ((distance <= 17) && (!hellhoundComponent->isShooting && hellhoundComponent->shootingCooldownCounter >= hellhoundComponent->shootingCooldown)) //is not shooting and cooldown is ready
			//{
			//	hellhoundComponent->lastPositionX = hellhoundTransformComponent->positionX;
			//	hellhoundComponent->lastPositionZ = hellhoundTransformComponent->positionZ;
			//	FixShootingTargetPosition(playerTransformCompenent, hellhoundTransformComponent, hellhoundComponent, enemyEntity); //set a target for the ranged attack
			//}
			else if (distance < 50) //hunting distance, go chase
			{
				if (hellhoundComponent->updatePathCounter >= hellhoundComponent->updatePathLimit)
				{
					hellhoundComponent->updatePathCounter = 0;
					if (playerComponent != nullptr && updateGridOnce)
					{
						updateGridOnce = false;
						CalculateGlobalMapValuesHellhound(valueGrid);
						if (valueGrid->cost[0][0] == -69.f)
						{
							updateGridOnce = true;
							continue; //illegal grid
						}
					}

					finalPath = CalculateAStarPath(valueGrid, hellhoundTransformComponent, playerTransformCompenent);

					if (finalPath.size() > 2)
					{
						hellhoundComponent->fx = finalPath[0].fx;
						hellhoundComponent->fz = finalPath[0].fz;
						hellhoundComponent->followPath = true;
					}

					// goal (next node) - current
					if (finalPath.size() > 2 && hellhoundComponent->followPath)
					{
						hellhoundComponent->dirX = (float)finalPath[1].x - (float)finalPath[0].x;
						hellhoundComponent->dirZ = -(float)(finalPath[1].z - (float)finalPath[0].z);
						hellhoundComponent->dir2X = (float)finalPath[2].x - (float)finalPath[1].x;
						hellhoundComponent->dir2Z = -(float)(finalPath[2].z - (float)finalPath[1].z);
						hellhoundComponent->followPath = true;
					}
					else
					{
						hellhoundComponent->followPath = false;
					}
				}

				if (hellhoundComponent->followPath == true && hellhoundComponent->updatePathCounter >= hellhoundComponent->updatePathLimit / 2.f)
				{
					hellhoundComponent->dirX = hellhoundComponent->dir2X;
					hellhoundComponent->dirZ = hellhoundComponent->dir2Z;
				}

				if (distance < hellhoundComponent->meleeDistance * 2.f) // dont follow path, go melee
				{
					hellhoundComponent->followPath = false;
				}

				
				ChaseBehaviour(playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats, enemyAnim, hellhoundComponent->dirX, hellhoundComponent->dirZ, hellhoundComponent->followPath);
			}
			else // idle
			{
				hellhoundComponent->charge = false;
				enmComp->lastPlayer.index = -1;//Search for a new player to hit.
				IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats, enemyAnim);
			}
		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			enmComp->lastPlayer.index = -1;//Search for a new player to hit.
			IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, hellhoundComponent, hellhoundTransformComponent, enemyStats, enemyAnim);
		}
		TransformDecelerate(enemyEntity);//Always decelerate

	}

	// pop the value map
	//MemLib::spop();
	free(valueGrid);
	return true;
}