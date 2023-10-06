#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "Systems\Systems.h"
#include "SDLHandler.h"


Particle* particles;
ParticleInputOutput* Particles::m_readBuffer;
ParticleInputOutput* Particles::m_writeBuffer;
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
	////Store read
	//ParticleInputOutput tempHolder = *m_readBuffer;

	//*m_readBuffer = *m_writeBuffer;
	//*m_writeBuffer = tempHolder;
}

void Particles::InitializeParticles()
{
	data = (ParticleMetadataBuffer*)MemLib::spush(sizeof(ParticleMetadataBuffer));
	m_readBuffer = (ParticleInputOutput*)MemLib::spush(sizeof(ParticleInputOutput));
	m_writeBuffer = (ParticleInputOutput*)MemLib::spush(sizeof(ParticleInputOutput));
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
	m_readBuffer->SRVIndex = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, (CPU_FLAGS)0);
	m_writeBuffer->SRVIndex = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES,resourceFlags, (CPU_FLAGS)0);

	m_readBuffer->UAVIndex = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_readBuffer->SRVIndex);
	m_writeBuffer->UAVIndex = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_writeBuffer->SRVIndex);

	m_computeShaders = LoadComputeShader("ParticleCS.cso");


	for (int i = 0; i < PARTICLE_METADATA_LIMIT; i++)
	{
		data->metadata[i].life = -1.f;
		data->metadata[i].maxRange = -1.f;
		data->metadata[i].pattern = -1; //-1.f;
		data->metadata[i].size = -1.f;
		data->metadata[i].spawnPos = DirectX::XMFLOAT3(-999.f, -999.f, -999.f);

		data->metadata[i].padding = 0;
	}

	RenderSlot = SetupParticles();

}

void Particles::ReleaseParticles()
{
	// Absolutely not! You either pop in the same scope, during the same frame, or never at all
	// It is entirely possible, and extremely likely that this will pop other completely unrelated that was added to the stack afterwards

	//MemLib::spop(); // for particles
	//MemLib::spop(); // read buffer
	//MemLib::spop(); // write buffer
	//MemLib::spop(); // For data
}

ParticleMetadataBuffer* Particles::GetData()
{
	return data;
}

void Particles::PrepareParticleCompute(RenderSetupComponent renderStates[8])
{
	SwitchInputOutput();

	SetComputeShader(m_computeShaders);
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

 	CopyToVertexBuffer(renderStates[RenderSlot].vertexBuffer, m_writeBuffer->SRVIndex);
}

void Particles::PrepareParticlePass(RenderSetupComponent renderStates[8])
{
	SetTopology(POINTLIST);

	SetWorldMatrix(1.f, 1.f, 1.f, BIND_VERTEX, 0);

	SetVertexShader(renderStates[RenderSlot].vertexShaders[0]);
	SetGeometryShader(renderStates[RenderSlot].geometryShader);
	SetPixelShader(renderStates[RenderSlot].pixelShaders[0]);

	SetVertexBuffer(renderStates[RenderSlot].vertexBuffer);
	SetRasterizerState(renderStates[RenderSlot].rasterizerState);

	//The camera constant buffer is set to the geometry shader outside of this function
}

void Particles::FinishParticlePass()
{
	SetTopology(TRIANGLELIST);

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

	data->metadata[metadataSlot].life = seconds;
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
