#pragma once

#include "EntityFramework.h"

enum enemyType
{
	invalidType = -1,
	hellhound,
	skeleton,
	eye,
	tempBoss
};


void SetupEnemy(EntityID& entity, enemyType eType, float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f, float mass = 1.f,
	float health = 100.f, float moveSpeed = 10.f, float damage = 10.f, float attackSpeed = 1.f, int soulWorth = 1, float facingX = 1.f,
	float facingY = 0.f, float facingZ = 1.f, float scaleX = 1.f, float scaleY = 1.f, float scaleZ = 1.f);

void CreatePlayer(float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f, float mass = 1.f,
	float health = 100.f, float moveSpeed = 10.f, float damage = 10.f, float attackSpeed = 1.f, int soulWorth = 1, float facingX = 1.f,
	float facingY = 0.f, float facingZ = 1.f, float scaleX = 1.f, float scaleY = 1.f, float scaleZ = 1.f);

void SetPlayerPosition(float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f);

void LoadPlayerSounds();