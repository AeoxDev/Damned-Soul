#pragma once
#include "EntityFramework.h"

void RandomPosition(EntityID& entity);

/*PLAYER EVENTS*/
void PlayerLoseControl(EntityID& entity);
void PlayerRegainControl(EntityID& entity);
void PlayerBecomeInvincible(EntityID& entity);
void PlayerBecomeDamageable(EntityID& entity);
void PlayerAttack(EntityID& entity);
void PlayerDash(EntityID& entity);

//Portal
void CreatePortal(EntityID& entity);
//void EnemyExclusion(EntityID& entity); not needed?
void PlayDeathAnimation(EntityID& entity);
void RemoveEnemy(EntityID& entity);