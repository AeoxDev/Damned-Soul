#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "EventFunctions.h"
#include "States\StateManager.h"

bool ControllerSystem::Update()
{
	//Controller for player during play
	
	for (auto entity : View<ControllerComponent, TransformComponent, StatComponent, AnimationComponent, MouseComponent>(registry))
	{
		if (gameSpeed < 0.00001f)
		{
			break;
		}
		//Get the relevant components from the entity
		ControllerComponent* controller = registry.GetComponent<ControllerComponent>(entity);
		StatComponent* stat = registry.GetComponent<StatComponent>(entity);
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		MouseComponent* mouseComponent = registry.GetComponent<MouseComponent>(entity);

		if (controller->enabled == -1)
			break;

		//Store variables for checking to see how far the entity has moved, these are relevant to the camera
		//transform->lastPositionZ = transform->positionZ;
		//transform->lastPositionX = transform->positionX;
		controller->goalX = 0.0f;
		controller->goalZ = 0.0f;
		//End of: Camera System thing

		//Default the animation to idle, subject to be changed based off of user input
		AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
		/*anim->aAnimTime += GetDeltaTime() * anim->aAnimTimeFactor;
		ANIM_BRANCHLESS(anim);*/


		/*MOVEMENT INPUT*/
		bool moving = false;
		if (keyInput[SCANCODE_W] == down)
		{
			moving = true;
			
			//transform->positionZ += stat->moveSpeed * GetDeltaTime();
			controller->goalZ += 1.0f;
		}
		if (keyInput[SCANCODE_S] == down)
		{
			moving = true;
			//transform->positionZ -= stat->moveSpeed * GetDeltaTime();
			controller->goalZ -= 1.0f;
		}
		if (keyInput[SCANCODE_A] == down)
		{
			moving = true;
			//transform->positionX -= stat->moveSpeed * GetDeltaTime();
			controller->goalX -= 1.0f;
		}
		if (keyInput[SCANCODE_D] == down)
		{
			moving = true;
			//transform->positionX += stat->moveSpeed * GetDeltaTime();
			controller->goalX += 1.0f;
		}
		MouseComponentUpdateDirection(entity);
		if (moving)
		{
			anim->aAnim = ANIMATION_WALK;
			anim->aAnimIdx = 0;

			float len = controller->goalX * controller->goalX + controller->goalZ * controller->goalZ;
			if (len <= 0.0f)
			{
				len = 1.0f;
			}
			else
			{
				len = sqrtf(controller->goalX * controller->goalX + controller->goalZ * controller->goalZ);
				
			}
			controller->goalZ /= len;
			controller->goalX /= len;
			transform->positionZ += controller->goalZ * stat->GetSpeed() * GetDeltaTime();
			transform->positionX += controller->goalX * stat->GetSpeed() * GetDeltaTime();
			/*SmoothRotation(transform, controller->goalX, controller->goalZ, 8.0f);*/
			SmoothRotation(transform, MouseComponentGetDirectionX(mouseComponent), MouseComponentGetDirectionZ(mouseComponent), 16.0f);
		}

		//clamp moveTime to lower limit if not moving
		else 
		{
			anim->aAnim = ANIMATION_IDLE;
			anim->aAnimIdx = 0;
			
			SmoothRotation(transform, MouseComponentGetDirectionX(mouseComponent), MouseComponentGetDirectionZ(mouseComponent), 16.0f);
			
		}

		/*COMBAT INPUT*/
		//Dash in the direction you're moving, defaults to dashing backwards if you're not moving. Gives i-frames for the duration
		if (keyState[SCANCODE_SPACE] == pressed)
		{
			if (moving)
			{
				//Set facing direction to dash direction when moving
				transform->facingX = controller->goalX;
				transform->facingZ = controller->goalZ;
				registry.AddComponent<DashArgumentComponent>(entity, transform->facingX, transform->facingZ, 2.5f);
				AddTimedEventComponentStart(entity, 0.0f, PlayerDashSound, CONDITION_DASH);
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerLoseControl, PlayerDash, 0.2f, PlayerRegainControl, CONDITION_DASH);
				AddSquashStretch(entity, Linear, 0.8f, 0.8f, 1.5f, true, 1.2f, 1.2f, 0.8f);
				int squashStretch = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, ResetSquashStretch, SquashStretch, 0.2f, ResetSquashStretch, 0, 1);
			}
			else
			{
				//Default dash goes backwards
				AddTimedEventComponentStart(entity, 0.0f, PlayerDashSound, CONDITION_DASH);
				transform->facingX = -MouseComponentGetDirectionX(mouseComponent);
				transform->facingZ = -MouseComponentGetDirectionZ(mouseComponent);
				break;

			}
		}

		//Switches animation to attack and deals damage in front of yourself halfway through the animation (offset attack hitbox)
		if (mouseButtonPressed[0] == pressed)
		{
			
			StatComponent* playerStats = registry.GetComponent<StatComponent>(entity);
			AddTimedEventComponentStartEnd(entity, 0.0f, ResetAnimation, 1.0f, nullptr, 1);
			AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerAttackSound, PlayerAttack, 1.0f / playerStats->GetAttackSpeed(), PlayerResetAnimFactor); //Esketit xd
		}
#ifdef _DEBUG
		if (keyState[SCANCODE_G] == pressed) {
			StatComponent* pStats = registry.GetComponent<StatComponent>(stateManager.player);
			PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
			HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(stateManager.player);
			hitbox->circleHitbox[2].radius = 100.0f;
			if (pStats->hazardModifier > -100.0f)
			{
				transform->mass += 100.0f;
				player->killingSpree = 10000;
				player->UpdateSouls(1000000);
				hitbox->circleHitbox[2].radius += 100.0f;
			}
			else
			{
				transform->mass -= 100.0f;
				hitbox->circleHitbox[2].radius -= 100.0f;
			}
		}
#endif // _DEBUG

		//Update animation at the end of user input
		anim->aAnimTime += GetDeltaTime() * anim->aAnimTimeFactor;
		ANIM_BRANCHLESS(anim);

	}
	//Loop for player during other places
	return true;
}