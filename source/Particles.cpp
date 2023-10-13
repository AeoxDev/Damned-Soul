#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "Systems\Systems.h"
#include "SDLHandler.h"
#include "Camera.h"
#include "DeltaTime.h"

PoolPointer<ParticleInputOutput> Particles::m_readBuffer;
PoolPointer<ParticleInputOutput> Particles::m_writeBuffer;
PoolPointer<ParticleMetadataBuffer> data;

int Particles::RenderSlot;

void Particles::SwitchInputOutput()
{
	////Store read
	//ParticleInputOutput tempHolder = *m_readBuffer;

	//std::memcpy(m_readBuffer, m_writeBuffer, sizeof(ParticleInputOutput));
	//*m_writeBuffer = tempHolder;

	SRV_IDX tempSrv = m_readBuffer->SRVIndex;
	UAV_IDX tempUav = m_readBuffer->UAVIndex;

	m_readBuffer->SRVIndex = m_writeBuffer->SRVIndex;
	m_readBuffer->UAVIndex = m_writeBuffer->UAVIndex;

	m_writeBuffer->SRVIndex = tempSrv;
	m_writeBuffer->UAVIndex = tempUav;
}

void Particles::InitializeParticles()
{
	data = MemLib::palloc(sizeof(ParticleMetadataBuffer));
	m_readBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_writeBuffer = MemLib::palloc(sizeof(ParticleInputOutput));

	Particle* particles;
	particles = (Particle*)MemLib::spush(sizeof(Particle) * MAX_PARTICLES);



	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].position = DirectX::XMFLOAT3((float)i, 0.f, 0.f);
		particles[i].time = 0.f;
		particles[i].rgb = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		particles[i].rotationZ = 0.f;
		particles[i].velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		particles[i].size = 0.f;
	}

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	// THEESE ARE THE BOOVAR OF MEMORY LEAKS, Mvh Arian (tror jag)
	m_readBuffer->SRVIndex =  CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, (CPU_FLAGS)0);
	m_writeBuffer->SRVIndex = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, (CPU_FLAGS)0);

	// These are 48 * 65536 = 3145728 large, one of the numbers of the error
	m_readBuffer->UAVIndex =  CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_readBuffer->SRVIndex);
	m_writeBuffer->UAVIndex = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_writeBuffer->SRVIndex);

	MemLib::spop(); // for particles



	for (int i = 0; i < PARTICLE_METADATA_LIMIT; i++)
	{
		data->metadata[i].life = -1.f;
		data->metadata[i].maxRange = -1.f;
		data->metadata[i].pattern = -1;
		data->metadata[i].size = -1.f;
		data->metadata[i].spawnPos = DirectX::XMFLOAT3(-999.f, -999.f, -999.f);

		data->metadata[i].deltaTime = 0;
	}

	RenderSlot = SetupParticles();
}

void Particles::ReleaseParticles()
{
	MemLib::pfree(data);
	MemLib::pfree(m_readBuffer);
	MemLib::pfree(m_writeBuffer);
}

ParticleMetadataBuffer* Particles::GetData()
{
	return data;
}

void Particles::PrepareParticleCompute(RenderSetupComponent renderStates[8])
{
	SwitchInputOutput();

	SetComputeShader(renderStates[RenderSlot].computeShader);
	SetConstantBuffer(renderStates[RenderSlot].constantBuffer, BIND_COMPUTE, 0);
	SetShaderResourceView(m_readBuffer->SRVIndex, BIND_COMPUTE, 0);
	SetUnorderedAcessView(m_writeBuffer->UAVIndex, 0);
}

void Particles::FinishParticleCompute(RenderSetupComponent renderStates[8])
{
	UnsetShaderResourceView(BIND_COMPUTE, 0);
	UnsetUnorderedAcessView(0);
	UnsetConstantBuffer(BIND_COMPUTE, 0);
	UnsetComputeShader();


	for (int i = 0; i < PARTICLE_METADATA_LIMIT; i++)
		data->metadata[i].deltaTime = GetDeltaTime();

	UpdateConstantBuffer(renderStates[RenderSlot].constantBuffer, data->metadata);

 	CopyToVertexBuffer(renderStates[RenderSlot].vertexBuffer, m_writeBuffer->SRVIndex);
}

void Particles::PrepareParticlePass(RenderSetupComponent renderStates[8])
{
	SetTopology(POINTLIST);

	SetWorldMatrix(0.f, 0.f, 0.f, BIND_VERTEX, 0);
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_GEOMETRY, 1);

	SetVertexShader(renderStates[RenderSlot].vertexShaders[0]);
	SetGeometryShader(renderStates[RenderSlot].geometryShader);
	SetPixelShader(renderStates[RenderSlot].pixelShaders[0]);

	SetVertexBuffer(renderStates[RenderSlot].vertexBuffer);
	SetRasterizerState(renderStates[RenderSlot].rasterizerState);

}

void Particles::FinishParticlePass()
{
	SetTopology(TRIANGLELIST);

	UnsetConstantBuffer(BIND_GEOMETRY, 1);

	UnsetVertexShader();
	UnsetGeometryShader();
	UnsetPixelShader();

	//UnsetVertexBuffer();
	UnsetRasterizerState();
}

// -- ECS FUNCTION DEFINTIONS -- //
ParticleComponent::ParticleComponent(RenderSetupComponent constantBuffer[8], int RenderSlot, float seconds, float radius, float size, float x, float y, float z, ComputeShaders pattern)
{
	metadataSlot = FindSlot();

	//data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;

	UpdateConstantBuffer(constantBuffer[RenderSlot].constantBuffer, data->metadata);
}

ParticleComponent::~ParticleComponent()
{
	data->metadata[metadataSlot].life = -1.f;
	metadataSlot = -1;
}

int ParticleComponent::FindSlot()
{
	for (int i = 0; i < PARTICLE_METADATA_LIMIT; i++)
	{
		if (data->metadata[i].life <= 0.0f)
		{
			metadataSlot = i;
			break;
		}
	}
	return metadataSlot;
}

void ParticleComponent::Setup(RenderSetupComponent constantBuffer[8], int RenderSlot, float seconds, float radius, float size, float x, float y, float z, ComputeShaders pattern)
{
	metadataSlot = FindSlot();

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;

	UpdateConstantBuffer(constantBuffer[RenderSlot].constantBuffer, data->metadata);
}