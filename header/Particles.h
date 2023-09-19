#pragma once
#include <DirectXMath.h>
#include "MemLib/PoolPointer.hpp"

//The UAV is needed to write to the buffer, the SRV is needed to read while simoutanisly allowing for caching
//Slot [0] will always be read and slot [1] will always be write
struct ParticleInputOutput
{
	int8_t SRVIndex;
	int8_t UAVIndex;
};

enum ComputeShaders
{
	SMOKE = 0
};

namespace Particles
{
	extern PoolPointer<ParticleInputOutput> m_readBuffer;
	extern PoolPointer<ParticleInputOutput> m_writeBuffer;

	extern int8_t m_computeShaders[8];
	extern int8_t m_vertexShader;
	extern int8_t m_pixelShader;
	extern int8_t m_geometryShader;
	extern int16_t m_metadata;
	extern int16_t m_vertexBuffer;
	extern int16_t m_indexBuffer;
	extern int8_t m_rasterizer;


	void SwitchInputOutput();
	void InitializeParticles();

	//Calls for D3D11Helper to set the compute shader and the resources it requires
	void PrepareParticleCompute();
	//Calls for D3D11Helper to reset the compute shader and copy the resources of the SRV to vertex buffer
	void FinishParticleCompute();
	//Calls for D3D11Helper to set the shaders and resources requiered for the particle pass
	void PrepareParticlePass();
	//Calls for D3D11Helper to reset the shaders and resources used by the particle pass
	void FinishParticlePass();

	//Returns index of inputted name
	int8_t GetComputeShaderIndex(ComputeShaders name);
}