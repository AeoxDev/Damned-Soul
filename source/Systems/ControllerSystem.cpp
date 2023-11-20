#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "EventFunctions.h"
#include "States\StateManager.h"
#include "Camera.h"

bool ControllerSystem::Update()
{
	//Controller for player during play
	if ((keyState[SCANCODE_SPACE] == pressed || mouseButtonPressed[MouseButton::left] == pressed
		|| mouseButtonPressed[MouseButton::right] == pressed))
	{
		if (!(currentStates & InMainMenu) && Camera::InCutscene() && !(currentStates & InCredits) && !(currentStates & InSettings))
		{
			for (auto entity : View<TimedEventComponent>(registry))
			{
				ReleaseTimedEvents(entity);
			}
			AddTimedEventComponentStart(stateManager.player, 0.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
			AddTimedEventComponentStart(stateManager.player, 0.0f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
			
		}
		if ((currentStates & InMainMenu) == true)
		{
			ReleaseTimedEvents(stateManager.stage);
			
			AddTimedEventComponentStart(stateManager.stage, 1.0f, LoopSpawnMainMenuEnemy, skeleton, 2);
			if (keyState[SCANCODE_SPACE] == pressed)
			{
				AddTimedEventComponentStart(stateManager.stage, (float)(rand() % 16) + 8.0f, MainMenuIntroCutscene, 0, 8);
				Camera::SetCutsceneMode(false);
			}
			else
			{
				AddTimedEventComponentStart(stateManager.stage, 0.0f, MainMenuIntroCutscene, 0, 8);
			}
		}
	}
#ifdef _DEBUG
	if (keyInput[SCANCODE_LCTRL] == down)
	{
		if (keyState[SCANCODE_1] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, skeleton, 256);
		}
		else if (keyState[SCANCODE_2] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, hellhound, 256);
		}
		else if (keyState[SCANCODE_3] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, eye, 256);
		}
		else if (keyState[SCANCODE_4] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, tempBoss, 256);
		}
	}
	if (keyInput[SCANCODE_H] == down)
	{
		if (keyState[SCANCODE_A] == pressed)
		{
			for (size_t i = 0; i < 8; i++)
			{
				hitboxVisualizerActive[i] = true;
				for (auto entity : View<HitboxComponent>(registry))
				{
					VisualizeHitbox(entity, i);
				}
			}
		}
		if (keyState[SCANCODE_0] == pressed)
		{
			for (auto entity : View<HitboxComponent>(registry))
			{
				VisualizeHitbox(entity, 0); 
			}
			hitboxVisualizerActive[0] = true;
		}
		else if (keyState[SCANCODE_1] == pressed)
		{
			for (auto entity : View<HitboxComponent>(registry))
			{
				VisualizeHitbox(entity, 1);
			}
			hitboxVisualizerActive[1] = true;
		}
		else if (keyState[SCANCODE_2] == pressed)
		{
			for (auto entity : View<HitboxComponent>(registry))
			{
				VisualizeHitbox(entity, 2);
			}
			hitboxVisualizerActive[2] = true;
		}
		else if (keyState[SCANCODE_3] == pressed)
		{
			for (auto entity : View<HitboxComponent>(registry))
			{
				VisualizeHitbox(entity, 3);
			}
			hitboxVisualizerActive[3] = true;
		}
		else if (keyState[SCANCODE_4] == pressed)
		{
			for (auto entity : View<HitboxComponent>(registry))
			{
				VisualizeHitbox(entity, 4);
			}
			hitboxVisualizerActive[4] = true;
		}
		else if (keyState[SCANCODE_LALT] == pressed || keyState[SCANCODE_LSHIFT] == pressed || keyState[SCANCODE_LCTRL] == pressed)
		{
			for (auto entity : View<HitboxComponent>(registry))
			{
				StopVisualizeHitbox(entity);
			}
			for (size_t i = 0; i < SAME_TYPE_HITBOX_LIMIT *2; i++)
			{
				hitboxVisualizerActive[i] = false;
			}
		}
	}
#endif // _DEBUG

	

	

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
		controller->goalX = 0.0f;
		controller->goalZ = 0.0f;

		//Default the animation to idle, subject to be changed based off of user input
		AnimationComponent* anim = registry.GetComponent<AnimationComponent>(entity);

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
	
			TransformAccelerate(entity, controller->goalX, controller->goalZ);
		
			/*SmoothRotation(transform, controller->goalX, controller->goalZ, 8.0f);*/
			SmoothRotation(transform, MouseComponentGetDirectionX(mouseComponent), MouseComponentGetDirectionZ(mouseComponent), 16.0f);
		}


		//clamp moveTime to lower limit if not moving
		else 
		{
			anim->aAnim = ANIMATION_IDLE;
			anim->aAnimIdx = 0;
			
			SmoothRotation(transform, MouseComponentGetDirectionX(mouseComponent), MouseComponentGetDirectionZ(mouseComponent), 16.0f);
			TransformDecelerate(entity);
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
		//Attack will now actually be more interesting. Duration of the continuous function in the timed event will now depend on which hit in the chain we're doing
		//Need: Variable storing time between inputs. If an attack happens within a certain time after another, the next attack in the chain. So also need a variable 
		//keeping track of which attack in the chain we did last
		//PlayerComponent now stores these two values, so we need to get access to it early
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
		
		//Only increment if we're not currently attacking, so we don't accidentally reset our attack chain while we're mid-combo
		if(player->isAttacking == false)
			player->timeSinceLastAttack += GetDeltaTime();

		//Clamp and reset attack chain if more than half a second has passed
		if (player->timeSinceLastAttack > 0.5f)
		{
			player->attackChainIndex = 0;
			player->timeSinceLastAttack = 0.0f;
		}

		//Schwing (Now with 50% less carpal tunnel)
		if (mouseButtonDown[0] == down && player->isAttacking == false)
		{
			StatComponent* playerStats = registry.GetComponent<StatComponent>(entity);
			float attackDuration = 5.0f;

			//Todo (if we get more weapons): Let there be some WeaponComponent that has its own attack chains and animation timings so it doesn't get hard-coded here
			if (player->attackChainIndex == 0) //First attack in the chain
			{
				player->attackChainIndex = 1;
				attackDuration = 0.5f;
				//attackDuration = 0.6f;
			}
			else //
			{
				if (player->attackChainIndex == 1) //Second attack in the chain
				{
					player->attackChainIndex = 2;
					attackDuration = 0.4f;
				}
				else //Third and final attack in the chain, resets attackChainIndex
				{
					player->attackChainIndex = 0;
					attackDuration = 0.7f;
					//attackDuration = 0.8f;
				}
				
			}
			attackDuration /= playerStats->GetAttackSpeed(); //Speed up the attack animation based on attack speed
			registry.AddComponent<AttackArgumentComponent>(entity, attackDuration);
			//AddTimedEventComponentStartEnd(entity, 0.0f, ResetAnimation, 1.0f, nullptr, 1);
			AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerBeginAttack, PlayerAttack, attackDuration, PlayerEndAttack); //Esketit xd
		}
		else if (mouseButtonDown[1] == down && player->currentCharge < player->maxCharge)
		{
			auto stats = registry.GetComponent<StatComponent>(entity);
			if (stats)
				stats->SetSpeedMult(0.2f);
			player->currentCharge += GetDeltaTime();
			if (player->currentCharge > player->maxCharge) //clamp, since I'm going to let this number modify damage
				player->currentCharge = player->maxCharge;
			//Play some sound, do some animation, indicate that we're charging the bigboy move
		}
		else //If you're not holding down any mouse button, see if we have any heavy-attack charge
		{
			if (player->currentCharge > 0.0f)
			{
				//it's time
				StatComponent* playerStats = registry.GetComponent<StatComponent>(entity);
				float attackDuration = 1.0f / playerStats->GetAttackSpeed();
				registry.AddComponent<AttackArgumentComponent>(entity, attackDuration);
				registry.AddComponent<ChargeAttackArgumentComponent>(entity, 1.0f + player->currentCharge);
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerBeginAttack, PlayerAttack, attackDuration, PlayerEndAttack);
			}
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