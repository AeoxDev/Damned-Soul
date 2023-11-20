#pragma once
#include "EntityID.h"

//void RandomPosition(EntityID& entity, const int& index);

//Events for any entity capable of being attacked
void BeginHit(EntityID& entity, const int& index);
void MiddleHit(EntityID& entity, const int& index);
void EndHit(EntityID& entity, const int& index);

//AAAA
void DashBeginHit(EntityID& entity, const int& index);

//Hellhound breath hazard
void HazardBeginHit(EntityID& entity, const int& index);
void HazardEndHit(EntityID& entity, const int& index);
//Static Hazard Damage
void StaticHazardDamage(EntityID& entity, const int& index);
void LavaBlinkColor(EntityID& entity, const int& index);

/*PLAYER EVENTS*/
//START
void PlayerLoseControl(EntityID& entity, const int& index);
void SetPlayerAttackHitboxActive(EntityID& entity, const int& index);
void PlayerBeginAttack(EntityID& entity, const int& index);

//END
void PlayerRegainControl(EntityID& entity, const int& index);
void SetPlayerAttackHitboxInactive(EntityID& entity, const int& index);
void PlayerEndAttack(EntityID& entity, const int& index);

//CONTINUOUS
void PlayerDash(EntityID& entity, const int& index);
void PlayerDashSound(EntityID& entity, const int& index);
void PlayerAttack(EntityID& entity, const int& index);
/*END OF PLAYER EVENTS*/

//Portal
void CreatePortal(EntityID& entity, const int& index);
//void EnemyExclusion(EntityID& entity); not needed?
void PlayDeathAnimation(EntityID& entity, const int& index);
void RemoveEnemy(EntityID& entity, const int& index);
void SplitBoss(EntityID& entity, const int& index);
void BossShockwaveStart(EntityID& entity, const int& index);
void BossShockwaveExpand(EntityID& entity, const int& index);
void BossShockwaveEnd(EntityID& entity, const int& index);


//Enemy Combat functions
void EnemyAttackFlash(EntityID& entity, const int& index);
void EnemyAttackGradient(EntityID& entity, const int& index);
void EnemyBeginAttack(EntityID& entity, const int& index);
void EnemyEndAttack(EntityID& entity, const int& index);
void EnemyBecomeStunned(EntityID& entity, const int& index);

//Loop static hazard hitbox
void LoopHitbox(EntityID& entity, const int& index);

//Hit Stop
#define FREEZE_TIME 0.08f
void HitStop(EntityID& entity, const int& index);

//Animation functions
void PauseAnimation(EntityID& entity, const int& index);
void ContinueAnimation(EntityID& entity, const int& index);
void ResetAnimation(EntityID& entity, const int& index);
void BlinkColor(EntityID& entity, const int& index);
void ResetColor(EntityID& entity, const int& index);
void SquashStretch(EntityID& entity, const int& index);
void ResetSquashStretch(EntityID& entity, const int& index);


//Stats
void SetSpeedZero(EntityID& entity, const int& index);
void ResetSpeed(EntityID& entity, const int& index);

//Camera
#define CAMERA_CONSTANT_SHAKE_TIME 0.4f
void ShakeCamera(EntityID& entity, const int& index);
void ResetCameraOffset(EntityID& entity, const int& index);

//Spawning uses condition for type
void SpawnMainMenuEnemy(EntityID& entity, const int& index);
//Main menu loop for spawning random enemies
void LoopSpawnMainMenuEnemy(EntityID& entity, const int& index);

//Creates a linear transition with end time as the full time.
void CutsceneCreateLinearTransition(EntityID& entity, const int& index);
//Do the given cutscene components arguments over time
void BeginCutscene(EntityID& entity, const int& index);
void CutsceneTransition(EntityID& entity, const int& index);
void EndCutscene(EntityID& entity, const int& index);
void MainMenuIntroCutscene(EntityID& entity, const int& index);
void Stage1IntroScene(EntityID& entity, const int& index);
void Stage2IntroScene(EntityID& entity, const int& index);
void Stage3IntroScene(EntityID& entity, const int& index);
void Stage4IntroScene(EntityID& entity, const int& index);

void BeginDestroyProjectile(EntityID& entity, const int& index); 
void EndDestroyProjectile(EntityID& entity, const int& index);

//Game speed
void SetGameSpeedDefault(EntityID& entity, const int& index);