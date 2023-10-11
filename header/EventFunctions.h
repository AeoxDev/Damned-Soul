#pragma once
#include "EntityFramework.h"

void RandomPosition(EntityID& entity);

/*PLAYER EVENTS*/
void PlayerLoseControl(EntityID& entity);
void PlayerRegainControl(EntityID& entity);
void PlayerDash(EntityID& entity);
//void EnemyExclusion(EntityID& entity); not needed?
void PlayDeathAnimation(EntityID& entity);
void RemoveEnemy(EntityID& entity);