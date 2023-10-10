#pragma once
#include "EntityFramework.h"

void RandomPosition(EntityID& entity);

/*PLAYER EVENTS*/
void PlayerLoseControl(EntityID& entity);
void PlayerRegainControl(EntityID& entity);
void PlayerDash(EntityID& entity);

//Portal
void CreatePortal(EntityID& entity);