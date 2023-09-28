#pragma once

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
	int Setup();

	void SetLife(float seconds);
	void SetMaxRange(float radius);
	void SetSize(float size);
	void SetPattern(ComputeShaders pattern);
	void SetPosition(float x, float y, float z);
};