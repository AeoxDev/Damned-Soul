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
	FIRE = 9
};

struct ParticleComponent
{
	int metadataSlot = -1;
	int groupsRequiered = -1;
	// If this is false constant buffer has not been reset
	bool bufferReset = false;
	// If this is false the particles in the buffer has not been reset
	bool vertexReset = false;

	// -- Functions defined in Particles.cpp --//
	// Finds the index of the metadata for this component
	int FindSlot();

	ParticleComponent(float seconds, float radius, float size, float x, float y, float z, int amount, ComputeShaders pattern);

	// Overload for FlameThrower
	// Flamethrower does not use max max distance therefore we store one of the triangles vertices x-position there.
	ParticleComponent(float seconds, float v0X, float size, float x, float y, float z, float rotationY, float v0Z, float v1X, float v1Z, float v2X, float v2Z, int amount, ComputeShaders pattern);
	~ParticleComponent();


	//void SetupFlamethrower(float seconds, float radius, float size, float x, float y, float z);
	void Release();
	void RemoveParticles(EntityID& entity);
	void ResetBuffer();
	void ResetVertex();

};

