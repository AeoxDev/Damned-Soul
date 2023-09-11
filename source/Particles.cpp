#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"

PoolPointer<Particle> m_particles;

int8_t m_computeShaders[8]; 
int8_t m_ACBuffer[2];		

void InitializeParticles()
{
	//NOTE TODO: DONT USE HARDCODED VALUES
	m_particles = MemLib::palloc(sizeof(Particle)*100);

	m_ACBuffer[0] = CreateUnorderedAcessView(NULL, sizeof(Particle), RESOURCE_BUFFER, (FLAGS)0, 0);
	m_ACBuffer[1] = CreateUnorderedAcessView(NULL, sizeof(Particle), RESOURCE_BUFFER, (FLAGS)0, 1);


	//m_ACBuffer[0] = MAKE AN SRV
	// constantDataDesc.Usage                  = D3D11_USAGE_DYNAMIC;
	//constantDataDesc.ByteWidth = sizeof(ParticleConstantData) * NUM_PARTICLES;
	//constantDataDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//constantDataDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//constantDataDesc.StructureByteStride = sizeof(ParticleConstantData);
	//constantDataDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	// 
	// srvDesc.Format                  = DXGI_FORMAT_UNKNOWN;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	//srvDesc.BufferEx.FirstElement = 0;
	//srvDesc.BufferEx.Flags = 0;
	//srvDesc.BufferEx.NumElements = NUM_PARTICLES;
	// 
	//m_ACBuffer[1] = MAKE AN UAV
	//outputDesc.Usage = D3D11_USAGE_DEFAULT;
	//outputDesc.ByteWidth = sizeof(ParticleData) * NUM_PARTICLES;
	//outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	//outputDesc.CPUAccessFlags = 0;
	//outputDesc.StructureByteStride = sizeof(ParticleData);
	//outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	//outputDesc.Usage = D3D11_USAGE_STAGING;
	//outputDesc.BindFlags = 0;
	//outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	//m_ACBuffer[0] = CreateUnorderedAcessView(NULL, sizeof(Particle), RESOURCE_BUFFER, 0);
	//m_ACBuffer[1] = CreateUnorderedAcessView(NULL, sizeof(Particle), RESOURCE_BUFFER, 0);
}
