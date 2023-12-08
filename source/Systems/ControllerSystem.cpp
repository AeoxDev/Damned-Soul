#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "EventFunctions.h"
#include "States\StateManager.h"
#include "Camera.h"
#include "Model.h"
#include "Level.h"
#include "Levels\LevelHelper.h"
#include "Levels\LevelLoader.h"
#include "UIButtonFunctions.h"

bool ControllerSystem::Update()
{
	//Controller for player during play
	if ((keyState[SCANCODE_SPACE] == pressed))
	{
		if (!(currentStates & InMainMenu) && Camera::InCutscene() > 0 && !(currentStates & InCredits) && !(currentStates & InSettings))
		{
			if (Camera::InCutscene() == 1)
			{
				//Skip the cutscene
				for (auto entity : View<TimedEventComponent>(registry))
				{
					ReleaseTimedEvents(entity);
				}
				ReloadPlayerNonGlobals();
				if (stateManager.cutsceneEnemy.index != -1)
				{
					registry.DestroyEntity(stateManager.cutsceneEnemy);
					stateManager.cutsceneEnemy.index = -1;
				}
				AddTimedEventComponentStart(stateManager.player, 0.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
				AddTimedEventComponentStart(stateManager.player, 0.0f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
			}
			else if (Camera::InCutscene() == 2)
			{
				for (auto entity : View<TimedEventComponent>(registry))
				{
					ReleaseTimedEvents(entity);
				}
				AddTimedEventComponentStart(stateManager.player, 0.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
				AddTimedEventComponentStart(stateManager.player, 0.0f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
				for (auto entity : View<AudioEngineComponent>(registry))
				{
					AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
					audioJungle->HandleSound();
					audioJungle->StopAllSounds();
				}
				UIFunctions::Game::LoadNextLevel(nullptr, -1);
			}
			else if (Camera::InCutscene() == 3)
			{
				//Nothing. Not a skippable cutscene.
#ifdef _DEBUG //Allow debug to skip the cutscene
				for (auto entity : View<TimedEventComponent>(registry))
				{
					ReleaseTimedEvents(entity);
				}
				ReloadPlayerNonGlobals();
				AddTimedEventComponentStart(stateManager.player, 0.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
				AddTimedEventComponentStart(stateManager.player, 0.0f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
#endif // _DEBUG //Allow debug to skip the cutscene

			}
		}
		
	}
	if ((currentStates & InMainMenu) == true && (keyState[SCANCODE_SPACE] == pressed || mouseButtonPressed[left] == pressed))
	{
		ReleaseTimedEvents(stateManager.stage);

		AddTimedEventComponentStart(stateManager.stage, 0.1f, LoopSpawnMainMenuEnemy, skeleton, 8);
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
#ifdef _DEBUG
	if (keyInput[SCANCODE_LCTRL] == down)
	{
		if (keyState[SCANCODE_1] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, skeleton, 256);
		}
		else if (keyState[SCANCODE_2] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, imp, 256);
		}
		else if (keyState[SCANCODE_3] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, hellhound, 256);
		}
		else if (keyState[SCANCODE_4] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, tempBoss, 256);
		}
		else if (keyState[SCANCODE_5] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, eye, 256);
		}
		else if (keyState[SCANCODE_6] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, empoweredSkeleton, 256);
		}
		else if (keyState[SCANCODE_7] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, minotaur, 256);
		}
		else if (keyState[SCANCODE_8] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, empoweredHellhound, 256);
		}
		else if (keyState[SCANCODE_9] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, empoweredImp, 256);
		}
		else if (keyState[SCANCODE_0] == pressed)
		{
			AddTimedEventComponentStart(stateManager.stage, 0.0f, SpawnMainMenuEnemy, lucifer, 256);
		}
	}
	if (keyInput[SCANCODE_L] == down)
	{
		if (keyState[SCANCODE_1] == pressed)
		{
			LoadLevel(1);
			stateManager.activeLevel = 1;
		}
		else if (keyState[SCANCODE_2] == pressed)
		{
			LoadLevel(3);
			stateManager.activeLevel = 3;
		}
		else if (keyState[SCANCODE_3] == pressed)
		{
			stateManager.activeLevel = 5;
			LoadLevel(5);
		}
		else if (keyState[SCANCODE_4] == pressed)
		{
			LoadLevel(7);
		}
		else if (keyState[SCANCODE_5] == pressed)
		{
			LoadLevel(9);
		}
		else if (keyState[SCANCODE_6] == pressed)
		{
			LoadLevel(11);
		}
		else if (keyState[SCANCODE_7] == pressed)
		{
			LoadLevel(13);
		}
		else if (keyState[SCANCODE_8] == pressed)
		{
			LoadLevel(15);
		}
		else if (keyState[SCANCODE_9] == pressed)
		{
			LoadLevel(17);
		}
		else if (keyState[SCANCODE_0] == pressed)//Reset shop, do this only once per game
		{
			LoadLevel(2);
		}
		else if (keyState[SCANCODE_S] == pressed)//Do this many times
		{
			LoadLevel(4);
		}
	}
	if (keyInput[SCANCODE_L] == down)
	{
		if (keyState[SCANCODE_1] == pressed)
		{
			LoadLevel(1);
		}
		else if (keyState[SCANCODE_2] == pressed)
		{
			LoadLevel(3);
		}
		else if (keyState[SCANCODE_3] == pressed)
		{
			LoadLevel(5);
			stateManager.activeLevel = 5;
		}
		else if (keyState[SCANCODE_4] == pressed)
		{
			LoadLevel(7);
			stateManager.activeLevel = 7;
		}
		else if (keyState[SCANCODE_5] == pressed)
		{
			LoadLevel(9);
			stateManager.activeLevel = 9;
		}
		else if (keyState[SCANCODE_6] == pressed)
		{
			LoadLevel(11);
			stateManager.activeLevel = 11;
		}
		else if (keyState[SCANCODE_7] == pressed)
		{
			LoadLevel(13);
			stateManager.activeLevel = 13;
		}
		else if (keyState[SCANCODE_8] == pressed)
		{
			LoadLevel(15);
			stateManager.activeLevel = 15;
		}
		else if (keyState[SCANCODE_9] == pressed)
		{
			LoadLevel(17);
			stateManager.activeLevel = 17;
		}
		else if (keyState[SCANCODE_0] == pressed)//Reset shop, do this only once per game
		{
			LoadLevel(2);
			stateManager.activeLevel = 2;
		}
		else if (keyState[SCANCODE_S] == pressed)//Do this many times
		{
			LoadLevel(4);
			stateManager.activeLevel = 4;
		}
	}
	if (keyInput[SCANCODE_H] == down)
	{
		if (keyState[SCANCODE_A] == pressed)
		{
			for (int i = 0; i < 8; i++)
			{
				hitboxVisualizerActive[i] = true;
				for (auto entity : View<HitboxComponent>(registry))
				{
					VisualizeHitbox(entity, i);
				}
			}
		}
		if (keyState[SCANCODE_G] == pressed)
		{
			SetHitboxIsMoveable(stateManager.player, 0, false);
			SetHitboxIsMoveable(stateManager.player, 1, false);
		}
		if (keyState[SCANCODE_I] == pressed)
		{
			SetHitboxIsMoveable(stateManager.player, 0, true);
			SetHitboxIsMoveable(stateManager.player, 1, true);
		}
		if (keyState[SCANCODE_S] == pressed)
		{
			if (stateManager.hitboxVis.index == -1)
			{
				stateManager.hitboxVis = registry.CreateEntity();
				stateManager.naviagtion = registry.CreateEntity();
				ModelBonelessComponent* stageHitbox;
				TransformComponent* transform;
				visualizeStage = true;
				switch (stateManager.activeLevel)
				{
					case 1://Level 1
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV1Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV1Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 3://Level 2
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV2Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV2Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 5://Level 3
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV3Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV3Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 7://Level 4
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV4Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV4Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 9://Level 5
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV5Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV5Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 11://Level 6
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV6Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV6Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 13://Level 7
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV7Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV7Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 15://Level 8
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV8Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV8Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
					case 17://Level 9
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.hitboxVis, LoadModel("LV9Hitbox.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.hitboxVis);
						stageHitbox = registry.AddComponent<ModelBonelessComponent>(stateManager.naviagtion, LoadModel("LV9Nav.mdl"));
						transform = registry.AddComponent<TransformComponent>(stateManager.naviagtion);
						break;
				default:
					break;
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
			if (stateManager.hitboxVis.index != -1)
			{
				registry.DestroyEntity(stateManager.hitboxVis);
				stateManager.hitboxVis.index = -1;
				visualizeStage = false;
				registry.DestroyEntity(stateManager.naviagtion);
				stateManager.naviagtion.index = -1;
			}

		}
	}
	if (keyInput[SCANCODE_LSHIFT] == down || keyInput[SCANCODE_RSHIFT] == down)
	{
		if (keyState[SCANCODE_UP] == pressed)
		{
			gameSpeed += 0.1f;
		}
		if (keyState[SCANCODE_DOWN] == pressed)
		{
			gameSpeed -= 0.1f;
		}
		if (keyState[SCANCODE_RIGHT] == pressed)
		{
			gameSpeed = 1.0f;
		}
		if (keyState[SCANCODE_LEFT] == pressed)
		{
			gameSpeed = 0.0f;
		}
	}
#endif // _DEBUG

	
	//Bugfix, player able to make moves before cutscenes, causing glitches.
	if (Camera::InCutscene() != 0)
	{
		return true;
	}

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

		//PlayerComponent now stores a bunch of variables for cooldowns and animation timings so we need access to it early
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);

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
			
			//transform->positionZ += stat->moveSpeed * GetDeltaTime();
			controller->goalZ += 1.0f;
		}
		if (keyInput[SCANCODE_S] == down)
		{
			//transform->positionZ -= stat->moveSpeed * GetDeltaTime();
			controller->goalZ -= 1.0f;
		}
		if (keyInput[SCANCODE_A] == down)
		{
			//transform->positionX -= stat->moveSpeed * GetDeltaTime();
			controller->goalX -= 1.0f;
		}
		if (keyInput[SCANCODE_D] == down)
		{
			
			//transform->positionX += stat->moveSpeed * GetDeltaTime();
			controller->goalX += 1.0f;
		}

		//Update facing based off of mouse position (but only if we aren't currently attacking, you'd better commit)
		if (!player->isAttacking)
		{
			MouseComponentUpdateDirection(entity);

		}
		
		if ((controller->goalX * controller->goalX + controller->goalZ * controller->goalZ) > 0.0f)
		{
			moving = true;
		}

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

			//Elliot: Make character face the walking direction
			if (!player->isAttacking && player->currentCharge < 0.01f)
			{
				SmoothRotation(transform, controller->goalX, controller->goalZ, 32.0f);
			}
			
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
		/*DASH*/
		//Decrement and clamp
		player->DashCooldown(GetDeltaTime());

		//Dash in the direction you're moving, defaults to dashing backwards if you're not moving. Gives i-frames for the duration
		if (keyState[SCANCODE_SPACE] == pressed && player->ConsumeDash())
		{
			//player->dashCounter = player->dashCooldown; // Not handled in ConsumeDash()
			//Putting cooldown on these dashes, PlayerComponent has the variables in charge of both current counter and the max-value
			if (moving)
			{
				//Set facing direction to dash direction when moving
				transform->facingX = controller->goalX;
				transform->facingZ = controller->goalZ;
				registry.AddComponent<DashArgumentComponent>(entity, transform->facingX, transform->facingZ, player->GetDashValue());
				AddTimedEventComponentStart(entity, 0.0f, PlayerDashSound, CONDITION_DASH);
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerLoseControl, PlayerDash, 0.2f, PlayerRegainControl, CONDITION_DASH);
				AddSquashStretch(entity, Linear, 0.8f, 0.8f, 1.5f, true, 1.2f, 1.2f, 0.8f);
				int squashStretch = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, ResetSquashStretch, SquashStretch, 0.2f, ResetSquashStretch, 0, 1);
			}
			else
			{
				//Default dash goes forwards
				//Set facing direction to dash direction when moving
				registry.AddComponent<DashArgumentComponent>(entity, transform->facingX, transform->facingZ, 2.5f);
				AddTimedEventComponentStart(entity, 0.0f, PlayerDashSound, CONDITION_DASH);
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerLoseControl, PlayerDash, 0.2f, PlayerRegainControl, CONDITION_DASH);
				AddSquashStretch(entity, Linear, 0.8f, 0.8f, 1.5f, true, 1.2f, 1.2f, 0.8f);
				int squashStretch = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, ResetSquashStretch, SquashStretch, 0.2f, ResetSquashStretch, 0, 1);
				break;
			}
		}

		//Switches animation to attack and deals damage in front of yourself halfway through the animation (offset attack hitbox)
		//Attack will now actually be more interesting. Duration of the continuous function in the timed event will now depend on which hit in the chain we're doing
		//Only increment if we're not currently attacking, so we don't accidentally reset our attack chain while we're mid-combo
		if(player->isAttacking == false)
			player->timeSinceLastAttack += GetDeltaTime();

		//Clamp and reset attack chain if more than half a second has passed
		if (player->timeSinceLastAttack > 0.5f)
		{
			player->attackChainIndex = 0;
			player->timeSinceLastAttack = 0.0f;
		}

		//Get our sound component so we can play sounds when we attack
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);

		//Schwing (Now with 50% less carpal tunnel)
		if (mouseButtonDown[0] == down && player->isAttacking == false)
		{
			StatComponent* playerStats = registry.GetComponent<StatComponent>(entity);
			float attackDuration = 5.0f;

			//Todo (if we get more weapons): Let there be some WeaponComponent that has its own attack chains and animation timings so it doesn't get hard-coded here
			if (player->attackChainIndex == 0) //First attack in the chain
			{
				if (sfx) sfx->Play(Player_Attack1, Channel_Base); //Attack1
				player->attackChainIndex = 1;
				attackDuration = 0.5f;
				//attackDuration = 0.6f;
			}
			else //
			{
				if (player->attackChainIndex == 1) //Second attack in the chain
				{
					if(sfx) sfx->Play(Player_Attack2, Channel_Base); //Attack2
					player->attackChainIndex = 2;
					attackDuration = 0.4f;
				}
				else //Third and final attack in the chain, resets attackChainIndex
				{
					if (sfx) sfx->Play(Player_Attack3, Channel_Base); //Attack3
					player->attackChainIndex = 0;
					attackDuration = 0.7f;
					//attackDuration = 0.8f;
				}
				
			}

			//Make player immediately face in our mouse direction when we attack for better directional combat
			transform->facingX = MouseComponentGetDirectionX(mouseComponent);
			transform->facingZ = MouseComponentGetDirectionZ(mouseComponent);

			attackDuration /= playerStats->GetAttackSpeed(); //Speed up the attack animation based on attack speed
			registry.AddComponent<AttackArgumentComponent>(entity, attackDuration);
			//AddTimedEventComponentStartEnd(entity, 0.0f, ResetAnimation, 1.0f, nullptr, 1);
			AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerBeginAttack, PlayerAttack, attackDuration, PlayerEndAttack); //Esketit xd
		}
		else if (mouseButtonDown[1] == down && player->currentCharge < player->maxCharge && player->isAttacking != true)
		{
			//Elliot: Make character slowly face the attacking direction
			SmoothRotation(transform, MouseComponentGetDirectionX(mouseComponent), MouseComponentGetDirectionZ(mouseComponent), 8.0f);

			for (auto audio : View<AudioEngineComponent>(registry))
			{
				AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(audio);
				FMOD::Sound* test = nullptr;
				if (sfx)
				{
					audioJungle->channels[sfx->channelIndex[Channel_Base]]->getCurrentSound(&test);
					if (audioJungle->sounds[PLAYER4] != test)
					{
						sfx->Play(Player_AttackHeavyCharge, Channel_Base);
					}
				}
			}
			auto stats = registry.GetComponent<StatComponent>(entity);
			if (stats)
				stats->SetSpeedMult(0.2f);
			/*player->currentCharge += GetDeltaTime();*/
			player->currentCharge += GetDeltaTime() * stats->GetAttackSpeed(); //Charge faster scaling off of attack speed baby
			auto skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
			if (skelel) //Gradually light up player when charging heavy attack
			{
				skelel->shared.bcaR_temp = player->currentCharge * 0.5f;
				skelel->shared.bcaG_temp = player->currentCharge * 0.5f;
				skelel->shared.bcaB_temp = player->currentCharge * 0.5f;
			}
			if (player->currentCharge > player->maxCharge) //clamp, since I'm going to let this number modify damage
				player->currentCharge = player->maxCharge;
			//Play some sound, do some animation, indicate that we're charging the bigboy move
		}
		else //If you're not holding down any mouse button, see if we have any heavy-attack charge
		{
			if (player->currentCharge > 0.0f)
			{
				//it's time
				if (sfx) sfx->Play(Player_HeavyAttack, Channel_Base);
				StatComponent* playerStats = registry.GetComponent<StatComponent>(entity);
				float attackDuration = 0.8f / playerStats->GetAttackSpeed();
				/*float attackDuration = 1.0f / playerStats->GetAttackSpeed();*/
				registry.AddComponent<AttackArgumentComponent>(entity, attackDuration);
				registry.AddComponent<ChargeAttackArgumentComponent>(entity, 1.0f + player->currentCharge);
				//Elliot Face the direction for consistency
				transform->facingX = MouseComponentGetDirectionX(mouseComponent);
				transform->facingZ = MouseComponentGetDirectionZ(mouseComponent);

				player->currentCharge = 0.0f;
				AddTimedEventComponentStartContinuousEnd(entity, 0.0f, PlayerBeginAttack, PlayerAttack, attackDuration, PlayerEndAttack);
			}
		}

#ifdef _DEBUG
		if (keyState[SCANCODE_G] == pressed) {
			StatComponent* pStats = registry.GetComponent<StatComponent>(stateManager.player);
			PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
			HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(stateManager.player);
			hitbox->circleHitbox[2].radius = 100.0f;
			if (GetGodModeFactor() <= 1.0f)
			{
				transform->mass += 1000.0f;
				player->killingSpree = 10000;
				pStats->UpdateBaseHealth(10000);
				pStats->ApplyHealing(10000);
				player->UpdateSouls(1000);
				pStats->UpdateBaseDamage(1000);
				hitbox->circleHitbox[2].radius += 100.0f;
				SetGodModeFactor(100.0f);
			}
			else
			{
				transform->mass -= 1000.0f;
				pStats->UpdateBaseHealth(-10000.f);
				pStats->ApplyDamage(1, false);
				pStats->CapHealth();
				pStats->UpdateBaseDamage(-1000);
				hitbox->circleHitbox[2].radius -= 100.0f;
				SetGodModeFactor(1.0f);
			}
		}
		if (keyState[SCANCODE_P] == pressed)
		{
			SetGodModePortal(true);
		}
#endif // _DEBUG

		//Update animation at the end of user input
		anim->aAnimTime += GetDeltaTime() * anim->aAnimTimeFactor;
		ANIM_BRANCHLESS(anim);

	}
	//Loop for player during other places
	return true;
}