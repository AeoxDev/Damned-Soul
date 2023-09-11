#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"

PoolPointer<Particle> m_particles;

int8_t m_computeShaders[8]; 
int8_t m_RWBuffer[2];	

int16_t m_metadata;


void InitializeParticles()
{
	PoolPointer<ParticleMetadata> data;


	data = MemLib::palloc(sizeof(ParticleMetadata));


	//NOTE TODO: DONT USE HARDCODED VALUES
	data->m_start = 0; data->m_end = 100;
	data->m_maxRange = 10.f;
	data->m_life = 5.f;

	data->m_spawnPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);


	m_particles = MemLib::palloc(sizeof(Particle) * data->m_end);
	m_metadata = CreateConstantBuffer(&(data->m_start), sizeof(ParticleMetadata), BIND_COMPUTE, 0);


	m_RWBuffer[0] = CreateShaderResourceView(NULL, sizeof(Particle), BIND_COMPUTE, RESOURCE_BUFFER, BIND_SHADER_RESOURCE, (FLAGS)0, 0);
	m_RWBuffer[1] = CreateUnorderedAcessView(NULL, sizeof(Particle), RESOURCE_BUFFER, BIND_UNORDERED_ACCESS, (FLAGS)0, 0);

	m_computeShaders[0] = LoadComputeShader("ParticleSmoke.cso");

	SetComputeShader(m_computeShaders[0]);

	SetConstantBuffer(m_metadata);

	SetShaderResourceView(m_RWBuffer[0]);
	SetUnorderedAcessView(m_RWBuffer[1]);

}

int8_t GetFirstParticle()
{
	return m_RWBuffer[0];
}

int8_t GetSecondParticle()
{
	return m_RWBuffer[1];
}
