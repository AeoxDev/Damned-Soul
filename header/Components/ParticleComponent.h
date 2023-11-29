#pragma once
#include "EntityID.h"

enum ComputeShaders
{
	SMOKE = 0,
	ARCH = 1,
	EXPLOSION = 2,
	FLAMETHROWER = 3,
	IMPLOSION = 4,
	RAIN = 5,
	SINUS = 6,
	LIGHTNING = 7,
	CIRCLE_FIELD = 8,
	FIRE = 9,
	PULSE = 10,
	ICETHROWER = 11,
	BOILING = 12,
	SPARK = 13,
	NO_MOVEMENT = 14
};

enum VFX_PATTERN
{
	FIRE = 0,
	ACID = 1,
	SWORD = 2
};

//(entity, float seconds, float radius, float size, float x, float y, float z, int amount, ComputeShaders pattern)
struct ParticleComponent
{
	int metadataSlot = -1;
	int groupsRequiered = -1;
	bool modelUse = false;
	// The mesh VFX
	uint64_t model;
	float meshMiddle[3];
	

	// -- Functions defined in Particles.cpp --//
	// Finds the index of the metadata for this component
	int FindSlot();

	ParticleComponent(float seconds, float radius, float size, float offsetX, float offsetY, float offsetZ, int amount, ComputeShaders pattern);
	// Overload for Pulse
	ParticleComponent(float seconds, float radius, float size, float offsetX, float offsetY, float offsetZ, float speed, int amount, ComputeShaders pattern);
	// Overload for VFX 
	ParticleComponent(float seconds, float radius, float size, float offsetX, float offsetY, float offsetZ, int amount, VFX_PATTERN vfxPattern, ComputeShaders pattern);
	ParticleComponent(float seconds, float radius, float size, float x, float y, float z, int amount, const char* filename, ComputeShaders pattern);

	// Overload for FlameThrower
	// Flamethrower does not use max max distance therefore we store one of the triangles vertices x-position there.
	ParticleComponent(float seconds, float v0X, float size, float offsetX, float offsetY, float offsetZ, float rotationY, float v0Z, float v1X, float v1Z, float v2X, float v2Z, int amount, ComputeShaders pattern);
	~ParticleComponent();


	//void SetupFlamethrower(float seconds, float radius, float size, float x, float y, float z);
	void Release();
};

