#pragma once
#include <DirectXMath.h>
#include <vector>
#include "MemLib/PoolPointer.hpp"
#include "ParticleComponent.h"
#include "GameRenderer.h"
#include "D3D11Helper\IDX_Types.h"

#define PARTICLE_METADATA_LIMIT 256
#define THREADS_PER_GROUP 32
#define MAX_PARTICLES PARTICLE_METADATA_LIMIT*256

//The UAV is needed to write to the buffer, the SRV is needed to read while simoutanisly allowing for caching
//Slot [0] will always be read and slot [1] will always be write
struct ParticleInputOutput
{
	SRV_IDX SRV;
	UAV_IDX UAV;
};

struct Particle
{
	DirectX::XMFLOAT3 position;
	float time;
	DirectX::XMFLOAT3 velocity;
	float rotationZ;
	DirectX::XMFLOAT3 rgb;
	float size;
	int patterns;
	int VFX;
};

//struct ExtraData
//{
//	int start;
//};

struct ParticleMetadata
{
	int start = -1; 
	int end = -1;
	float life = 0.f;// Time a paritcle is allowed to live, if < 0.0, no calcs
	float maxRange = 10.f; // How long particle is allowed to go, FLAMETHOWER pattern does not use this so it stores X value of triangles vertex
	DirectX::XMFLOAT3 spawnPos{ 0.f, 0.f, 0.f };
	float size = 1.f; // Size of particle
	DirectX::XMFLOAT3 positionInfo{ 0.f, 0.f, 0.f }; // Stores XZ position of flamethrower movements value of triangles vertex
	int pattern = 0; // The movement pattern of the particle

	float deltaTime = 0;
	DirectX::XMFLOAT2 morePositionInfo{ 0.f, 0.f}; // Stores XZ position of flamethrower movements value of triangles vertex
	bool reset = false;
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
	extern std::vector<int> m_unoccupiedParticles;

	void SwitchInputOutput();
	void InitializeParticles();
	void ReleaseParticles();

	void UpdateMetadata(int metadataSlot, float x, float y, float z);
	// Overload for flamethrower triangles
	void UpdateMetadata(int metadataSlot, float v0x, float v0z, float v1x, float v1z, float v2x, float v2z);
	ParticleMetadataBuffer* GetData();
	ParticleMetadata GetMetadataAtIndex(int metadataSlot);

	//Calls for D3D11Helper to set the compute shader and the resources it requires
	void PrepareParticleCompute();
	//Calls for D3D11Helper to reset the compute shader and copy the resources of the SRV to vertex buffer
	void FinishParticleCompute();
	//Calls for D3D11Helper to set the shaders and resources requiered for the particle pass
	void PrepareParticlePass(int metadataSlot);
	//Calls for D3D11Helper to reset the shaders and resources used by the particle pass
	void FinishParticlePass();
	//Calls for D3D11Helper to set the shaders and resources requiered for the VFX pass
	void PrepareVFXPass(RenderSetupComponent renderStates[8]);
	//Calls for D3D11Helper to reset the shaders and resources used by the VFX pass
	void FinishVFXPass();

	void PrepareMeshPass(int metadataSlot);
	void FinishMeshPass();

	void UpdateStart(int& metadataSlot);
	void UpdateVFXStart(int& metadataSlot);
}

