#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "EventFunctions.h"

bool ControllerSystem::Update()
{
	for (auto entity : View<ControllerComponent, TransformComponent, StatComponent, AnimationComponent, MouseComponent>(registry))
	{
		//Get the relevant components from the entity
		ControllerComponent* controller = registry.GetComponent<ControllerComponent>(entity);
		StatComponent* stat = registry.GetComponent<StatComponent>(entity);
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		MouseComponent* mouseComponent = registry.GetComponent<MouseComponent>(entity);

		//Store variables for checking to see how far the entity has moved, these are relevant to the camera
		//transform->lastPositionZ = transform->positionZ;
		//transform->lastPositionX = transform->positionX;
		controller->goalX = 0.0f;
		controller->goalZ = 0.0f;
		//End of: Camera System thing

		//Default the animation to idle, subject to be changed based off of user input
		AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);
		anim->aAnim = ANIMATION_IDLE;
		anim->aAnimIdx = 0;

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
			transform->positionZ += controller->goalZ * stat->moveSpeed * GetDeltaTime();
			transform->positionX += controller->goalX * stat->moveSpeed * GetDeltaTime();
			/*SmoothRotation(transform, controller->goalX, controller->goalZ, 8.0f);*/
			SmoothRotation(transform, MouseComponentGetDirectionX(mouseComponent), MouseComponentGetDirectionZ(mouseComponent), 8.0f);
		}

		//clamp moveTime to lower limit if not moving
		else 
		{
			SmoothRotation(transform, MouseComponentGetDirectionX(mouseComponent), MouseComponentGetDirectionZ(mouseComponent), 8.0f);
			
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
			}
			else
			{
				//Default dash goes backwards
				transform->facingX = -MouseComponentGetDirectionX(mouseComponent);
				transform->facingZ = -MouseComponentGetDirectionZ(mouseComponent);

			}
			registry.AddComponent<DashArgumentComponent>(entity, transform->facingX, transform->facingZ, 2.5f);
			AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerLoseControl, PlayerDash, 0.2f, PlayerRegainControl, CONDITION_DASH);
		}

		//Switches animation to attack and deals damage in front of yourself halfway through the animation (offset attack hitbox)
		if (mouseButtonPressed[0] == pressed)
		{
			AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, PlayerAttack, 1.0f, SetPlayerAttackHitboxInactive);
		}
	}
	return true;
}