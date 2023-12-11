#include "EventFunctions.h"
#include "TimedEventComponent.h"
#include "Components.h"
#include "Registry.h"
#include "Camera.h"
#include <assert.h>
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "CollisionFunctions.h"
#include "Level.h"
#include "Input.h"
#include "SDLHandler.h"
#include "UIButtonFunctions.h"
#include "Particles.h" //DOG

void CutsceneCreateLinearTransition(EntityID& entity, const int& index)
{
	float transitionTime = GetTimedEventTotalTime(entity, index);
	CutsceneComponent* cutscene = registry.AddComponent<CutsceneComponent>(entity);
	DirectX::XMFLOAT3 lookAt = Camera::GetLookAtFloat();
	cutscene->startLookAtX = lookAt.x;
	cutscene->startLookAtY = lookAt.y;
	cutscene->startLookAtZ = lookAt.z;

	DirectX::XMFLOAT3 position = Camera::GetPositionFloat();
	cutscene->startPositionX = position.x;
	cutscene->startPositionY = position.y;
	cutscene->startPositionZ = position.z;
	 
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	assert(transform != nullptr);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY;
	cutscene->goalLookAtZ = transform->positionZ;

	cutscene->goalPositionX = transform->positionX + CAMERA_OFFSET_X * 0.1f;
	cutscene->goalPositionY = transform->positionY + CAMERA_OFFSET_Y * 0.1f;
	cutscene->goalPositionZ = transform->positionZ + CAMERA_OFFSET_Z * 0.1f;
	


	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, transitionTime, EndCutscene);
}
void BeginCutscene(EntityID& entity, const int& index)
{
	Camera::SetCutsceneMode(1);
}
void BeginPortalCutscene(EntityID& entity, const int& index)
{
	Camera::SetCutsceneMode(2);
}
void BeginShopCutscene(EntityID& entity, const int& index)
{
	Camera::SetCutsceneMode(3);
}
void EndCutscene(EntityID& entity, const int& index)
{
	Camera::SetCutsceneMode(0);
}
//Do the given cutscene components arguments over time
void CutsceneTransition(EntityID& entity, const int& index)
{
	CutsceneComponent* cutscene = registry.GetComponent<CutsceneComponent>(entity);
	assert(cutscene != nullptr);
	float time = GetTimedEventElapsedTime(entity, index);
	float totalTime = GetTimedEventTotalTime(entity, index);
	float scalar = time / totalTime;//From 0 to 1

	
	if (cutscene->mode & CutsceneMode::Cutscene_Camera)
	{
		if (cutscene->mode & Cutscene_Accelerating)
		{
			scalar *= scalar;
		}
		if (cutscene->mode & Cutscene_Decelerating)
		{
			scalar = sqrtf(scalar);
		}
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
			float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
			float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
			float newPosX = posDifX * scalar + cutscene->startPositionX;
			float newPosY = posDifY * scalar + cutscene->startPositionY;
			float newPosZ = posDifZ * scalar + cutscene->startPositionZ;
			Camera::SetPosition(newPosX, newPosY, newPosZ, false);
		}
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			float lookAtDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
			float lookAtDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
			float lookAtDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
			float newLookAtX = lookAtDifX * scalar + cutscene->startLookAtX;
			float newLookAtY = lookAtDifY * scalar + cutscene->startLookAtY;
			float newLookAtZ = lookAtDifZ * scalar + cutscene->startLookAtZ;
			Camera::SetLookAt(newLookAtX, newLookAtY, newLookAtZ);
		}
	}

	if (cutscene->mode & Cutscene_Character_Walk)
	{
		float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
		float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
		float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
		float newPosX = posDifX * scalar + cutscene->startPositionX;
		float newPosY = posDifY * scalar + cutscene->startPositionY;
		float newPosZ = posDifZ * scalar + cutscene->startPositionZ;
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		//Set the facing towards the goal
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			transform->facingX = posDifX;
			transform->facingY = posDifY;
			transform->facingZ = posDifZ;
			NormalizeFacing(transform);
		}
		
		//Move the character
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			transform->positionX = newPosX;
			transform->positionZ = newPosZ;
			transform->positionY = newPosY;
		}
		
		//Loop the walk animation
		AnimationComponent* animation = registry.GetComponent<AnimationComponent>(entity);
		BlendAnimationComponent* bAnimation = registry.GetComponent<BlendAnimationComponent>(entity);
		if (animation != nullptr)
		{
			animation->aAnim = ANIMATION_WALK;
			animation->aAnimIdx = 0;
			animation->aAnimTime = GetDeltaTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS(animation);
		}
		else if (bAnimation != nullptr)
		{
			bAnimation->anim1.aAnim = ANIMATION_WALK;
			bAnimation->anim1.aAnimIdx = 0;
			bAnimation->anim1.aAnimTime = GetDeltaTime() + GetTimedEventElapsedTime(entity, index);

			bAnimation->anim2.aAnim = ANIMATION_WALK;
			bAnimation->anim2.aAnimIdx = 0;
			bAnimation->anim2.aAnimTime = GetDeltaTime() + GetTimedEventElapsedTime(entity, index);

			ANIM_BRANCHLESS((&(bAnimation->anim1)));
			ANIM_BRANCHLESS((&(bAnimation->anim2)));
		}
	}
	if (cutscene->mode & Cutscene_Character_Fall)
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
		float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
		float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
		float newPosX = posDifX * scalar + cutscene->startPositionX;
		float newPosZ = posDifZ * scalar + cutscene->startPositionZ;
		if (cutscene->mode & Cutscene_Accelerating)
			scalar *= scalar;
		if (cutscene->mode & Cutscene_Decelerating)
		{
			scalar = sqrtf(scalar);
		}
		float newPosY = posDifY * scalar + cutscene->startPositionY;
	
		float facingDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
		float facingDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
		float facingDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
		float newFacingX = facingDifX * scalar + cutscene->startLookAtX;
		float newFacingY = facingDifY * scalar + cutscene->startLookAtY;
		float newFacingZ = facingDifZ * scalar + cutscene->startLookAtZ;

		//Set the facing towards the goal
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			transform->facingX = newFacingX;
			transform->facingY = newFacingY;
			transform->facingZ = newFacingZ;
			NormalizeFacing(transform);
		}
		else
		{
			//Smooth rotation towards mouse
			float mousePosX = -0.13f + ((float)mouseX / (float)sdl.WIDTH);//Down to 0.0f to 1.0f
			mousePosX = (mousePosX - 0.5f) * 2.0f;//From -1.0f to 1.0f;
			
			SmoothRotationIgnoreTime(transform, mousePosX, -0.33f, 1.0f);
		}

		//Move the character
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			transform->positionX = newPosX;
			transform->positionY = newPosY;
			transform->positionZ = newPosZ;
		}

		//Loop the walk animation
		//AnimationComponent* animation = registry.GetComponent<AnimationComponent>(entity);
		//animation->aAnim = ANIMATION_IDLE;
		//animation->aAnimIdx = 1;
		//animation->aAnimTime = 0.01f + GetDeltaTime() + GetTimedEventElapsedTime(entity, index);
		//ANIM_BRANCHLESS(animation);

		BlendAnimationComponent* animation = registry.GetComponent<BlendAnimationComponent>(entity);
		animation->anim1.aAnim = ANIMATION_IDLE;
		animation->anim1.aAnimIdx = 1;
		animation->anim1.aAnimTime = 0.01f + GetDeltaTime() + GetTimedEventElapsedTime(entity, index);
		ANIM_BRANCHLESS((&(animation->anim1)));

		animation->anim2.aAnim = ANIMATION_IDLE;
		animation->anim2.aAnimIdx = 1;
		animation->anim2.aAnimTime = 0.01f + GetDeltaTime() + GetTimedEventElapsedTime(entity, index);
		ANIM_BRANCHLESS((& (animation->anim2)));

	}
	if (cutscene->mode & Cutscene_Character_Idle)
	{
		if (cutscene->mode & Cutscene_Accelerating)
			scalar *= scalar;
		if (cutscene->mode & Cutscene_Decelerating)
		{
			scalar = sqrtf(scalar);
		}
		float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
		float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
		float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
		float newPosX = posDifX * scalar + cutscene->startPositionX;
		float newPosY = posDifY * scalar + cutscene->startPositionY;
		float newPosZ = posDifZ * scalar + cutscene->startPositionZ;

		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		float facingDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
		float facingDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
		float facingDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
		float newFacingX = facingDifX * scalar + cutscene->startLookAtX;
		float newFacingY = facingDifY * scalar + cutscene->startLookAtY;
		float newFacingZ = facingDifZ * scalar + cutscene->startLookAtZ;

		//Set the facing towards the goal
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			transform->facingX = newFacingX;
			transform->facingY = newFacingY;
			transform->facingZ = newFacingZ;
			NormalizeFacing(transform);
		}
		//Move the character
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			transform->positionX = newPosX;
			transform->positionY = newPosY;
			transform->positionZ = newPosZ;
		}

		//Loop the idle animation
		AnimationComponent* animation = registry.GetComponent<AnimationComponent>(entity);
		BlendAnimationComponent* bAnimation = registry.GetComponent<BlendAnimationComponent>(entity);
		if (animation != nullptr)
		{
			animation->aAnim = ANIMATION_IDLE;
			animation->aAnimIdx = 0;
			animation->aAnimTime = 0.01f + GetFrameTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS(animation);
		}
		else if (bAnimation != nullptr)
		{
			bAnimation->anim1.aAnim = ANIMATION_IDLE;
			bAnimation->anim1.aAnimIdx = 0;
			bAnimation->anim1.aAnimTime = 0.01f + GetFrameTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS((&(bAnimation->anim1)));

			bAnimation->anim2.aAnim = ANIMATION_IDLE;
			bAnimation->anim2.aAnimIdx = 0;
			bAnimation->anim2.aAnimTime = 0.01f + GetDeltaTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS((&(bAnimation->anim2)));
		}
		
	}
	if (cutscene->mode & Cutscene_Character_Attack)
	{
		if (cutscene->mode & Cutscene_Accelerating)
			scalar *= scalar;
		if (cutscene->mode & Cutscene_Decelerating)
		{
			scalar = sqrtf(scalar);
		}
		float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
		float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
		float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
		float newPosX = posDifX * scalar + cutscene->startPositionX;
		float newPosY = posDifY * scalar + cutscene->startPositionY;
		float newPosZ = posDifZ * scalar + cutscene->startPositionZ;

		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		float facingDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
		float facingDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
		float facingDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
		float newFacingX = facingDifX * scalar + cutscene->startLookAtX;
		float newFacingY = facingDifY * scalar + cutscene->startLookAtY;
		float newFacingZ = facingDifZ * scalar + cutscene->startLookAtZ;

		//Set the facing towards the goal
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			transform->facingX = newFacingX;
			transform->facingY = newFacingY;
			transform->facingZ = newFacingZ;
			NormalizeFacing(transform);
		}
		//Move the character
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			transform->positionX = newPosX;
			transform->positionY = newPosY;
			transform->positionZ = newPosZ;
		}

		//Loop the attack animation
		AnimationComponent* animation = registry.GetComponent<AnimationComponent>(entity);
		BlendAnimationComponent* bAnimation = registry.GetComponent<BlendAnimationComponent>(entity);
		if (animation != nullptr)
		{
			animation->aAnim = ANIMATION_ATTACK;
			animation->aAnimIdx = 0;
			animation->aAnimTime = 0.01f + GetFrameTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS(animation);
		}
		else if (bAnimation != nullptr)
		{
			bAnimation->anim1.aAnim = ANIMATION_ATTACK;
			bAnimation->anim1.aAnimIdx = 0;
			bAnimation->anim1.aAnimTime = 0.01f + GetFrameTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS((&(bAnimation->anim1)));

			bAnimation->anim2.aAnim = ANIMATION_ATTACK;
			bAnimation->anim2.aAnimIdx = 0;
			bAnimation->anim2.aAnimTime = 0.01f + GetFrameTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS((&(bAnimation->anim2)));
		}
		
	}
	if (cutscene->mode & Cutscene_Character_BossLanding)
	{
		if (cutscene->mode & Cutscene_Accelerating)
			scalar *= scalar;
		if (cutscene->mode & Cutscene_Decelerating)
		{
			scalar = sqrtf(scalar);
		}
		float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
		float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
		float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
		float newPosX = posDifX * scalar + cutscene->startPositionX;
		float newPosY = posDifY * scalar + cutscene->startPositionY;
		float newPosZ = posDifZ * scalar + cutscene->startPositionZ;

		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		float facingDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
		float facingDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
		float facingDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
		float newFacingX = facingDifX * scalar + cutscene->startLookAtX;
		float newFacingY = facingDifY * scalar + cutscene->startLookAtY;
		float newFacingZ = facingDifZ * scalar + cutscene->startLookAtZ;

		//Set the facing towards the goal
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			transform->facingX = newFacingX;
			transform->facingY = newFacingY;
			transform->facingZ = newFacingZ;
			NormalizeFacing(transform);
		}
		//Move the character
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			transform->positionX = newPosX;
			transform->positionY = newPosY;
			if (scalar > 0.5f)
			{
				transform->positionY = 0.0f;
			}
			transform->positionZ = newPosZ;
		}

		//Loop the idle animation
		AnimationComponent* animation = registry.GetComponent<AnimationComponent>(entity);
		if (animation != nullptr)
		{
			animation->aAnim = ANIMATION_WALK;
			animation->aAnimIdx = 2;
			animation->aAnimTime = 0.01f + GetFrameTime() + scalar;
		}

	}

	if (cutscene->mode & Cutscene_Character_DogBrace)
	{
		if (cutscene->mode & Cutscene_Accelerating)
			scalar *= scalar;
		if (cutscene->mode & Cutscene_Decelerating)
		{
			scalar = sqrtf(scalar);
		}
		float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
		float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
		float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
		float newPosX = posDifX * scalar + cutscene->startPositionX;
		float newPosY = posDifY * scalar + cutscene->startPositionY;
		float newPosZ = posDifZ * scalar + cutscene->startPositionZ;

		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		float facingDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
		float facingDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
		float facingDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
		float newFacingX = facingDifX * scalar + cutscene->startLookAtX;
		float newFacingY = facingDifY * scalar + cutscene->startLookAtY;
		float newFacingZ = facingDifZ * scalar + cutscene->startLookAtZ;

		//Set the facing towards the goal
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			transform->facingX = newFacingX;
			transform->facingY = newFacingY;
			transform->facingZ = newFacingZ;
			NormalizeFacing(transform);
		}
		//Move the character
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			transform->positionX = newPosX;
			transform->positionY = newPosY;
			transform->positionZ = newPosZ;
		}

		//Loop the brace animation
		AnimationComponent* animation = registry.GetComponent<AnimationComponent>(entity);
		if (animation != nullptr)
		{
			animation->aAnim = ANIMATION_ATTACK;
			animation->aAnimIdx = 2;
			animation->aAnimTime = 0.01f + GetFrameTime() + GetTimedEventElapsedTime(entity, index);
			//ANIM_BRANCHLESS(animation);
		}
	}

	if (cutscene->mode & Cutscene_Character_DogBreath)
	{
		if (cutscene->mode & Cutscene_Accelerating)
			scalar *= scalar;
		if (cutscene->mode & Cutscene_Decelerating)
		{
			scalar = sqrtf(scalar);
		}
		float posDifX = cutscene->goalPositionX - cutscene->startPositionX;
		float posDifY = cutscene->goalPositionY - cutscene->startPositionY;
		float posDifZ = cutscene->goalPositionZ - cutscene->startPositionZ;
		float newPosX = posDifX * scalar + cutscene->startPositionX;
		float newPosY = posDifY * scalar + cutscene->startPositionY;
		float newPosZ = posDifZ * scalar + cutscene->startPositionZ;

		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		float facingDifX = cutscene->goalLookAtX - cutscene->startLookAtX;
		float facingDifY = cutscene->goalLookAtY - cutscene->startLookAtY;
		float facingDifZ = cutscene->goalLookAtZ - cutscene->startLookAtZ;
		float newFacingX = facingDifX * scalar + cutscene->startLookAtX;
		float newFacingY = facingDifY * scalar + cutscene->startLookAtY;
		float newFacingZ = facingDifZ * scalar + cutscene->startLookAtZ;

		//Set the facing towards the goal
		if (cutscene->mode & CutsceneMode::Transition_LookAt)
		{
			transform->facingX = newFacingX;
			transform->facingY = newFacingY;
			transform->facingZ = newFacingZ;
			NormalizeFacing(transform);
		}
		//Move the character
		if (cutscene->mode & CutsceneMode::Transition_Position)
		{
			transform->positionX = newPosX;
			transform->positionY = newPosY;
			transform->positionZ = newPosZ;
		}

		//Loop the attack animation
		AnimationComponent* animation = registry.GetComponent<AnimationComponent>(entity);
		if (animation != nullptr)
		{
			animation->aAnim = ANIMATION_ATTACK;
			animation->aAnimIdx = 3;
			animation->aAnimTime = 0.01f + GetFrameTime() + GetTimedEventElapsedTime(entity, index);
			ANIM_BRANCHLESS(animation);

			//Update particle movement
			ParticleComponent* particles = registry.GetComponent<ParticleComponent>(entity);
			if (particles)
			{
				HellhoundBehaviour* hc = registry.GetComponent<HellhoundBehaviour>(entity);
				if (hc)
				{
					hc->currentShootingAttackRange += GetFrameTime() * hc->shootingAttackSpeedForHitbox * (float)(hc->currentShootingAttackRange < hc->offsetForward);
					float  cornersX[3] = { 0.0f, hc->currentShootingAttackRange * (hc->offsetSide / hc->offsetForward), -hc->currentShootingAttackRange * (hc->offsetSide / hc->offsetForward) };//Counter clockwise
					float  cornersZ[3] = { -1.0f, -hc->currentShootingAttackRange, -hc->currentShootingAttackRange };//Counter clockwise
					Particles::UpdateMetadata(particles->metadataSlot, cornersX[0], cornersZ[0], cornersX[1], cornersZ[1], cornersX[2], cornersZ[2]);
				}
				
			}
		}

	}
	
}

void SpawnCutsceneParticles(EntityID& entity, const int& index)
{
	HellhoundBehaviour* hc = registry.GetComponent<HellhoundBehaviour>(entity);
	if (hc)
	{
		float  cornersX[3] = { 0.0f, hc->currentShootingAttackRange * (hc->offsetSide / hc->offsetForward), -hc->currentShootingAttackRange * (hc->offsetSide / hc->offsetForward) };//Counter clockwise
		float  cornersZ[3] = { -1.0f, -hc->currentShootingAttackRange, -hc->currentShootingAttackRange };//Counter clockwise
		registry.AddComponent<ParticleComponent>(entity, 1.0f, cornersX[0], 0.5f,
			0.0f, 1.7f, 7.5f, 0.0f, //Z offset fix
			cornersZ[0], cornersX[1], cornersZ[1], cornersX[2], cornersZ[2], 
			2.f, 0.10f, .0f,//rgb
			2048, FLAMETHROWER); // changed 
	}
}

void DestroyCutsceneParticles(EntityID& entity, const int& index)
{
	ParticleComponent* particles = registry.GetComponent<ParticleComponent>(entity);
	if (particles)
	{
		particles->Release();
		registry.RemoveComponent<ParticleComponent>(entity);
	}
}

void SetGameSpeedDefault(EntityID& entity, const int& index)
{
	gameSpeed = 1.0f;
	TimedEventIgnoreGamespeed(false);
}

void EventLoadNextLevel(EntityID& entity, const int& index)
{
	LoadLevel(++stateManager.activeLevel);
}

void EventShopLoadNextLevel(EntityID& entity, const int& index)
{
	UIFunctions::Game::LoadNextLevel(nullptr, 0);
}
