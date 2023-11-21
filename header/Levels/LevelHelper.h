#pragma once

#include "EntityFramework.h"
#include "EnemyType.h"
#include "StaticHazardComponent.h"
struct StageSetupVariables
{
	int stageNr = 0; 
	float rm = 1.0f, gm = 1.0f, bm = 1.0f; 
	float ra = .0f, ga = .0f, ba = .0f; 
	float gamma = 1.0f;
	float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
	float offsetX = .0f, offsetY = .0f, offsetZ = .0f;
};
EntityID SetUpStage(StageSetupVariables& stageVars);

EntityID SetUpHazard(const StaticHazardType& type, const float scale, const float ra, const float ga, const float ba, const float rm, const float gm, const float bm, const float gamma);

EntityID SetupEnemy(EnemyType eType, float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f, float mass = 6969.f,
	float health = 6969.f, float moveSpeed = 6969.f, float damage = 6969.f, float attackSpeed = 6969.f, int soulWorth = 6969, float scaleX = 1.f, 
	float scaleY = 1.f, float scaleZ = 1.f, float facingX = 0.f,float facingY = 0.f, float facingZ = -1.f);

void CreatePlayer(float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f, float mass = 1.f,
	float health = 100.f, float moveSpeed = 10.f, float damage = 10.f, float attackSpeed = 1.f, int soulWorth = 1, float facingX = 1.f,
	float facingY = 0.f, float facingZ = 1.f, float scaleX = 1.f, float scaleY = 1.f, float scaleZ = 1.f);

void SetPlayerPosition(float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f);

void LoadPlayerSounds();

void ReloadPlayerNonGlobals();

EntityID RandomPlayerEnemy(EnemyType enemyType);