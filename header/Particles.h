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

//The UAV is needed to write to the buffer, the SRV is needed to read while simoutanisly allowing for caching
//Slot [0] will always be read and slot [1] will always be write
struct ParticleInputOutput
{
	int8_t SRVIndex;
	int8_t UAVIndex;
};

void InitializeParticles();

//Calls for D3D11Helper to set the compute shader and the resources it requires
void PrepareParticleCompute();
//Calls for D3D11Helper to reset the compute shader and copy the resources of the SRV to vertex buffer
void FinishParticleCompute();
//Calls for D3D11Helper to set the shaders and resources requiered for the particle pass
void PrepareParticlePass();
//Calls for D3D11Helper to reset the shaders and resources used by the particle pass
void FinishParticlePass();



