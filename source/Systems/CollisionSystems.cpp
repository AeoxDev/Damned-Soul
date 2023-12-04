#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "EventFunctions.h"
#include "CombatFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h"
#include "States\StateManager.h"
#include "CollisionFunctions.h"
#include "Camera.h"

float giSpawnPosX = 0.0f;
float giSpawnPosZ = 0.0f;

struct GIFloat2
{
	float x, z;
	GIFloat2 operator+(GIFloat2 add);
};
GIFloat2 GIFloat2::operator+(GIFloat2 add)
{
	
	
	GIFloat2 sum;
	sum.x = this->x + add.x;
	sum.z = this->z + add.z;
	return sum;
}
GIFloat2 GetGIadjacentDirections(int pixelPosX, int pixelPosZ, int type);
void WallResolve(EntityID entity, ProjectileComponent* proj, GeometryIndependentComponent* geoCo, TransformComponent* p, StatComponent* stat)
{
	proj = registry.GetComponent<ProjectileComponent>(entity);
	if (proj != nullptr)
	{
		if (proj->type == eye)
		{
			CreateAcidHazard(entity, 0);
		}
		registry.DestroyEntity(entity);
	}
	{
		GridPosition pixelPos = PositionOnGrid(geoCo, p);
		TransformComponent lastPos;
		lastPos.positionX = p->lastPositionX;
		lastPos.positionZ = p->lastPositionZ;
		GridPosition lastPixelPos = PositionOnGrid(geoCo, &lastPos);
		//Edge direction
		if (pixelPos.x >= GI_TEXTURE_DIMENSIONS || pixelPos.x < 0 || pixelPos.z >= GI_TEXTURE_DIMENSIONS || pixelPos.z < 0 ||
			lastPixelPos.x >= GI_TEXTURE_DIMENSIONS || lastPixelPos.x < 0 || lastPixelPos.z >= GI_TEXTURE_DIMENSIONS || lastPixelPos.z < 0)
		{
			p->positionX = giSpawnPosX;
			p->positionZ = giSpawnPosZ;
			proj = registry.GetComponent<ProjectileComponent>(entity);
			if (proj != nullptr)
			{
				registry.DestroyEntity(entity);
			}
			return;
		}

		GIFloat2 direction = GetGIadjacentDirections(pixelPos.x, pixelPos.z, 0);
		GIFloat2 lastDirection = GetGIadjacentDirections(lastPixelPos.x, lastPixelPos.z, 0);

		GIFloat2 directionSum = direction + lastDirection;//0, 0 and last
		float sumX = directionSum.x;
		float sumZ = directionSum.z;

		float len = sqrtf(sumX * sumX + sumZ * sumZ);
		if (len > 0)
		{
			sumX /= len;
			sumZ /= len;
		}

		float repositionFactor = 1.0f;
		p->positionX += repositionFactor * sumX * stat->GetSpeed() * GetDeltaTime();
		p->positionZ += repositionFactor * sumZ * stat->GetSpeed() * GetDeltaTime();
		if (sumX * sumX + sumZ * sumZ > 0.0f)
		{
			//On edge. Go back a little based on current speed to prevent glitching.
			p->positionX -= repositionFactor * p->currentSpeedX * GetDeltaTime();
			p->positionZ -= repositionFactor * p->currentSpeedZ * GetDeltaTime();
		}
		else
		{
			//Something went wrong and now we are inside the wall, go back to last safe spot.
			p->positionX = p->lastSafeGIPosX;
			p->positionY = p->lastSafeGIPosY;
			p->positionZ = p->lastSafeGIPosZ;

			KnockBackComponent* k = registry.GetComponent<KnockBackComponent>(entity);
			if (k)
			{
				registry.RemoveComponent<KnockBackComponent>(entity);
			}
		}

		//When dashing, bounce
		if (entity.index == stateManager.player.index)
		{
			PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
			if (player == nullptr || player->isDashing == false)
			{
				return;
			}

			float inX = p->currentSpeedX;//From last to pos
			float inZ = p->currentSpeedZ;
			len = sqrtf(inX * inX + inZ * inZ);
			if (len > 0)
			{
				inX /= len;
				inZ /= len;
			}

			//God ol' reflection math.
			float scalar = 2.0f * (inX * sumX + inZ * sumZ);	// 2.0 * dot(Incident, Normal);

			if (sumX == 0.0f && sumZ == 0.0f)
			{
				p->currentSpeedX *= -1.0f;
				p->currentSpeedZ *= -1.0f;
				p->positionX += p->currentSpeedX * GetDeltaTime();

				p->positionZ += p->currentSpeedZ * GetDeltaTime();
			}
			else
			{

				p->positionX = p->lastPositionX;
				p->positionZ = p->lastPositionZ;
				//p->currentSpeedX *= inX - scalar * sumX;
				//p->currentSpeedZ *= inZ - scalar * sumZ;
				p->currentSpeedX = sumX;
				p->currentSpeedZ = sumZ;
				p->positionX += p->currentSpeedX * GetDeltaTime();
				p->positionZ += p->currentSpeedZ * GetDeltaTime();
				//Squish
				AddSquashStretch(entity, Constant, 1.1f, 1.1f, 0.8f, false, 1.0f, 1.0f, 1.0f);
				p->facingX = -sumX;
				p->facingZ = -sumZ;
				NormalizeFacing(p);
				float punishTime = 0.05f;
				AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
				BlendAnimationComponent* blendAnim = registry.GetComponent<BlendAnimationComponent>(entity);

				if (anim != nullptr)
				{
					anim->aAnimIdx = 0;
					anim->aAnim = ANIMATION_DEATH;
					anim->aAnimTime = 0.01f;
				}
				else if (blendAnim != nullptr)
				{
					blendAnim->anim1.aAnimIdx = 0;
					blendAnim->anim1.aAnim = ANIMATION_DEATH;
					blendAnim->anim1.aAnimTime = 0.01f;
				
					blendAnim->anim2.aAnimIdx = 0;
					blendAnim->anim2.aAnim = ANIMATION_DEATH;
					blendAnim->anim2.aAnimTime = 0.01f;
				}
				player->isDashing = false;
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PauseAnimation, TPose, punishTime, ContinueAnimation, 0, 2);
				int squashStretch = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, ResetSquashStretch, SquashStretch, punishTime, ResetSquashStretch, 0, 2);
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, SetSpeedZero, ShakeCamera, punishTime, ResetSpeed, 0, 2);
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerLoseControl, PlayerLoseControl, punishTime, PlayerRegainControl, 0, 2);
			}
		}


	}
}


//To find the direction to get out of cracks.
GIFloat2 GetGIadjacentDirections(int pixelPosX, int pixelPosZ, int type)
{

	
	float posX = 0.0f;
	float posZ = 0.0f;

	//Do a 5 by 5 g rid
	const int gridSize = 5;
	int gridX[gridSize] = { pixelPosX - 2 ,pixelPosX - 1, pixelPosX, (pixelPosX + 1) % GI_TEXTURE_DIMENSIONS, (pixelPosX + 2) % GI_TEXTURE_DIMENSIONS };
	int gridZ[gridSize] = { pixelPosZ - 2 ,pixelPosZ - 1, pixelPosZ, (pixelPosZ + 1) % GI_TEXTURE_DIMENSIONS, (pixelPosZ + 2) % GI_TEXTURE_DIMENSIONS };

	if (gridX[0] < 0)
	{
		gridX[0] = GI_TEXTURE_DIMENSIONS - 2;
	}
	if (gridX[1] < 0)
	{
		gridX[1] = GI_TEXTURE_DIMENSIONS - 1;
	}
	if (gridZ[0] < 0)
	{
		gridZ[0] = GI_TEXTURE_DIMENSIONS - 2;
	}
	if (gridZ[1] < 0)
	{
		gridZ[1] = GI_TEXTURE_DIMENSIONS - 1;
	}
	int value = 0;
	//First check the upper left, then to for rowwise
	for (int z = 0; z < gridSize; z++)
	{
		for (int x = 0; x < gridSize; x++)
		{
			value = giTexture->texture[gridZ[z]][gridX[x]];
			if (value == type)
			{
				posX += (x - (gridSize / 2));
				posZ += (z - (gridSize / 2));
			}
			
		}
	}
	
	GIFloat2 toReturn;
	toReturn.x = -posX;
	toReturn.z = posZ;
	return toReturn;
}
void SetGISpawnPosition(float x, float z)
{
	giSpawnPosX = x;
	giSpawnPosZ = z;
}

void HazardDamageHelper(EntityID& victim, const float DPS)
{

	auto defenderStats = registry.GetComponent<StatComponent>(victim);

	if (0.001f < defenderStats->hazardModifier)
	{
		DamageOverTime dotComp(DPS * defenderStats->hazardModifier, 1.f);

		float finalDamage = Combat::CalculateDamage(dotComp, victim, RelicInput::DMG::DOT_HAZARD) * GetDeltaTime();

		if (0.0001f < finalDamage)
		{
			Combat::HitFlat(victim, defenderStats, finalDamage);
		}
	}
}

bool GeometryIndependentSystem::Update()
{
	//First find the GI component
	GeometryIndependentComponent* geoCo = nullptr;
	for (auto entity : View<GeometryIndependentComponent>(registry))
	{
		geoCo = registry.GetComponent<GeometryIndependentComponent>(entity);
	}
	//Then check the position of all players and enemies:
	PlayerComponent* player = nullptr;
	if (stateManager.player.index != -1)
	{
		player = registry.GetComponent<PlayerComponent>(stateManager.player);

	}
	if (geoCo != nullptr)
	{
		for (auto entity : View<TransformComponent, HitboxComponent, StatComponent>(registry))
		{
			TransformComponent* p = registry.GetComponent<TransformComponent>(entity);
			HitboxComponent* h = registry.GetComponent<HitboxComponent>(entity);
			StatComponent* stat = registry.GetComponent<StatComponent>(entity);
			AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
			BlendAnimationComponent* blendAnim = registry.GetComponent<BlendAnimationComponent>(entity);
			//We have found a player component with a transform
			//Now take position and translate to pixel on texture and check if stage, if not, reset pos for now
			if (HitboxCanHitGI(entity))
			{
				int r = PixelValueOnPosition(geoCo,p);
				int takeDamage = 0;
				ProjectileComponent* proj = nullptr;
				if (player != nullptr && r == HAZARD_GATE && player->portalCreated == false)
				{
					r = HAZARD_WALL;
				}
				switch (r)
				{
				case -1:
					p->positionX = giSpawnPosX;
					p->positionZ = giSpawnPosZ;
					proj = registry.GetComponent<ProjectileComponent>(entity);
					if (proj != nullptr)
					{
						registry.DestroyEntity(entity);
					}
					break;
				case 0:
				/*	p->positionX = giSpawnPosX;
					p->positionZ = giSpawnPosZ;
					proj = registry.GetComponent<ProjectileComponent>(entity);
					if (proj != nullptr)
					{
						registry.DestroyEntity(entity);
					}*/
					//Detect edge
					WallResolve(entity, proj, geoCo, p, stat);

					//p->currentSpeedX *= 0.0f;
					//p->currentSpeedZ *= 0.0f;
					break;
				case 1:
					//Footstep sound here?
					stat->m_acceleration = stat->m_baseAcceleration;
					//Remember last non clipping state.
					p->lastSafeGIPosX = p->positionX;
					p->lastSafeGIPosY = p->positionY;
					p->lastSafeGIPosZ = p->positionZ;
					break;
				case HAZARD_LAVA:
					if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
					{
						anim->aAnimTimeFactor = stat->lavaAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}

					else if (blendAnim != nullptr && blendAnim->anim2.aAnim == ANIMATION_WALK)
					{
						blendAnim->anim2.aAnimTimeFactor = stat->lavaAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}

					stat->m_acceleration = stat->m_baseAcceleration * stat->lavaAccelFactor;

					HazardDamageHelper(entity, 25.f);
					//takeDamage = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, StaticHazardDamage, nullptr, HAZARD_LAVA_UPDATE_TIME, nullptr, r, 1);
					break;
				case HAZARD_CRACK:
					if (stat->canWalkOnCrack == false || proj != nullptr)
					{
						//Detect edge
						GridPosition pixelPos = PositionOnGrid(geoCo, p);
						TransformComponent lastPos;
						lastPos.positionX = p->lastPositionX;
						lastPos.positionZ = p->lastPositionZ;
						GridPosition lastPixelPos = PositionOnGrid(geoCo, &lastPos);
						//Edge direction
						GIFloat2 direction = GetGIadjacentDirections(pixelPos.x, pixelPos.z, HAZARD_CRACK);
						GIFloat2 lastDirection = GetGIadjacentDirections(lastPixelPos.x, lastPixelPos.z, HAZARD_CRACK);
						
						float sumX = direction.x + lastDirection.x;
						float sumZ = direction.z + lastDirection.z;

						float len = sqrtf(sumX*sumX + sumZ*sumZ);
						if (len > 0)
						{
							sumX /= len;
							sumZ /= len;
						}
						

						p->positionX += sumX * stat->GetSpeed() * GetDeltaTime();
						p->positionZ += sumZ * stat->GetSpeed() * GetDeltaTime();
						//p->currentSpeedX *= 0.0f;
						//p->currentSpeedZ *= 0.0f;
					
					}
					break;
				case HAZARD_ACID://Lava but more damage
					if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
					{
						anim->aAnimTimeFactor = stat->acidAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}
					else if (blendAnim != nullptr && blendAnim->anim2.aAnim == ANIMATION_WALK)
					{
						blendAnim->anim2.aAnimTimeFactor = stat->acidAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}
					stat->m_acceleration = stat->m_baseAcceleration * stat->acidAccelFactor;

					HazardDamageHelper(entity, 50.f);
					break;
				case HAZARD_ICE:
					//ICE:
					if (anim != nullptr && anim->aAnim == ANIMATION_WALK)
					{
						anim->aAnimTimeFactor = stat->iceAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}
					else if (blendAnim != nullptr && blendAnim->anim2.aAnim == ANIMATION_WALK)
					{
						blendAnim->anim2.aAnimTimeFactor = stat->iceAnimFactor;
						AddTimedEventComponentStart(entity, 0.01f, ContinueAnimation, 0, 2);
					}
					stat->m_acceleration = stat->m_baseAcceleration * stat->iceAccelFactor;

					//HazardDamageHelper(entity, 25.f);
					//takeDamage = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, StaticHazardDamage, nullptr, HAZARD_LAVA_UPDATE_TIME, nullptr, r, 1);
					break;
				case HAZARD_GATE:
				{
					if (Camera::InCutscene() == 0 && entity.index == stateManager.player.index)
					{
						OnCollisionParameters params = { 0 };
						params.entity2 = stateManager.player;
						LoadNextLevel(params);
					}
					
				}
				
					
					break;
				case HAZARD_NAV:
				{
					//If enemy, do pretend wall:
					EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
					if (enemy)
					{
						WallResolve(entity, proj, geoCo, p, stat);
					}

				}
					break;
				default:
					break;
				}
			}
			
		}
	}
	
	return true;
}

