#pragma once
#include "GameRenderer.h"

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

	ParticleComponent(RenderSetupComponent constantBuffer[8], int RenderSlot, float seconds, float radius, float size, float x, float y, float z, ComputeShaders pattern);
	~ParticleComponent();

	void Setup(RenderSetupComponent constantBuffer[8], int RenderSlot, float seconds, float radius, float size, float x, float y, float z, ComputeShaders pattern);
};