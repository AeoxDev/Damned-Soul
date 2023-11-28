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

struct ModelTextRead
{
	EnemyType eType;
	float positionX;
	float positionZ;
	int soulValue;
};

bool SetupAllEnemies(std::string filePath);

EntityID SetUpStage(StageSetupVariables& stageVars);

EntityID SetUpHazard(const StaticHazardType& type, const float scale, const float ra, const float ga, const float ba, const float rm, const float gm, const float bm, const float gamma);

EntityID SetupEnemy(EnemyType eType, float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f, int soulWorth = 6969, float mass = 6969.f,
	float health = 6969.f, float moveSpeed = 6969.f, float damage = 6969.f, float attackSpeed = 6969.f,  float scaleX = 2.f,
	float scaleY = 2.f, float scaleZ = 2.f, float facingX = 0.f, float facingY = 0.f, float facingZ = -1.f, bool zacIndex1 = true, bool zacIndex2 = true, bool zacIndex3 = true,
	bool zacIndex4 = true, bool zacIndex5 = true, bool worthLess = true);

void CreatePlayer(float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f, float mass = 1.f,
	float health = 100.f, float moveSpeed = 10.f, float damage = 10.f, float attackSpeed = 1.f, int soulWorth = 1, float facingX = 1.f,
	float facingY = 0.f, float facingZ = 1.f, float scaleX = 1.8f, float scaleY = 1.8f, float scaleZ = 1.8f);

void SetPlayerPosition(float positionX = 0.f, float positionY = 0.f, float positionZ = 0.f);

void LoadPlayerSounds();

void ReloadPlayerNonGlobals();

EntityID RandomPlayerEnemy(EnemyType enemyType);

void SetScoreboardUI(EntityID stage);