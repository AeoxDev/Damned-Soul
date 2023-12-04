#include "Particles.h"
#include "D3D11Helper\D3D11Helper.h"	
#include "D3D11Helper\D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "Systems\Systems.h"
#include "SDLHandler.h"
#include "Camera.h"
#include "DeltaTime.h"
#include "Registry.h"
#include "Model.h"

PoolPointer<ParticleInputOutput> Particles::m_readBuffer;
PoolPointer<ParticleInputOutput> Particles::m_writeBuffer;
std::vector<int> Particles::m_unoccupiedParticles;
CB_IDX startKeeper;
CB_IDX vfxStartKeeper;
CB_IDX VFXMetadata;
SRV_IDX particleSRV;

PoolPointer<ParticleMetadataBuffer> data;

// ## ALEX CODE ##
CB_IDX VFXConstantBuffer;

SRV_IDX VFXBackBufferSRV; // Difference between SRV and TX is that SRV are textures created by the pipeline.
TX_IDX VFXColorRampTX;
TX_IDX VFXNoiseTX;
TX_IDX VFXShapeTX;
TX_IDX VFXMaskTX;


struct VFXBufferData
{
	DirectX::XMFLOAT2 resolution_in; // Screen Resolution
};
// ## EO ALEX CODE ##

int Particles::RenderSlot;

// Compute shader used to reset particle components
CS_IDX setToZeroCS = -1;
// Shaders for VFX
VS_IDX MeshVS = -1;

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
	/*VFXBufferData VFXData = {
		DirectX::XMFLOAT2(0.0f, 1.0f),
		DirectX::XMFLOAT2(sdl.WIDTH,sdl.HEIGHT),
		0.75f };*/
	VFXBufferData VFXData = { DirectX::XMFLOAT2(sdl.WIDTH,sdl.HEIGHT) }; //Kept this in the merge
	VFXConstantBuffer = CreateConstantBuffer((void*)&VFXData, sizeof(VFXBufferData));

	//VFXSampler =		CreateSamplerState();
	VFXBackBufferSRV =	CreateShaderResourceViewTexture(renderStates[backBufferRenderSlot].renderTargetView, RESOURCE_FLAGS::BIND_RENDER_TARGET);
	VFXColorRampTX =	LoadTexture("\\VFX_FireGradient.png");
	VFXNoiseTX =		LoadTexture("\\VFX_Noises.png");
	VFXShapeTX =		LoadTexture("\\VFX_Shapes.png");
	VFXMaskTX =			LoadTexture("\\VFX_Masks.png");
	VFXMetadata =		CreateConstantBuffer(sizeof(VFXMeshData));
// ## EO ALEX CODE ##

	flipBookTexture = LoadTexture("\\SpriteFireLavaBubble.png");//created texture resource //note that dubble slash need to be used before texture file name ("\\LavaPlaceholderAlpha.png")
	flipBookTextureTwo = LoadTexture("\\SpriteSmokeSpark.png");//created texture resource 
	textureParticle = LoadTexture("\\LavaPlaceholderAlpha.png");
	noTextureParticle = LoadTexture("\\DefaultParticle.png");

	sampler = CreateSamplerState(); //created sampler resource


	data = MemLib::palloc(sizeof(ParticleMetadataBuffer));
	m_readBuffer = MemLib::palloc(sizeof(ParticleInputOutput));
	m_writeBuffer = MemLib::palloc(sizeof(ParticleInputOutput));

	m_unoccupiedParticles = std::vector<int>(MAX_PARTICLES, -1); 

	Particle* particles;
	particles = (Particle*)MemLib::spush(sizeof(Particle) * MAX_PARTICLES);

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].position = DirectX::XMFLOAT3(99999.f, 99999.f, 99999.f);
		particles[i].time = 0.f;
		particles[i].velocity = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
		particles[i].rotationZ = 0.f;
		particles[i].rgb = DirectX::XMFLOAT3(1.f, 0.f, 0.f);
		particles[i].size = 0.f;
		particles[i].patterns= -1;
		particles[i].VFX =  -1;

	}

	RESOURCE_FLAGS resourceFlags = static_cast<RESOURCE_FLAGS>(BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_readBuffer->SRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, NONE);
	m_writeBuffer->SRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, resourceFlags, NONE);

	m_readBuffer->UAV = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_readBuffer->SRV);
	m_writeBuffer->UAV = CreateUnorderedAccessViewBuffer(sizeof(Particle), MAX_PARTICLES, m_writeBuffer->SRV);

	particleSRV = CreateShaderResourceViewBuffer(&(*particles), sizeof(Particle), MAX_PARTICLES, BIND_SHADER_RESOURCE, READ);

	MemLib::spop(); // for particles

	// Constant buffer that keeps the offset of particle component in the physical buffer for vertex pulling
	startKeeper = CreateConstantBuffer(sizeof(int));
	vfxStartKeeper = CreateConstantBuffer(sizeof(int));


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
	MeshVS = LoadVertexShader("VFX_MESH_VS.cso", DEFAULT);
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

void Particles::PrepareParticleCompute()
{
	SwitchInputOutput();

	if (Camera::InCutscene() > 0)
	{
		data->metadata[0].deltaTime = GetFrameTime();
	}
	else
	{
		data->metadata[0].deltaTime = GetDeltaTime();
	}

	UpdateConstantBuffer(renderStates[RenderSlot].constantBuffer, data->metadata);

	SetComputeShader(renderStates[RenderSlot].computeShader);
	SetConstantBuffer(renderStates[RenderSlot].constantBuffer, BIND_COMPUTE, 0);
	SetUnorderedAcessView(m_writeBuffer->UAV, 0);
	SetUnorderedAcessView(m_readBuffer->UAV, 1);
}

void Particles::FinishParticleCompute()
{
	UnsetComputeShader();
	UnsetUnorderedAcessView(0);
	UnsetUnorderedAcessView(1);
	UnsetConstantBuffer(BIND_COMPUTE, 0);
}

void Particles::PrepareParticlePass(int metadataSlot)
{
	SetTopology(POINTLIST);

	CopySRVtoSRV(particleSRV, m_writeBuffer->SRV);

	SetVertexShader(renderStates[RenderSlot].vertexShaders[0], true);
	SetGeometryShader(renderStates[RenderSlot].geometryShader);
	SetPixelShader(renderStates[RenderSlot].pixelShaders[0]);

	SetConstantBuffer(VFXConstantBuffer, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 0);
	SetShaderResourceView(VFXBackBufferSRV, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 0);
	SetTexture(VFXColorRampTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 1);
	SetTexture(VFXNoiseTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 2);
	SetTexture(VFXShapeTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 3);
	SetTexture(VFXMaskTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 4);
	SetSamplerState(sampler, 3);

	// ## EO ALEX CODE ##

	SetConstantBuffer(startKeeper, BIND_VERTEX, 2);
	SetConstantBuffer(Camera::GetCameraBufferIndex(), BIND_GEOMETRY, 1);
	SetShaderResourceView(particleSRV, BIND_VERTEX, 0);
	UnsetVertexBuffer();
	UnsetIndexBuffer();

	SetRasterizerState(renderStates[RenderSlot].rasterizerState);

	if (  data->metadata[metadataSlot].pattern == FIRE || data->metadata[metadataSlot].pattern == BOILING)
		SetTexture(flipBookTexture, BIND_PIXEL, 6); 
	else if (data->metadata[metadataSlot].pattern == SPARK || data->metadata[metadataSlot].pattern == SMOKE)
		SetTexture(flipBookTextureTwo, BIND_PIXEL, 6); 
	else if (data->metadata[metadataSlot].pattern == FLAMETHROWER || data->metadata[metadataSlot].pattern == PULSE)
		SetTexture(textureParticle, BIND_PIXEL, 6); 
	else
		SetTexture(noTextureParticle, BIND_PIXEL, 6);


	/*if (data->metadata[metaDataSlot].pattern == 0 || data->metadata[metaDataSlot].pattern == 9 /*|| data->metadata[metaDataSlot].pattern == 10)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
	{

		SetTexture(flipBookTexture, BIND_PIXEL, 2); //Set texture
		SetTexture(flipBookTextureTwo, BIND_PIXEL, 2); //Set texture


	}
	else if (data->metadata[metaDataSlot].pattern == 15)//	SMOKE = 0,ARCH = 1,EXPLOSION = 2,FLAMETHROWER = 3,IMPLOSION = 4,RAIN = 5,SINUS = 6,
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
	*/

}

void Particles::FinishParticlePass()
{
	UnsetVertexShader();
	UnsetGeometryShader();
	UnsetPixelShader();

	UnsetConstantBuffer(BIND_GEOMETRY, 1);
	UnsetConstantBuffer(BIND_VERTEX, 2);
	UnsetShaderResourceView(BIND_VERTEX, 0);

	// Alex Code
	UnsetConstantBuffer(BIND_PIXEL, 0);
	UnsetShaderResourceView(BIND_PIXEL, 0);
	UnsetTexture(VFXColorRampTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 1);
	UnsetTexture(VFXNoiseTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 2);
	UnsetTexture(VFXShapeTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 3);
	UnsetTexture(VFXMaskTX, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 4);

	UnsetTexture(textureParticle, BIND_PIXEL, 6);
	
	UnsetSamplerState(3);

	UnsetRasterizerState();

	SetTopology(TRIANGLELIST);
}

void Particles::PrepareMeshPass(int metadataSlot, ParticleComponent& pComp, float timeCap)
{
	PrepareParticlePass(metadataSlot);

	SetTopology(TRIANGLELIST);

	UnsetVertexShader();
	UnsetGeometryShader();

	pComp.VFXTimeAlive += timeCap;// data->metadata[0].deltaTime;

	// We want everything from the regular particle pass but without the geoemtry shader and a different vertex shader
	SetVertexShader(MeshVS);

	SetConstantBuffer(renderStates[RenderSlot].constantBuffer, BIND_VERTEX, 2);
	
	VFXMeshData tempData;
	tempData.metadataSlot = metadataSlot;
	tempData.timeAlive = pComp.VFXTimeAlive;

	UpdateConstantBuffer(VFXMetadata, &tempData);
	SetConstantBuffer(VFXMetadata, BIND_VERTEX, 3);

}

void Particles::FinishMeshPass()
{
	FinishParticlePass();

	UnsetConstantBuffer(BIND_VERTEX, 2);
	UnsetConstantBuffer(BIND_VERTEX, 3);
}


void Particles::UpdateStart(int& metadataSlot)
{
	int start = data->metadata[metadataSlot].start;
	UnsetConstantBuffer(BIND_VERTEX, 2);

	UpdateConstantBuffer(startKeeper, &start);
}

void Particles::UpdateVFXStart(int& metadataSlot)
{
	int start = data->metadata[metadataSlot].start;
	UnsetConstantBuffer(BIND_VERTEX, 3);

	UpdateConstantBuffer(vfxStartKeeper, &start);
}


// -- PARTICLE COMPONENT FUNCTION DEFINTIONS -- //
ParticleComponent::ParticleComponent(float seconds, float radius, float size, float offsetX, float offsetY, float offsetZ, int amount, ComputeShaders pattern)
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
	data->metadata[metadataSlot].spawnPos.x = offsetX;	data->metadata[metadataSlot].spawnPos.y = offsetY;	data->metadata[metadataSlot].spawnPos.z = offsetZ;
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

ParticleComponent::ParticleComponent(float seconds, float radius, float size, float offsetX, float offsetY, float offsetZ, int amount, VFX_PATTERN vfxPattern)
{
	metadataSlot = FindSlot();
	VFX = true;

	//Calculate how many groups are requiered to write to all particles
	float groups = (float)amount / (float)THREADS_PER_GROUP;
	if (groups == (int)groups)
		groupsRequiered = groups;
	else
		groupsRequiered = groups + 1;

	data->metadata[metadataSlot].life = seconds;
	data->metadata[metadataSlot].maxRange = radius;
	data->metadata[metadataSlot].size = size;
	data->metadata[metadataSlot].spawnPos.x = offsetX;	data->metadata[metadataSlot].spawnPos.y = offsetY;	data->metadata[metadataSlot].spawnPos.z = offsetZ;
	data->metadata[metadataSlot].pattern = 14;
	data->metadata[metadataSlot].morePositionInfo.y = vfxPattern;

	data->metadata[metadataSlot].positionInfo.x = -9999.f; data->metadata[metadataSlot].positionInfo.y = -9999.f; data->metadata[metadataSlot].positionInfo.z = -9999.f;
	data->metadata[metadataSlot].morePositionInfo.x = -9999.f;
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

ParticleComponent::ParticleComponent(float seconds, float radius, float size, float offsetX, float offsetY, float offsetZ, int amount, const char* filename, VFX_PATTERN vfxPattern)
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
	data->metadata[metadataSlot].spawnPos.x = offsetX;	data->metadata[metadataSlot].spawnPos.y = offsetY;	data->metadata[metadataSlot].spawnPos.z = offsetZ;
	data->metadata[metadataSlot].pattern = 14;
	data->metadata[metadataSlot].morePositionInfo.y = vfxPattern;

	data->metadata[metadataSlot].positionInfo.x = -9999.f; data->metadata[metadataSlot].positionInfo.y = -9999.f; data->metadata[metadataSlot].positionInfo.z = -9999.f;
	data->metadata[metadataSlot].morePositionInfo.x = -9999.f;
	data->metadata[metadataSlot].reset = false;

	UpdateConstantBuffer(renderStates[Particles::RenderSlot].constantBuffer, data->metadata);

	model = LoadModel(filename);
	modelUse = true;

	VFXTimeAlive = 0.0f;

	meshOffset[0] = offsetX;
	meshOffset[1] = offsetY;
	meshOffset[2] = offsetZ;

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
	groupsRequiered = -1;
	VFX = false;
	modelUse = false;
}
