#pragma once
#include <DirectXMath.h>
#include "MemLib/PoolPointer.hpp"
#include "MemLib\ML_Vector.hpp"
#include "ParticleComponent.h"
#include "GameRenderer.h"
#include "D3D11Helper\IDX_Types.h"

//struct LogicalBuffer final {
//	uint8_t* data = nullptr;
//	size_t dataSize = 0;
//	size_t dataFormatStride = 0;
//	size_t physicalAddress = 0;
//	uint32_t slot = -1;
//};
//
//struct PhysicalBuffer final {
//	//CB_IDX* physicalBuffer = nullptr; ITS IN SRV IDX
//	SRV_IDX* physicalBufferView = nullptr;
//	size_t physicalDataSize = 0;
//	bool isDirty = false;
//
//	typedef ML_Vector<LogicalBuffer*> PageArray;
//	PageArray allPages;
//	PhysicalBuffer() = default;
//	inline ~PhysicalBuffer()
//	{
//		/*if (physicalBuffer != nullptr)
//			physicalBuffer->Release();
//		if (physicalBufferView != nullptr)
//			physicalBufferView->Release();*/
//	}
//	void Allocate(LogicalBuffer* logicalBuffer);
//	void Release(LogicalBuffer* logicalBuffer);
//	void RebuildPages(); // very expensive operation
//};
//
//void PhysicalBuffer::Allocate(LogicalBuffer* logicalBuffer)
//{ 
//	allPages.push_back(logicalBuffer); 
//	isDirty = true; 
//} 
//void PhysicalBuffer::Release(LogicalBuffer* logicalBuffer)
//{ 
//	allPages.erase(logicalBuffer->slot); 
//	isDirty = true;
//}
//


#define PARTICLE_METADATA_LIMIT 256
#define THREADS_PER_GROUP 256
#define MAX_PARTICLES PARTICLE_METADATA_LIMIT*THREADS_PER_GROUP

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
};

struct ParticleMetadata
{
	int start = -1; 
	int end = -1;
	float life = 0.f;// Time a paritcle is allowed to live, if < 0.0, no calcs
	float maxRange = 10.f; // How long particle is allowed to go 
	DirectX::XMFLOAT3 spawnPos{ 0.f, 0.f, 0.f };
	float size = 1.f; // Size of particle
	DirectX::XMFLOAT3 positionInfo{ 0.f, 0.f, 0.f };
	int pattern = 0; // The movement pattern of the particle



	float deltaTime = 0;
	float rotationY = 0;
	DirectX::XMFLOAT2 morePositionInfo{ 0.f, 0.f};

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

	void UpdateMetadata(int metadataSlot, float x, float y, float z);
	// Overload for flamethrower triangles
	void UpdateMetadata(int metadataSlot, float v0x, float v0z, float v1x, float v1z, float v2x, float v2z);
	ParticleMetadataBuffer* GetData();
	ParticleMetadata GetMetadataAtIndex(int metadataSlot);

	//Calls for D3D11Helper to set the compute shader and the resources it requires
	void PrepareParticleCompute(RenderSetupComponent renderStates[8]);
	//Calls for D3D11Helper to reset the compute shader and copy the resources of the SRV to vertex buffer
	void FinishParticleCompute(RenderSetupComponent renderStates[8]);
	//Calls for D3D11Helper to set the shaders and resources requiered for the particle pass
	void PrepareParticlePass(RenderSetupComponent renderStates[8]);
	//Calls for D3D11Helper to reset the shaders and resources used by the particle pass
	void FinishParticlePass();
}

