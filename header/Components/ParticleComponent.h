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
	// Doesnt really work yet, might get deprecated
	SPAWN = 12
};

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

	ParticleComponent(float seconds, float radius, float size, float x, float y, float z, int amount, ComputeShaders pattern);
	// Overload for VFX mesh
	ParticleComponent(float seconds, float radius, float size, float x, float y, float z, int amount, const char* filename, ComputeShaders pattern);

	// Overload for FlameThrower
	// Flamethrower does not use max max distance therefore we store one of the triangles vertices x-position there.
	ParticleComponent(float seconds, float v0X, float size, float x, float y, float z, float rotationY, float v0Z, float v1X, float v1Z, float v2X, float v2Z, int amount, ComputeShaders pattern);
	~ParticleComponent();


	//void SetupFlamethrower(float seconds, float radius, float size, float x, float y, float z);
	void Release();
};

