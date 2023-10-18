#pragma once
#include "GameRenderer.h"
#include "Particles.h"

enum ComputeShaders
{
	SMOKE = 0,
	ARCH = 1,
	EXPLOSION = 2,
	FLAMETHROWER = 3,
	IMPLOSION = 4,
	RAIN = 5,
	SINUS = 6,
};

struct ParticleComponent
{
	int metadataSlot = -1;

	// -- Functions defined in Particles.cpp --//
	// Finds the index of the metadata for this component
	int FindSlot();

	ParticleComponent(float seconds, float radius, float size, float x, float y, float z, ComputeShaders pattern);
	~ParticleComponent();


	void Setup(float seconds, float radius, float size, float x, float y, float z, ComputeShaders pattern);
	void Release();
};
