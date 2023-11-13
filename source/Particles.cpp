#include "Particles.h"
#include "D3D11Helper\D3D11Helper.h"	
#include "D3D11Helper\D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "Systems\Systems.h"
#include "SDLHandler.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "Registry.h"

PoolPointer<ParticleInputOutput> Particles::m_readWriteBuffer;
PoolPointer<ParticleMetadataBuffer> data;

int Particles::RenderSlot;

// Compute shader used to reset particle components
CS_IDX setToZeroCS = -1;

void Particles::SwitchInputOutput()
{
	UAV_IDX tempInput = m_readWriteBuffer->inputUAV;
	UAV_IDX tempOutput = m_readWriteBuffer->outputUAV;

	m_readWriteBuffer->outputUAV = tempInput;
	m_readWriteBuffer->inputUAV = tempOutput;
}

void Particles::InitializeParticles()
{
	data = MemLib::palloc(sizeof(ParticleMetadataBuffer));
	m_readWriteBuffer = MemLib::palloc(sizeof(ParticleInputOutput));

	Particle* particles;
	particles = (Particle*)MemLib::spush(sizeof(Particle) * MAX_PARTICLES);



	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].position = DirectX::XMFLOAT3(99999.f, 9999999.f, 9999999.f);
		particles[i].time = 0.f;
		particles[i].velocity = DirectX::XMFLOAT3(10.f, 10.f, 10.f);
		particles[i].rotationZ = 0.f;
		particles[i].rgb = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
		particles[i].size = 0.f;
	}


	m_readWriteBuffer->inputUAV = CreateUnorderedAccessViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, BIND_UNORDERED_ACCESS, NONE);
	m_readWriteBuffer->outputUAV = CreateUnorderedAccessViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, BIND_UNORDERED_ACCESS, NONE);
	MemLib::spop(); // for particles



	for (int i = 0; i < PARTICLE_METADATA_LIMIT; i++)
	{
		data->metadata[i].life = -1.f;
		data->metadata[i].maxRange = -1.f;
		data->metadata[i].pattern = -1;
		data->metadata[i].size = -1.f;
		data->metadata[i].spawnPos = DirectX::XMFLOAT3(-99999.f, -99999.f, -99999.f);

		if (i < 100)
			data->metadata[i].deltaTime = 0.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f - 0.f))); // random between 0.0 to 1.0
		else
			data->metadata[i].deltaTime = 1.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.f - 1.f))); // random between 1.0 to 3.0

	}

	RenderSlot = SetupParticles();
}

void Particles::ReleaseParticles()
{
	MemLib::pfree(data);
	MemLib::pfree(m_readWriteBuffer);
}

void Particles::UpdateMetadata(int metadataSlot, float x, float y, float z)
{
	data->metadata[metadataSlot].spawnPos.x = x;
	data->metadata[metadataSlot].spawnPos.y = y;
	data->metadata[metadataSlot].spawnPos.z = z;
}

void Particles::UpdateMetadata(int metadataSlot, float v0x, float v0z, float v1x, float v1z, float v2x, float v2z)
{
	data->metadata[metadataSlot].positionInfo.y = v0x; data->metadata[metadataSlot].positionInfo.z = v0z;
	data->metadata[metadataSlot].morePositionInfo.x = v1x; 	data->metadata[metadataSlot].morePositionInfo.y = v1z;
	data->metadata[metadataSlot].morePositionInfo.z = v2x; data->metadata[metadataSlot].morePositionInfo.w = v2z;
}

ParticleMetadataBuffer* Particles::GetData()
{
	return data;
}

//Create an array with base positions
//Array with base rotations

//Transform update: Loop to update metatdata with transform and base pos/rotations.



void Particles::PrepareParticleCompute(RenderSetupComponent renderStates[8])
{
	SwitchInputOutput();

	data->metadata[0].deltaTime = GetDeltaTime();
	//if (data->metadata[pc->metadataSlot].pattern == FLAMETHROWER)
	//{
	//	data->metadata[pc->metadataSlot].life = ;
	//		data->metadata[pc->metadataSlot].spawnPos = ;
	//}


	UpdateConstantBuffer(renderStates[RenderSlot].constantBuffer, data->metadata);

	SetComputeShader(renderStates[RenderSlot].computeShader);
	SetConstantBuffer(renderStates[RenderSlot].constantBuffer, BIND_COMPUTE, 0);
	SetUnorderedAcessView(m_readWriteBuffer->inputUAV, 0);
	SetUnorderedAcessView(m_readWriteBuffer->outputUAV, 1);
}

void Particles::FinishParticleCompute(RenderSetupComponent renderStates[8])
{
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	UnsetConstantBuffer(BIND_COMPUTE, 0);
	UnsetComputeShader();

	CopyToVertexBuffer(renderStates[RenderSlot].vertexBuffer, m_readWriteBuffer->outputUAV);
}

void Particles::PrepareParticlePass(RenderSetupComponent renderStates[8])
{
	SetTopology(POINTLIST);

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

// -- PARTICLE COMPONENT FUNCTION DEFINTIONS -- //
ParticleComponent::ParticleComponent(float seconds, float radius, float size, float x, float y, float z, ComputeShaders pattern)
{
	metadataSlot = FindSlot();

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	if (-1 == setToZeroCS)
		setToZeroCS = LoadComputeShader("ParticleTimeResetCS.cso");

	// Prepare dispatch
	SetComputeShader(setToZeroCS);
	SetUnorderedAcessView(Particles::m_readWriteBuffer->inputUAV, 0);
	SetUnorderedAcessView(Particles::m_readWriteBuffer->outputUAV, 1);

	// Reset the time values of the particles to a glorious zero
	Dispatch(1, metadataSlot + 1, 1); //x * y * z

	// Call the finish function, no need to reinvent the wheel for this one
	Particles::FinishParticleCompute(renderStates);
}

ParticleComponent::ParticleComponent(float seconds, float radius, float size, float x, float y, float z, float rotationY, float v0X, float v0Z, float v1X, float v1Z, float v2X, float v2Z, ComputeShaders pattern)
{
	metadataSlot = FindSlot();

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;
	data->metadata[metadataSlot].rotationY = rotationY;
	data->metadata[metadataSlot].positionInfo.x = 0.0f;
	data->metadata[metadataSlot].positionInfo.y = v0X; data->metadata[metadataSlot].positionInfo.z = v0Z;
	data->metadata[metadataSlot].morePositionInfo.x = v1X; data->metadata[metadataSlot].morePositionInfo.y = v1Z; data->metadata[metadataSlot].morePositionInfo.z = v2X;  data->metadata[metadataSlot].morePositionInfo.w = v2Z;

	data->metadata[metadataSlot].positionInfo.x = -99.f;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);
}

ParticleComponent::~ParticleComponent()
{
	data->metadata[metadataSlot].life = -1.f;
	data->metadata[metadataSlot].maxRange = -1.f;
	data->metadata[metadataSlot].size = -1.f;
	data->metadata[metadataSlot].spawnPos.x = -1.f;	data->metadata[metadataSlot].spawnPos.y = -1.f;	data->metadata[metadataSlot].spawnPos.z = -1.f;
	data->metadata[metadataSlot].pattern = -1;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

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

void ParticleComponent::Release()
{
	data->metadata[metadataSlot].life = -1.f;
	data->metadata[metadataSlot].maxRange = -1.f;
	data->metadata[metadataSlot].size = -1.f;
	data->metadata[metadataSlot].spawnPos.x = 99999.f;	data->metadata[metadataSlot].spawnPos.y = 99999.f;	data->metadata[metadataSlot].spawnPos.z = 99999.f;
	data->metadata[metadataSlot].pattern = -1;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	metadataSlot = -1;
}

void ParticleComponent::RemoveParticles(EntityID& entity)
{
	data->metadata[metadataSlot].life = -1.f;
	data->metadata[metadataSlot].maxRange = -1.f;
	data->metadata[metadataSlot].size = -1.f;
	data->metadata[metadataSlot].spawnPos.x = 99999.f;	data->metadata[metadataSlot].spawnPos.y = 99999.f;	data->metadata[metadataSlot].spawnPos.z = 99999.f;
	data->metadata[metadataSlot].pattern = -1;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	metadataSlot = -1;

	registry.RemoveComponent<ParticleComponent>(entity);

}
