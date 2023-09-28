#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "SDLHandler.h"
#include "GameRenderer.h"
#include "ParticleComponent.h"


Particle* particles;
ParticleInputOutput* Particles::m_readBuffer;
ParticleInputOutput* Particles::m_writeBuffer;
ParticleMetadata* particleCompData;
ParticleMetadataBuffer* data;


int8_t  Particles::m_computeShaders;
int8_t  Particles::m_vertexShader;
int8_t  Particles::m_pixelShader;
int8_t  Particles::m_geometryShader;
int16_t Particles::m_metadata;
int16_t Particles::m_vertexBuffer;
int16_t Particles::m_indexBuffer;
int8_t  Particles::m_rasterizer; 

int8_t Particles::m_renderTargetView;
int8_t Particles::m_depthStencilView;
int8_t Particles::m_shaderResourceView;

int Particles::RenderSlot;

void Particles::SwitchInputOutput()
{
	//Store read
	ParticleInputOutput tempHolder = *m_readBuffer;

	*m_readBuffer = *m_writeBuffer;
	*m_writeBuffer = tempHolder;
}

void Particles::InitializeParticles()
{
	uint32_t* index;

	particleCompData = (ParticleMetadata*)MemLib::spush(sizeof(ParticleMetadata));
	data = (ParticleMetadataBuffer*)MemLib::spush(sizeof(ParticleMetadataBuffer));

	float size = sizeof(Particle) * MAX_PARTICLES;

	m_readBuffer = (ParticleInputOutput*)MemLib::spush(sizeof(ParticleInputOutput));
	m_writeBuffer = (ParticleInputOutput*)MemLib::spush(sizeof(ParticleInputOutput));
	particles = (Particle*)MemLib::spush(sizeof(Particle) * MAX_PARTICLES);
	index = (uint32_t*)MemLib::spush(sizeof(uint32_t) * MAX_PARTICLES);


	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].position = DirectX::XMFLOAT3((float)i, 0.f, 0.f);
		particles[i].time = 0.f;
		particles[i].rgb = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		particles[i].rotationZ = 0.f;
		particles[i].velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		particles[i].size = 0.f;

		index[i] = i;
	}

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_readBuffer->SRVIndex = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, (CPU_FLAGS)0);
	m_writeBuffer->SRVIndex = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES,resourceFlags, (CPU_FLAGS)0);

	m_readBuffer->UAVIndex = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_readBuffer->SRVIndex);
	m_writeBuffer->UAVIndex = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_writeBuffer->SRVIndex);

	m_computeShaders = LoadComputeShader("ParticleSmoke.cso");

	RenderSlot = SetupParticles(index);


	//When done initializing free the temporary particle data
	MemLib::spop(); // for particles
	MemLib::spop(); // for index

}

void Particles::ReleaseParticles()
{
	MemLib::spop(); // read buffer
	MemLib::spop(); // write buffer
	MemLib::spop(); // For particle metadata
	MemLib::spop(); // For data
}

void Particles::PrepareParticleCompute(RenderSetupComponent renderStates[8])
{
	SwitchInputOutput();

	SetComputeShader(renderStates[RenderSlot].computeShader);
	SetConstantBuffer(renderStates[RenderSlot].constantBuffer, BIND_COMPUTE, true);
	SetShaderResourceView(renderStates[RenderSlot].shaderResourceView, BIND_COMPUTE, 0);
	SetUnorderedAcessView(renderStates[RenderSlot].unorderedAccessView, 0);
}

void Particles::FinishParticleCompute(RenderSetupComponent renderStates[8])
{
	UnloadShaderResourceView(BIND_COMPUTE, 0);
	UnloadUnorderedAcessView(0);
	ResetComputeShader();

	CopyToVertexBuffer(renderStates[RenderSlot].vertexBuffer, m_writeBuffer->SRVIndex);
}

void Particles::PrepareParticlePass(RenderSetupComponent renderStates[8])
{
	SetTopology(POINTLIST);

	SetVertexShader(renderStates[RenderSlot].vertexShader);
	SetGeometryShader(renderStates[RenderSlot].geometryShader);
	SetPixelShader(renderStates[RenderSlot].pixelShader);

	SetVertexBuffer(renderStates[RenderSlot].vertexBuffer);
	//SetIndexBuffer(m_indexBuffer);

	SetRasterizerState(renderStates[RenderSlot].rasterizerState);

	//The camera constant buffer is set to the geometry shader outside of this function
}

void Particles::FinishParticlePass()
{
	ResetGeometryShader();
}

void Particles::PrepareSmokeParticles(Registry& registry, EntityID& entityID, RenderSetupComponent constantBuffer[8], float seconds, float radius, float size, DirectX::XMFLOAT3 entityPosition)
{
	registry.AddComponent<ParticleComponent>(entityID);
	ParticleComponent* pc = registry.GetComponent<ParticleComponent>(entityID);
	pc->metadataSlot = pc->Setup();

	pc->SetLife(seconds);
	pc->SetMaxRange(radius);
	pc->SetSize(size);
	pc->SetPosition(entityPosition.x, entityPosition.y, entityPosition.z);
	pc->SetPattern(SMOKE);

	particleCompData->life = seconds;
	particleCompData->maxRange = radius;
	particleCompData->size = size;
	particleCompData->spawnPos = entityPosition;
	particleCompData->pattern = SMOKE;

	UpdateConstantBuffer(constantBuffer[RenderSlot].constantBuffer, particleCompData);
}

int ParticleComponent::Setup()
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

void ParticleComponent::SetLife(float seconds)
{
	assert(metadataSlot > -1 || metadataSlot < PARTICLE_METADATA_LIMIT);

	data->metadata[metadataSlot].life = seconds;
}

void ParticleComponent::SetMaxRange(float radius)
{
	assert(metadataSlot > -1 || metadataSlot < PARTICLE_METADATA_LIMIT);

	data->metadata[metadataSlot].maxRange = radius;
}

void ParticleComponent::SetSize(float size)
{
	assert(metadataSlot > -1 || metadataSlot < PARTICLE_METADATA_LIMIT);

	data->metadata[metadataSlot].size = size;
}

void ParticleComponent::SetPattern(ComputeShaders pattern)
{
	assert(metadataSlot > -1 || metadataSlot < PARTICLE_METADATA_LIMIT);

	data->metadata[metadataSlot].pattern = pattern;
}

void ParticleComponent::SetPosition(float x, float y, float z)
{
	assert(metadataSlot > -1 || metadataSlot < PARTICLE_METADATA_LIMIT);

	data->metadata[metadataSlot].spawnPos.x = x;
	data->metadata[metadataSlot].spawnPos.y = y;
	data->metadata[metadataSlot].spawnPos.z = z;
}
