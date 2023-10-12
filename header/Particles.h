#pragma once
#include <DirectXMath.h>
#include "MemLib/PoolPointer.hpp"
#include "GameRenderer.h"
#include "ParticleComponent.h"

#define PARTICLE_METADATA_LIMIT 256
#define THREADS_PER_GROUP 256
#define MAX_PARTICLES PARTICLE_METADATA_LIMIT*THREADS_PER_GROUP

//The UAV is needed to write to the buffer, the SRV is needed to read while simoutanisly allowing for caching
//Slot [0] will always be read and slot [1] will always be write
struct ParticleInputOutput
{
	int8_t SRVIndex;
	int8_t UAVIndex;
};

struct Particle
{
	DirectX::XMFLOAT3 position;
	float time;
	DirectX::XMFLOAT3 velocity;
	float rotationZ;
	DirectX::XMFLOAT3 rgb;
	float size;
};

struct ParticleMetadata
{
	float life = 0.f;// Time a paritcle is allowed to live, if < 0.0, no calcs
	float maxRange = 10.f; // How long particle is allowed to go 
	float size = 1.f; // Size of particle
	int pattern = 0; // The movement pattern of the particle

	DirectX::XMFLOAT3 spawnPos{ 0.f, 0.f, 0.f };

	float deltaTime = 0;
};

struct ParticleMetadataBuffer
{
	ParticleMetadata metadata[PARTICLE_METADATA_LIMIT];
};

namespace Particles
{
	extern int RenderSlot;

	extern PoolPointer<ParticleInputOutput> m_readBuffer;
	extern PoolPointer<ParticleInputOutput> m_writeBuffer;


	void SwitchInputOutput();
	void InitializeParticles();
	void ReleaseParticles();

	ParticleMetadataBuffer* GetData();

	//Calls for D3D11Helper to set the compute shader and the resources it requires
	void PrepareParticleCompute(RenderSetupComponent renderStates[8]);
	//Calls for D3D11Helper to reset the compute shader and copy the resources of the SRV to vertex buffer
	void FinishParticleCompute(RenderSetupComponent renderStates[8]);
	//Calls for D3D11Helper to set the shaders and resources requiered for the particle pass
	void PrepareParticlePass(RenderSetupComponent renderStates[8]);
	//Calls for D3D11Helper to reset the shaders and resources used by the particle pass
	void FinishParticlePass();
}

