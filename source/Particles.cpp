#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"


//NOT MEMBER VARS, CHANGER later
PoolPointer<Particle> m_particles;
PoolPointer<ParticleInputOutput> m_readBuffer;
PoolPointer<ParticleInputOutput> m_writeBuffer;

int8_t m_computeShaders[8];
int8_t m_vertexShader, m_pixelShader, m_geometryShader;
int16_t m_metadata;
int16_t m_vertexBuffer;

void InitializeParticles()
{
	PoolPointer<ParticleMetadata> data;
	data = MemLib::palloc(sizeof(ParticleMetadata));


	//NOTE TODO: DONT USE HARDCODED VALUES
	data->m_start = 0; data->m_end = 100;
	data->m_life = 5.f;
	data->m_maxRange = 10.f;

	data->m_spawnPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

	float size = sizeof(Particle) * data->m_end;


	m_particles = MemLib::palloc(sizeof(Particle) * data->m_end);
	m_readBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_writeBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_metadata = CreateConstantBuffer(&(*data), sizeof(m_metadata), BIND_COMPUTE, 0);

	for (int i = 0; i < data->m_end; i++)
	{
		m_particles[i].m_position = DirectX::XMFLOAT3((float)i, 0.f, 0.f);
		m_particles[i].m_rgb = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		m_particles[i].m_velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	}

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_readBuffer->SRVIndex = CreateShaderResourceView(&(*m_particles), sizeof(Particle), BIND_COMPUTE, RESOURCE_BUFFER, resourceFlags, (CPU_FLAGS)0, 0);
	m_writeBuffer->SRVIndex = CreateShaderResourceView(&(*m_particles), sizeof(Particle), BIND_COMPUTE, RESOURCE_BUFFER, resourceFlags, (CPU_FLAGS)0, 0);


	m_readBuffer->UAVIndex = CreateUnorderedAcessView(&(*m_particles), sizeof(Particle), m_readBuffer->SRVIndex, 0);
	m_writeBuffer->UAVIndex = CreateUnorderedAcessView(&(*m_particles), sizeof(Particle), m_writeBuffer->SRVIndex, 0);

	m_vertexBuffer = CreateVertexBuffer(&(*m_particles), sizeof(Particle), data->m_end, USAGE_DEFAULT);

	m_computeShaders[0] = LoadComputeShader("ParticleSmoke.cso");

	m_vertexShader = LoadVertexShader("ParticleVertex.cso");
	m_pixelShader = LoadPixelShader("ParticlePixel.cso");
	m_geometryShader = LoadGeometryShader("ParticleGeometry.cso");
}

void SetupParticles()
{
	SetComputeShader(m_computeShaders[0]);
	SetConstantBuffer(m_metadata, true);
}

void SwitchInputOutput()
{
	//Store read
	ParticleInputOutput tempHolder = *m_readBuffer;

	*m_readBuffer = *m_writeBuffer;
	*m_writeBuffer = tempHolder;
}

void PrepareParticles()
{
	SwitchInputOutput();


	SetShaderResourceView(m_readBuffer->SRVIndex);
	SetUnorderedAcessView(m_writeBuffer->UAVIndex);
}

void FinishParticles()
{
	UnloadShaderResourceView(m_readBuffer->SRVIndex);
	UnloadUnorderedAcessView(m_writeBuffer->UAVIndex);

	CopyToVertexBuffer(m_vertexBuffer, m_writeBuffer->SRVIndex);
}
