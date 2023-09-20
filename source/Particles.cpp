#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"

__declspec(align(16)) struct Particle
{
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_rgb;
};

struct ParticleMetadata
{
	int m_start, m_end;//Which particles in the array to work with
	float m_life;//if < 0.0, no calcs
	float m_maxRange;

	DirectX::XMFLOAT3 m_spawnPos;
};

PoolPointer<Particle> particles;
PoolPointer<ParticleInputOutput> Particles::m_readBuffer;
PoolPointer<ParticleInputOutput> Particles::m_writeBuffer;

int8_t  Particles::m_computeShaders[8];
int8_t  Particles::m_vertexShader;
int8_t  Particles::m_pixelShader;
int8_t  Particles::m_geometryShader;
int16_t Particles::m_metadata;
int16_t Particles::m_vertexBuffer;
int16_t Particles::m_indexBuffer;
int8_t  Particles::m_rasterizer; 

int8_t m_renderTargetView;
int8_t m_depthStencilView;
int8_t m_shaderResourceView;

void Particles::SwitchInputOutput()
{
	//Store read
	ParticleInputOutput tempHolder = *m_readBuffer;

	*m_readBuffer = *m_writeBuffer;
	*m_writeBuffer = tempHolder;
}


void Particles::InitializeParticles()
{
	PoolPointer<ParticleMetadata> data;
	data = MemLib::palloc(sizeof(ParticleMetadata));


	//NOTE TODO: DONT USE HARDCODED VALUES
	data->m_start = 0; data->m_end = 100;
	data->m_life = 5.f;
	data->m_maxRange = 10.f;

	data->m_spawnPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

	float size = sizeof(Particle) * data->m_end;
	PoolPointer<uint32_t> index = MemLib::palloc(sizeof(uint32_t) * data->m_end);


	particles = MemLib::palloc(sizeof(Particle) * data->m_end);
	m_readBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_writeBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_metadata = CreateConstantBuffer(&(*data), sizeof(m_metadata), 0);

	m_rasterizer = CreateRasterizerState(false, true);

	for (int i = 0; i < data->m_end; i++)
	{
		particles[i].m_position = DirectX::XMFLOAT3((float)i, 0.f, 0.f);
		particles[i].m_rgb = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		particles[i].m_velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

		index[i] = i;
	}

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_readBuffer->SRVIndex = CreateShaderResourceView(&(*particles), sizeof(Particle), BIND_COMPUTE, RESOURCE_BUFFER, resourceFlags, (CPU_FLAGS)0, 0);
	m_writeBuffer->SRVIndex = CreateShaderResourceView(&(*particles), sizeof(Particle), BIND_COMPUTE, RESOURCE_BUFFER, resourceFlags, (CPU_FLAGS)0, 0);


	m_readBuffer->UAVIndex = CreateUnorderedAcessView(&(*particles), sizeof(Particle), m_readBuffer->SRVIndex, 0);
	m_writeBuffer->UAVIndex = CreateUnorderedAcessView(&(*particles), sizeof(Particle), m_writeBuffer->SRVIndex, 0);

	m_vertexBuffer = CreateVertexBuffer(&(*particles), sizeof(Particle), data->m_end, USAGE_DEFAULT);
	m_indexBuffer = CreateIndexBuffer(&(*index), sizeof(int), data->m_end);

	m_computeShaders[SMOKE] = LoadComputeShader("ParticleSmoke.cso");

	m_vertexShader = LoadVertexShader("ParticleVS.cso", PARTICLE);
	m_pixelShader = LoadPixelShader("ParticlePS.cso");
	m_geometryShader = LoadGeometryShader("ParticleGS.cso");

	//When done initializing free the temporary paritcle data
	MemLib::pfree(particles);
}

void Particles::PrepareParticleCompute()
{
	SwitchInputOutput();

	SetComputeShader(m_computeShaders[0]);

	SetConstantBuffer(m_metadata, BIND_COMPUTE, true);
	SetShaderResourceView(m_readBuffer->SRVIndex);
	SetUnorderedAcessView(m_writeBuffer->UAVIndex);
}

void Particles::FinishParticleCompute()
{
	UnloadShaderResourceView(m_readBuffer->SRVIndex);
	UnloadUnorderedAcessView(m_writeBuffer->UAVIndex);
	ResetComputeShader();

	CopyToVertexBuffer(m_vertexBuffer, m_writeBuffer->SRVIndex);
}

void Particles::PrepareParticlePass()
{
	SetVertexShader(m_vertexShader);
	SetGeometryShader(m_geometryShader);
	SetPixelShader(m_pixelShader);

	SetVertexBuffer(m_vertexBuffer);
	SetIndexBuffer(m_indexBuffer);

	SetRasterizerState(m_rasterizer);

	//The camera constant buffer is set to the geometry shader outside of this function
}

void Particles::FinishParticlePass()
{
	ResetGeometryShader();
}

int8_t Particles::GetComputeShaderIndex(ComputeShaders name)
{
	int8_t returnValue = -1;

	switch (name)
	{
	case SMOKE:
		returnValue = SMOKE;
		break;
	default:
		returnValue = -1;
		break;
	}
	return returnValue;
}