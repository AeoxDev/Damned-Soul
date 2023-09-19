#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"


//NOT MEMBER VARS, CHANGER later
PoolPointer<Particle> m_particles;
PoolPointer<ParticleInputOutput> m_readBuffer;
PoolPointer<ParticleInputOutput> m_writeBuffer;

CS_IDX m_computeShaders[8];
VS_IDX m_vertexShader;
PS_IDX m_pixelShader;
GS_IDX m_geometryShader;
CB_IDX m_metadata, m_worldMatrix;
VB_IDX m_vertexBuffer;
IB_IDX m_indexBuffer;
RS_IDX m_rasterizer;

void SwitchInputOutput()
{
	//Store read
	ParticleInputOutput tempHolder = *m_readBuffer;

	*m_readBuffer = *m_writeBuffer;
	*m_writeBuffer = tempHolder;
}


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
	PoolPointer<uint32_t> index = MemLib::palloc(sizeof(uint32_t) * data->m_end);


	m_particles = MemLib::palloc(sizeof(Particle) * data->m_end);
	m_readBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_writeBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_metadata = CreateConstantBuffer(&(*data), sizeof(m_metadata), 0);

	DirectX::XMMATRIX worldMat = DirectX::XMMatrixIdentity();
	m_worldMatrix = CreateConstantBuffer(&worldMat, sizeof(DirectX::XMMATRIX), 0);

	m_rasterizer = CreateRasterizerState(false, true);

	for (int i = 0; i < data->m_end; i++)
	{
		m_particles[i].m_position = DirectX::XMFLOAT3((float)i, 0.f, 0.f);
		m_particles[i].m_rgb = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		m_particles[i].m_velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

		index[i] = i;
	}

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_readBuffer->SRVIndex = CreateShaderResourceView(&(*m_particles), sizeof(Particle), BIND_COMPUTE, RESOURCE_BUFFER, resourceFlags, (CPU_FLAGS)0, 0);
	m_writeBuffer->SRVIndex = CreateShaderResourceView(&(*m_particles), sizeof(Particle), BIND_COMPUTE, RESOURCE_BUFFER, resourceFlags, (CPU_FLAGS)0, 0);


	m_readBuffer->UAVIndex = CreateUnorderedAcessView(&(*m_particles), sizeof(Particle), m_readBuffer->SRVIndex, 0);
	m_writeBuffer->UAVIndex = CreateUnorderedAcessView(&(*m_particles), sizeof(Particle), m_writeBuffer->SRVIndex, 0);

	m_vertexBuffer = CreateVertexBuffer(&(*m_particles), sizeof(Particle), data->m_end, USAGE_DEFAULT);
	m_indexBuffer = CreateIndexBuffer(&(*index), sizeof(int), data->m_end);

	m_computeShaders[0] = LoadComputeShader("ParticleSmoke.cso");

	m_vertexShader = LoadVertexShader("ParticleVS.cso", PARTICLE);
	m_pixelShader = LoadPixelShader("ParticlePS.cso");
	m_geometryShader = LoadGeometryShader("ParticleGS.cso");
}

void PrepareParticleCompute()
{
	SwitchInputOutput();

	SetComputeShader(m_computeShaders[0]);

	SetConstantBuffer(m_metadata, BIND_COMPUTE, true);
	SetShaderResourceView(m_readBuffer->SRVIndex);
	SetUnorderedAcessView(m_writeBuffer->UAVIndex);
}

void FinishParticleCompute()
{
	UnloadShaderResourceView(m_readBuffer->SRVIndex);
	UnloadUnorderedAcessView(m_writeBuffer->UAVIndex);
	ResetComputeShader();

	CopyToVertexBuffer(m_vertexBuffer, m_writeBuffer->SRVIndex);
}

void PrepareParticlePass()
{
	SetVertexShader(m_vertexShader);
	SetGeometryShader(m_geometryShader);
	SetPixelShader(m_pixelShader);

	SetVertexBuffer(m_vertexBuffer);
	SetIndexBuffer(m_indexBuffer);

	SetRasterizerState(m_rasterizer);

	SetConstantBuffer(m_worldMatrix, BIND_VERTEX);
	//The camera constant buffer is set to the geometry shader outside of this function
}

void FinishParticlePass()
{
	ResetGeometryShader();
}
