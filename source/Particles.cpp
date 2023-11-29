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
CB_IDX startKeeper;
SRV_IDX particleSRV;

PoolPointer<ParticleMetadataBuffer> data;

// ## ALEX CODE ##
GS_IDX tempUVPanningGS;
PS_IDX VFXPixelShader;
CB_IDX VFXConstantBuffer;

SMP_IDX VFXSampler;

SRV_IDX VFXBackBufferSRV; // Difference between SRV and TX is that SRV are textures created by the pipeline.
TX_IDX VFXColorRampTX;
TX_IDX VFXVornoiTX;
TX_IDX VFXNoiseTX;
TX_IDX VFXShapeTX;
TX_IDX VFXMaskTX;


struct VFXBufferData
{
	DirectX::XMFLOAT2 offsetXY_in; // Offset of the uv coordinates in x ( u ) and y ( v ). Clamped between 1 and -1 since its illogial to do other ones.
	DirectX::XMFLOAT2 resolution_in; // Offset of the uv coordinates in x ( u ) and y ( v ). Clamped between 1 and -1 since its illogial to do other ones.
	float panSpeed_in; // How fast do you want the panning to be. A multiplier.
};
// ## EO ALEX CODE ##

int Particles::RenderSlot;

// Compute shader used to reset particle components
CS_IDX setToZeroCS = -1;

TX_IDX flipBookTexture = -1; /// create holder for texture
TX_IDX flipBookTextureTwo = -1; /// create holder for texture
TX_IDX textureParticle = -1; /// create holder for texture
TX_IDX noTextureParticle = -1; /// create holder for texture
SMP_IDX sampler = -1; //create holder for sampler

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
// ## ALEX CODE ##
	VFXPixelShader = LoadPixelShader("VFXPixel.cso");
	tempUVPanningGS = LoadGeometryShader("VFXGeometry.cso");

	VFXBufferData VFXData = {
		DirectX::XMFLOAT2(0.0f, 1.0f),
		DirectX::XMFLOAT2(sdl.WIDTH,sdl.HEIGHT),
		0.75f };
	VFXConstantBuffer = CreateConstantBuffer((void*)&VFXData, sizeof(VFXBufferData));

	VFXSampler =		CreateSamplerState();
	VFXBackBufferSRV =	CreateShaderResourceViewTexture(renderStates[backBufferRenderSlot].renderTargetView, RESOURCE_FLAGS::BIND_RENDER_TARGET);
	VFXColorRampTX =	LoadTexture("\\VFX_FireGradient.png");
	VFXVornoiTX =		LoadTexture("\\VFX_Vornoi.png");
	VFXNoiseTX =		LoadTexture("\\VFX_gNoise.png");
	//VFXShapeTX =		LoadTexture("\\VFX_InnerGradient.png"); // VFX_SwordSlash
	VFXShapeTX =		LoadTexture("\\VFX_CircleSoft.png"); // VFX_Fire
	VFXMaskTX =			LoadTexture("\\VFX_GradientMask.png");
// ## EO ALEX CODE ##

	data = MemLib::palloc(sizeof(ParticleMetadataBuffer));
	m_readBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_writeBuffer = MemLib::palloc(sizeof(ParticleInputOutput));

	m_unoccupiedParticles = std::vector<int>(MAX_PARTICLES, -1);  //.reserve(sizeof(int) * MAX_PARTICLES);
	//std::fill(m_unoccupiedParticles.begin(), m_unoccupiedParticles.end(), -1);

	Particle* particles;
	particles = (Particle*)MemLib::spush(sizeof(Particle) * MAX_PARTICLES);

	flipBookTexture = LoadTexture("\\SpriteFireLavaBubble.png");//created texture resource //note that dubble slash need to be used before texture file name ("\\LavaPlaceholderAlpha.png")
	flipBookTextureTwo = LoadTexture("\\SpriteSmokeSpark.png");//created texture resource 
	textureParticle = LoadTexture("\\LavaPlaceholderAlpha.png");
	noTextureParticle = LoadTexture("\\DefaultParticle.png");

	sampler = CreateSamplerState(); //created sampler resource

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].position = DirectX::XMFLOAT3(99999.f, 99999.f, 99999.f);
		particles[i].time = 0.f;
		particles[i].velocity = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
		particles[i].rotationZ = 0.f;
		particles[i].rgb = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
		particles[i].size = 0.f;
		particles[i].patterns= -1;
		particles[i].particlePatterns =  -1;

	}

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_readBuffer->SRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, NONE);
	m_writeBuffer->SRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, NONE);

	m_readBuffer->UAV = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_readBuffer->SRV);
	m_writeBuffer->UAV = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_writeBuffer->SRV);

	particleSRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, BIND_SHADER_RESOURCE, READ);

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
		data->metadata[i].reset = false;




	}

	setToZeroCS = LoadComputeShader("ParticleTimeResetCS.cso");
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
	data->metadata[metadataSlot].maxRange = v0x;
	data->metadata[metadataSlot].positionInfo.x = v0z; data->metadata[metadataSlot].positionInfo.y = v1x; data->metadata[metadataSlot].positionInfo.z = v1z;
	data->metadata[metadataSlot].morePositionInfo.x = v2x; data->metadata[metadataSlot].morePositionInfo.y = v2z; 
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
	SetUnorderedAcessView(m_writeBuffer->UAV, 0);
	SetUnorderedAcessView(m_readBuffer->UAV, 1);
}

void Particles::FinishParticleCompute(RenderSetupComponent renderStates[8])
{
	UnsetComputeShader();
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	UnsetConstantBuffer(BIND_COMPUTE, 0);
}

void Particles::PrepareParticlePass(RenderSetupComponent renderStates[8], int metaDataSlot)
{
	SetTopology(POINTLIST);

	CopySRVtoSRV(particleSRV, m_writeBuffer->SRV);

	SetVertexShader(renderStates[RenderSlot].vertexShaders[0], true);
	SetGeometryShader(renderStates[RenderSlot].geometryShader);
	SetPixelShader(renderStates[RenderSlot].pixelShaders[0]);

	// The constant buffer for vertex is set outside of this function, in the ParticleSystemCPU Update() call
	SetShaderResourceView(particleSRV, BIND_VERTEX, 0);
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_GEOMETRY, 1);
	UnsetVertexBuffer();
	UnsetIndexBuffer();

	SetRasterizerState(renderStates[RenderSlot].rasterizerState);

	if (data->metadata[metaDataSlot].pattern == 0 || data->metadata[metaDataSlot].pattern == 9 /*|| data->metadata[metaDataSlot].pattern == 10*/)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
	{

		SetTexture(flipBookTexture, BIND_PIXEL, 2); //Set texture
		//SetTexture(flipBookTextureTwo, BIND_PIXEL, 2); //Set texture


	}
	//else if (data->metadata[metaDataSlot].pattern == 15)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
	//{

	//	SetTexture(flipBookTextureTwo, BIND_PIXEL, 2); //Set texture


	//}
	else if (data->metadata[metaDataSlot].pattern == 3)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
	{

		SetTexture(textureParticle, BIND_PIXEL, 2); //Set texture


	}
	else
	{

		SetTexture(noTextureParticle, BIND_PIXEL, 2); //Set texture

	}
	SetSamplerState(sampler, 2); //Set sampler


}

void Particles::FinishParticlePass()
{
	UnsetVertexShader();
	UnsetGeometryShader();
	UnsetPixelShader();

	SetTopology(TRIANGLELIST);

	UnsetConstantBuffer(BIND_GEOMETRY, 1);
	UnsetConstantBuffer(BIND_VERTEX, 2);

	UnsetShaderResourceView(BIND_VERTEX, 0);

	UnsetRasterizerState();

	//UnsetTexture missing//			   //Unset texture 
	UnsetSamplerState(2); //Unset sampler 
}

void Particles::PrepareVFXPass(RenderSetupComponent renderStates[8], int metaDataSlot)
{
	SetTopology(POINTLIST);

	CopySRVtoSRV(particleSRV, m_writeBuffer->SRV);

	SetVertexShader(renderStates[RenderSlot].vertexShaders[0], true);
	//SetGeometryShader(renderStates[RenderSlot].geometryShader);
	//SetPixelShader(renderStates[RenderSlot].pixelShaders[0]);

	// ## ALEX CODE ##
	SetGeometryShader(tempUVPanningGS);
	SetPixelShader(VFXPixelShader);
	SetConstantBuffer(VFXConstantBuffer, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 0);
	SetShaderResourceView(VFXBackBufferSRV, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 0);
	SetTexture(VFXColorRampTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 1);
	SetTexture(VFXVornoiTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 2);
	SetTexture(VFXNoiseTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 3);
	SetTexture(VFXShapeTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 4);
	SetTexture(VFXMaskTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 5);
	SetSamplerState(VFXSampler, 3);

	// ## EO ALEX CODE ##

	// The constant buffer for vertex is set outside of this function, in the ParticleSystemCPU Update() call
	SetShaderResourceView(particleSRV, BIND_VERTEX, 0);
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_GEOMETRY, 1);
	UnsetVertexBuffer();
	UnsetIndexBuffer();

	SetRasterizerState(renderStates[RenderSlot].rasterizerState);

	if (data->metadata[metaDataSlot].pattern == 0 || data->metadata[metaDataSlot].pattern == 9 || data->metadata[metaDataSlot].pattern == 12)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
	{

		SetTexture(flipBookTexture, BIND_PIXEL, 2); //Set texture
		//SetTexture(flipBookTextureTwo, BIND_PIXEL, 2); //Set texture


	}
	else if (data->metadata[metaDataSlot].pattern == 13)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
	{

		SetTexture(flipBookTextureTwo, BIND_PIXEL, 2); //Set texture


	}
	else if (data->metadata[metaDataSlot].pattern == 3)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
	{

		SetTexture(textureParticle, BIND_PIXEL, 2); //Set texture


	}
	else
	{

		SetTexture(noTextureParticle, BIND_PIXEL, 2); //Set texture

	}
	SetSamplerState(sampler, 2); //Set sampler

}

void Particles::FinishVFXPass()
{
	UnsetVertexShader();
	UnsetGeometryShader();
	UnsetPixelShader();

	// Alex Code
	UnsetConstantBuffer(BIND_PIXEL, 0);
	UnsetSamplerState(3);
	UnsetShaderResourceView(SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 0);

	UnsetTexture(VFXColorRampTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 1);
	UnsetTexture(VFXVornoiTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 2);
	UnsetTexture(VFXNoiseTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 3);
	UnsetTexture(VFXShapeTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 4);
	UnsetTexture(VFXMaskTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 5);

	SetTopology(TRIANGLELIST);

	UnsetConstantBuffer(BIND_GEOMETRY, 1);
	UnsetConstantBuffer(BIND_VERTEX, 2);

	UnsetShaderResourceView(BIND_VERTEX, 0);

	UnsetRasterizerState();

	//UnsetTexture missing//			   //Unset texture 
	UnsetSamplerState(2); //Unset sampler 
}

void Particles::UpdateSingularMetadata(int& metadataSlot)
{
	int start;
	start = data->metadata[metadataSlot].start;

	UnsetConstantBuffer(BIND_VERTEX, 2);

	UpdateConstantBuffer(startKeeper, &start);

	SetConstantBuffer(startKeeper, BIND_VERTEX, 2);
}

// -- PARTICLE COMPONENT FUNCTION DEFINTIONS -- //
ParticleComponent::ParticleComponent(float seconds, float radius, float size, float x, float y, float z, int amount, ComputeShaders pattern)
{
	metadataSlot = FindSlot();
	//Calculate how many groups are requiered to write to all particles
	float groups = (float)amount / (float)THREADS_PER_GROUP;
	if (groups == (int)groups)
		groupsRequiered = groups;
	else
		groupsRequiered = groups + 1;

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;

	data->metadata[metadataSlot].positionInfo.x = -9999.f; data->metadata[metadataSlot].positionInfo.y = -9999.f; data->metadata[metadataSlot].positionInfo.z = -9999.f;
	data->metadata[metadataSlot].morePositionInfo.x = -9999.f; data->metadata[metadataSlot].morePositionInfo.y = -9999.f;
	data->metadata[metadataSlot].reset = false;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);


	// We need to find "amount" of particles free in the physical buffer
	// so we can allocate it for the ParticleComponents logical buffer
	int freeConsecutively = 0;
	int counter = 0;
	for (int i : Particles::m_unoccupiedParticles)
	{
		counter++;

		if (i == -1)
			freeConsecutively++;
		else
			freeConsecutively = 0;

		if (freeConsecutively >= amount)
		{
			data->metadata[metadataSlot].start = counter - amount;
			data->metadata[metadataSlot].end = counter - 1;


			std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + (data->metadata[metadataSlot].end + 1), metadataSlot);
			break;
		}
		
	}
}

ParticleComponent::ParticleComponent(float seconds, float radius, float size, float x, float y, float z, float speed, int amount, ComputeShaders pattern)
{
	metadataSlot = FindSlot();
	//Calculate how many groups are requiered to write to all particles
	float groups = (float)amount / (float)THREADS_PER_GROUP;
	if (groups == (int)groups)
		groupsRequiered = groups;
	else
		groupsRequiered = groups + 1;

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;

	data->metadata[metadataSlot].positionInfo.x = speed; data->metadata[metadataSlot].positionInfo.y = -9999.f; data->metadata[metadataSlot].positionInfo.z = -9999.f;
	data->metadata[metadataSlot].morePositionInfo.x = -9999.f; data->metadata[metadataSlot].morePositionInfo.y = -9999.f;
	data->metadata[metadataSlot].reset = false;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);


	// We need to find "amount" of particles free in the physical buffer
	// so we can allocate it for the ParticleComponents logical buffer
	int freeConsecutively = 0;
	int counter = 0;
	for (int i : Particles::m_unoccupiedParticles)
	{
		counter++;

		if (i == -1)
			freeConsecutively++;
		else
			freeConsecutively = 0;

		if (freeConsecutively >= amount)
		{
			data->metadata[metadataSlot].start = counter - amount;
			data->metadata[metadataSlot].end = counter - 1;


			std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + (data->metadata[metadataSlot].end + 1), metadataSlot);
			break;
		}

	}
}

ParticleComponent::ParticleComponent(float seconds, float radius, float size, float offsetX, float offsetY, float offsetZ, int amount, VFX_PATTERN vfxPattern, ComputeShaders pattern)
{
}

ParticleComponent::ParticleComponent(float seconds, float v0X, float size, float x, float y, float z, float rotationY, float v0Z, float v1X, float v1Z, float v2X, float v2Z, int amount, ComputeShaders pattern)
{
	metadataSlot = FindSlot();
	//Calculate how many groups are requiered to write to all particles
	float groups = (float)amount / (float)THREADS_PER_GROUP;
	if (groups == (int)groups)
		groupsRequiered = groups;
	else
		groupsRequiered = groups + 1;

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = v0X;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = x;	data->metadata[metadataSlot].spawnPos.y = y;	data->metadata[metadataSlot].spawnPos.z = z;
	data->metadata[metadataSlot].pattern = pattern;

	data->metadata[metadataSlot].positionInfo.x = -9999.f; data->metadata[metadataSlot].positionInfo.y = -9999.f; data->metadata[metadataSlot].positionInfo.z = -9999.f;
	data->metadata[metadataSlot].morePositionInfo.x = -9999.f; data->metadata[metadataSlot].morePositionInfo.y = -9999.f;
	data->metadata[metadataSlot].reset = false;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);
	// We need to find "amount" of particles free in the physical buffer
	// so we can allocate it for the ParticleComponents logical buffer
	int freeConsecutively = 0;
	int counter = 0;
	for (int i : Particles::m_unoccupiedParticles)
	{
		counter++;

		if (i == -1)
			freeConsecutively++;
		else
			freeConsecutively = 0;

		if (freeConsecutively >= amount)
		{
			data->metadata[metadataSlot].start = counter - amount;
			data->metadata[metadataSlot].end = counter - 1;


			std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + (data->metadata[metadataSlot].end + 1), metadataSlot);
			break;
		}
	}

	//// Prepare dispatch
	//SetComputeShader(setToZeroCS);
	//SetUnorderedAcessView(Particles::m_readBuffer->UAV, 0);
	//SetUnorderedAcessView(Particles::m_writeBuffer->UAV, 1);
	//SetConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, BIND_COMPUTE, 0);

	//// Reset the time values of the particles to a glorious zero
	//Dispatch(groupsRequiered + 1, metadataSlot + 1, 1); //x * y * z

	//// Finish dispatch
	//UnsetComputeShader();
	//UnsetUnorderedAcessView(0);
	//UnsetUnorderedAcessView(1);
	//UnsetConstantBuffer(BIND_COMPUTE, 0);
}


// IRRELEVANT
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
	if (data->metadata[metadataSlot].start >= data->metadata[metadataSlot].end)
		return;

	std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].end, -1);
	
	// Declare to the compute shader that this components particles is to be reset
	data->metadata[metadataSlot].reset = true;
	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	// Prepare dispatch
	SetComputeShader(setToZeroCS);
	SetUnorderedAcessView(Particles::m_readBuffer->UAV, 0);
	SetUnorderedAcessView(Particles::m_writeBuffer->UAV, 1);
	SetConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, BIND_COMPUTE, 0);

	// Reset the time values of the particles to a glorious zero
	Dispatch(groupsRequiered + 1, metadataSlot + 1, 1); //x * y * z

	// Finish dispatch
	UnsetComputeShader();
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	UnsetConstantBuffer(BIND_COMPUTE, 0);

	data->metadata[metadataSlot].life = -1.f;
	data->metadata[metadataSlot].maxRange = -1.f;
	data->metadata[metadataSlot].size = -1.f;
	data->metadata[metadataSlot].spawnPos.x = 99999.f;	data->metadata[metadataSlot].spawnPos.y = 99999.f;	data->metadata[metadataSlot].spawnPos.z = 99999.f;
	data->metadata[metadataSlot].pattern = -1;
	data->metadata[metadataSlot].start = 0.f; data->metadata[metadataSlot].end = 0.f;
	data->metadata[metadataSlot].positionInfo.x = 99999.f; data->metadata[metadataSlot].positionInfo.y = 99999.f; data->metadata[metadataSlot].positionInfo.z = 99999.f;
	data->metadata[metadataSlot].morePositionInfo.x = 99999.f; data->metadata[metadataSlot].morePositionInfo.y = 99999.f;
	data->metadata[metadataSlot].reset = false;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	metadataSlot = -1;
}

//void ParticleComponent::RemoveParticles(EntityID& entity)
//{
//	std::fill(Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].start, Particles::m_unoccupiedParticles.begin() + data->metadata[metadataSlot].end, -1);
//
//	// Declare to the compute shader that this components particles is to be reset
//	data->metadata[metadataSlot].reset = true;
//	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);
//
//	// Prepare dispatch
//	SetComputeShader(setToZeroCS);
//	SetUnorderedAcessView(Particles::m_readBuffer->UAV, 0);
//	SetUnorderedAcessView(Particles::m_writeBuffer->UAV, 1);
//	SetConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, BIND_COMPUTE, 0);
//
//	// Reset the time values of the particles to a glorious zero
//	Dispatch(groupsRequiered + 1, metadataSlot + 1, 1); //x * y * z
//
//	// Finish dispatch
//	UnsetComputeShader();
//	UnsetUnorderedAcessView(0);
//	UnsetUnorderedAcessView(1);
//	UnsetConstantBuffer(BIND_COMPUTE, 0);
//
//	data->metadata[metadataSlot].life = -1.f;
//	data->metadata[metadataSlot].maxRange = -1.f;
//	data->metadata[metadataSlot].size = -1.f;
//	data->metadata[metadataSlot].spawnPos.x = 99999.f;	data->metadata[metadataSlot].spawnPos.y = 99999.f;	data->metadata[metadataSlot].spawnPos.z = 99999.f;
//	data->metadata[metadataSlot].pattern = -1.f;
//	data->metadata[metadataSlot].start = 0.f; data->metadata[metadataSlot].end = 0.f;
//
//	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);
//
//	metadataSlot = -1;
//
//	registry.RemoveComponent<ParticleComponent>(entity);
//}

void ParticleComponent::ResetBuffer()
{
	data->metadata[metadataSlot].life = -1.f;
	data->metadata[metadataSlot].maxRange = -1.f;
	data->metadata[metadataSlot].size = -1.f;
	data->metadata[metadataSlot].spawnPos.x = 99999.f;	data->metadata[metadataSlot].spawnPos.y = 99999.f;	data->metadata[metadataSlot].spawnPos.z = 99999.f;
	data->metadata[metadataSlot].pattern = -1;
	data->metadata[metadataSlot].start = 0.f; data->metadata[metadataSlot].start = 0.f;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	metadataSlot = -1;
	bufferReset = true;
}

void ParticleComponent::ResetVertex()
{
	// Prepare dispatch
	SetComputeShader(setToZeroCS);
	SetUnorderedAcessView(Particles::m_readBuffer->UAV, 0);
	SetUnorderedAcessView(Particles::m_writeBuffer->UAV, 1);
	SetConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, BIND_COMPUTE, 0);

	// Reset the time values of the particles to a glorious zero
	Dispatch(groupsRequiered + 1, metadataSlot + 1, 1); //x * y * z


	UnsetComputeShader();
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	UnsetConstantBuffer(BIND_COMPUTE, 0);

	vertexReset = true;
}
