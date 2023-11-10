#include "Particles.h"
#include "D3D11Helper\D3D11Helper.h"	
#include "D3D11Helper\D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "Systems\Systems.h"
#include "SDLHandler.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "Registry.h"

PoolPointer<ParticleInputOutput> Particles::m_readBuffer;
PoolPointer<ParticleInputOutput> Particles::m_writeBuffer;
std::vector<int> Particles::m_unoccupiedParticles;
int start;


PoolPointer<ParticleMetadataBuffer> data;
CB_IDX startKeeper;

int Particles::RenderSlot;

// Compute shader used to reset particle components
CS_IDX setToZeroCS = -1;

void Particles::SwitchInputOutput()
{
	SRV_IDX readSRV = m_readBuffer->SRV;
	UAV_IDX readUAV = m_readBuffer->UAV;

	m_readBuffer->SRV = m_writeBuffer->SRV;
	m_readBuffer->UAV = m_writeBuffer->UAV;

	m_writeBuffer->SRV = readSRV;
	m_writeBuffer->UAV = readUAV;
}

void Particles::InitializeParticles()
{
	data = MemLib::palloc(sizeof(ParticleMetadataBuffer));
	m_readBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_writeBuffer = MemLib::palloc(sizeof(ParticleInputOutput));

	m_unoccupiedParticles = std::vector<int>(MAX_PARTICLES, -1);  //.reserve(sizeof(int) * MAX_PARTICLES);
	//std::fill(m_unoccupiedParticles.begin(), m_unoccupiedParticles.end(), -1);

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

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_readBuffer->SRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, NONE);
	m_writeBuffer->SRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, NONE);

	m_readBuffer->UAV = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_readBuffer->SRV);
	m_writeBuffer->UAV = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_writeBuffer->SRV);

	MemLib::spop(); // for particles

	startKeeper = CreateConstantBuffer(sizeof(int));

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


		data->metadata[i].morePositionInfo = DirectX::XMFLOAT2(-99999.f, -99999.f);
		data->metadata[i].positionInfo = DirectX::XMFLOAT3(-99999.f, -99999.f, -99999.f);
		data->metadata[i].rotationY = 0.0f;




	}

	RenderSlot = SetupParticles();
}

void Particles::ReleaseParticles()
{
	MemLib::pfree(data);
	MemLib::pfree(m_readBuffer);
	MemLib::pfree(m_writeBuffer);
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
	//data->metadata[metadataSlot].morePositionInfo.z = v2x; data->metadata[metadataSlot].morePositionInfo.w = v2z;
}

ParticleMetadataBuffer* Particles::GetData()
{
	return data;
}

ParticleMetadata Particles::GetMetadataAtIndex(int metadataSlot)
{
	return data->metadata[metadataSlot];
}

void Particles::PrepareParticleCompute(RenderSetupComponent renderStates[8])
{
	SwitchInputOutput();

	data->metadata[0].deltaTime = GetDeltaTime();

	UpdateConstantBuffer(renderStates[RenderSlot].constantBuffer, data->metadata);

	SetComputeShader(renderStates[RenderSlot].computeShader);
	SetConstantBuffer(renderStates[RenderSlot].constantBuffer, BIND_COMPUTE, 0);
	SetShaderResourceView(m_readBuffer->SRV, BIND_COMPUTE, 0);
	SetUnorderedAcessView(m_writeBuffer->UAV, 1);
}

void Particles::FinishParticleCompute(RenderSetupComponent renderStates[8])
{
	UnsetShaderResourceView(BIND_COMPUTE, 0);
	UnsetUnorderedAcessView(1);
	UnsetConstantBuffer(BIND_COMPUTE, 0);
	UnsetComputeShader();

	//CopyToVertexBuffer(renderStates[RenderSlot].vertexBuffer, m_readWriteBuffer->outputUAV);
}

void Particles::PrepareParticlePass(RenderSetupComponent renderStates[8])
{

	SetTopology(POINTLIST);

	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_GEOMETRY, 1);

	SetVertexShader(renderStates[RenderSlot].vertexShaders[0], true);
	SetGeometryShader(renderStates[RenderSlot].geometryShader);
	SetPixelShader(renderStates[RenderSlot].pixelShaders[0]);

	SetShaderResourceView(m_writeBuffer->SRV, BIND_VERTEX, 2);
	UnsetVertexBuffer();
	UnsetIndexBuffer();

	SetRasterizerState(renderStates[RenderSlot].rasterizerState);

}

void Particles::FinishParticlePass()
{
	SetTopology(TRIANGLELIST);

	UnsetConstantBuffer(BIND_GEOMETRY, 1);
	UnsetConstantBuffer(BIND_VERTEX, 2);

	UnsetVertexShader();
	UnsetGeometryShader();
	UnsetPixelShader();

	UnsetShaderResourceView(BIND_VERTEX, 2);

	UnsetRasterizerState();
}

void Particles::UpdateSingularMetadata(int& metadataSlot)
{
	start = metadataSlot;

	UpdateConstantBuffer(startKeeper, &start);

	SetConstantBuffer(startKeeper, BIND_VERTEX, 2);
}

// -- PARTICLE COMPONENT FUNCTION DEFINTIONS -- //
ParticleComponent::ParticleComponent(float seconds, float radius, float size, float x, float y, float z, int amount, ComputeShaders pattern)
{
	metadataSlot = FindSlot();

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;

	// We need to find "amount" of particles free in the physical buffer
	// so we can allocate it for the ParticleComponents logical buffer
	int freeConsecutively = 0;
	int counter = 0;
	for (int i : Particles::m_unoccupiedParticles)
	{
		if (i == -1)
			freeConsecutively++;
		else
			freeConsecutively = 0;

		if (freeConsecutively >= amount)
		{
			data->metadata[metadataSlot].start = counter - amount;
			if (data->metadata[metadataSlot].start == -1)
				data->metadata[metadataSlot].start = 0;
			data->metadata[metadataSlot].end = counter;

			std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + (data->metadata[metadataSlot].end + 1), metadataSlot);
			break;
		}
		counter++;
	}


	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	if (-1 == setToZeroCS)
		setToZeroCS = LoadComputeShader("ParticleTimeResetCS.cso");

	// Prepare dispatch
	SetComputeShader(setToZeroCS);
	SetUnorderedAcessView(Particles::m_readBuffer->UAV, 0);
	SetUnorderedAcessView(Particles::m_writeBuffer->UAV, 1);

	// Reset the time values of the particles to a glorious zero
	Dispatch(1, metadataSlot + 1, 1); //x * y * z


	UnsetComputeShader();
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	//// Call the finish function, no need to reinvent the wheel for this one
	//Particles::FinishParticleCompute(renderStates);
}

ParticleComponent::ParticleComponent(float seconds, float v0X, float size, float x, float y, float z, float rotationY, float v0Z, float v1X, float v1Z, float v2X, float v2Z, int amount, ComputeShaders pattern)
{
	metadataSlot = FindSlot();

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = v0X;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;
	data->metadata[metadataSlot].rotationY = rotationY;
	data->metadata[metadataSlot].positionInfo.x = v0Z; data->metadata[metadataSlot].positionInfo.y = v1X; data->metadata[metadataSlot].positionInfo.z = v1Z;
	data->metadata[metadataSlot].morePositionInfo.x = v2X; data->metadata[metadataSlot].morePositionInfo.y = v2Z; //data->metadata[metadataSlot].morePositionInfo.z = v2X;  data->metadata[metadataSlot].morePositionInfo.w = v2Z;

	// We need to find "amount" of particles free in the physical buffer
	// so we can allocate it for the ParticleComponents logical buffer
	int freeConsecutively = 0;
	int counter = 0;
	for (int i : Particles::m_unoccupiedParticles)
	{
		if (i == -1)
			freeConsecutively++;
		else
			freeConsecutively = 0;

		if (freeConsecutively >= amount)
		{
			data->metadata[metadataSlot].start = counter - (amount - 1); // -1 because we start at 0
			data->metadata[metadataSlot].end = counter;

			std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].end, metadataSlot);
			break;
		}
		counter++;
	}

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);
}

ParticleComponent::~ParticleComponent()
{
	data->metadata[metadataSlot].life = -1.f;
	data->metadata[metadataSlot].maxRange = -1.f;
	data->metadata[metadataSlot].size = -1.f;
	data->metadata[metadataSlot].spawnPos.x = -1.f;	data->metadata[metadataSlot].spawnPos.y = -1.f;	data->metadata[metadataSlot].spawnPos.z = -1.f;
	data->metadata[metadataSlot].pattern = -1.f;

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
	data->metadata[metadataSlot].pattern = -1.f;

	std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].end, -1);

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	metadataSlot = -1;
}

void ParticleComponent::RemoveParticles(EntityID& entity)
{
	data->metadata[metadataSlot].life = -1.f;
	data->metadata[metadataSlot].maxRange = -1.f;
	data->metadata[metadataSlot].size = -1.f;
	data->metadata[metadataSlot].spawnPos.x = 99999.f;	data->metadata[metadataSlot].spawnPos.y = 99999.f;	data->metadata[metadataSlot].spawnPos.z = 99999.f;
	data->metadata[metadataSlot].pattern = -1.f;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	metadataSlot = -1;

	registry.RemoveComponent<ParticleComponent>(entity);

}
