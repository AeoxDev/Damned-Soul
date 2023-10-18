#pragma once
#include "EntityFramework.h"

void RandomPosition(EntityID& entity);

/*PLAYER EVENTS*/
//START
void PlayerLoseControl(EntityID& entity);
void SetPlayerAttackHitboxActive(EntityID& entity);

//END
void PlayerRegainControl(EntityID& entity);
void SetPlayerAttackHitboxInactive(EntityID& entity);

//CONTINUOUS
void PlayerAttack(EntityID& entity);
void PlayerDash(EntityID& entity);
/*END OF PLAYER EVENTS*/

//Portal
void CreatePortal(EntityID& entity);
//void EnemyExclusion(EntityID& entity); not needed?
void PlayDeathAnimation(EntityID& entity);
void RemoveEnemy(EntityID& entity);