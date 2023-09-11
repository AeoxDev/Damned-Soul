#pragma once
#include <DirectXMath.h>
#include "MemLib/PoolPointer.hpp"

__declspec(align(16)) struct Particle
{	
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_rgb;
};

//Compute - index 1
//index: collection metadata

struct ParticleMetadata
{
	int m_start, m_end;//Which particles in the array to work with
	float m_life;//if < 0.0, no calcs
	float m_maxRange;

	DirectX::XMFLOAT3 m_spawnPos;
};

extern int16_t m_metadata;

extern PoolPointer<Particle> m_particles;

extern int8_t m_computeShaders[8]; //The amount of compute shaders
extern int8_t m_UAVBuffers[2]; //0 starts as input, 1 starts as output, they switch each frame
extern int8_t m_SRVBuffers[2]; // ------------------------||----------------------------------


void InitializeParticles();

//Returns the first created particle buffer
int8_t GetFirstParticle();
//Returns the second created particle buffer
int8_t GetSecondParticle();


