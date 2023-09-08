#pragma once
#include "D3D11Graphics.h"
#include <cstring>
#include <string>

struct StatusComponent
{
	int hp = 50;

	StatusComponent() = default;
	StatusComponent(int n) : hp(n) {}
};

struct PlayerComponent
{
#define MAX_LENGTH 16
	char name[MAX_LENGTH] = "Default Name";

	PlayerComponent() = default;
	PlayerComponent(const char s[MAX_LENGTH])
	{
		std::memcpy(name, s, MAX_LENGTH);
	}
};

struct EnemyComponent
{
#define MAX_LENGTH 16
	char name[MAX_LENGTH] = "Default Name";

	EnemyComponent() = default;
	EnemyComponent(const char s[MAX_LENGTH])
	{
		std::memcpy(name, s, MAX_LENGTH);
	}
};

struct WackyComponent
{
	int hp = 0;
	float speed = 5.5f;
	char name[16] = "Hi";

	WackyComponent(int hp, float speed, PlayerComponent pc) : hp(hp), speed(speed) 
	{
		std::memcpy(this->name, pc.name, 16);
	}
};

struct GraphicsComponent
{
	//posX, posY, texture..
};

struct PhysicsComponent
{
	float mass;
	float lastPosX;
	float lastPosY;
};

struct CircleColliderComponent
{
	float radius, offsetX, offsetY;
};

struct ConvexColliderComponent
{
	float centerX, centerY;

	int cornerAmount;
	float cornerX[16], cornerY[16];
};

struct GeometryIndependentColliderComponent
{
	uint8_t pixelMap[512 * 512]; //#define 512*512 as size? or rather just 512 as dimension
	float offsetX, offsetY;
};

//Components:
/*
Graphics (d3d11 stuff, transform too)
Status (hp, movespeed, attack speed, damage)
Player (souls collected)
Enemy (possibly some bool checking to see if they're in an attack animation)
Weapon (move attack speed and damage variables here? depends how we want to do things)
StaticHazard (damage)
*/