#pragma once
#include "EntityFramework.h"

//void RandomPosition(EntityID& entity, const int& index);

//Events for any entity capable of being attacked
void BeginHit(EntityID& entity, const int& index);
void MiddleHit(EntityID& entity, const int& index);
void EndHit(EntityID& entity, const int& index);

/*PLAYER EVENTS*/
//START
void PlayerLoseControl(EntityID& entity, const int& index);
void SetPlayerAttackHitboxActive(EntityID& entity, const int& index);

//END
void PlayerRegainControl(EntityID& entity, const int& index);
void SetPlayerAttackHitboxInactive(EntityID& entity, const int& index);

//CONTINUOUS
void PlayerAttack(EntityID& entity, const int& index);
void PlayerDash(EntityID& entity, const int& index);
/*END OF PLAYER EVENTS*/

//Portal
void CreatePortal(EntityID& entity, const int& index);
//void EnemyExclusion(EntityID& entity); not needed?
void PlayDeathAnimation(EntityID& entity, const int& index);
void RemoveEnemy(EntityID& entity, const int& index);
void SplitBoss(EntityID& entity, const int& index);