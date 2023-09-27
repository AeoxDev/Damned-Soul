#include "Backend\GeometryIndependent.h"
#include "Camera.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"

#define TEXTURE_DIMENSIONS 256
struct GIConstantBufferData
{
	//Contains what is needed for the pixel shader to know what it should be doing
	float heightThreshold;//The height in y to consider as stage or not
	float idValue;//1 is stage, 2+ are static hazards, this is not used when reading from textures
	float padding;
	bool isTexture;//When hazard, if texture, read from texture instead.
};
struct GeometryIndependentComponent
{
	RTV_IDX renderTargetView;
	PS_IDX pixelShader;
	VS_IDX vertexShader;
	CB_IDX constantBuffer;
	TX_IDX stagingTexture;
	DSV_IDX depthStencil;
	GIConstantBufferData shaderData;

	//TextureComponent
	uint8_t texture[TEXTURE_DIMENSIONS][TEXTURE_DIMENSIONS];

	~GeometryIndependentComponent();
};
void RenderGeometryIndependentCollisionToTexture(Registry& registry, EntityID& stageEntity)
{
	//Find GI component
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == nullptr)
	{
		return;
	}
	//Find stage component

	//Look at model using orthographic camera

	//Set camera orthographic
	Camera::ToggleProjection();
	DirectX::XMVECTOR previousPos = Camera::GetPosition();
	DirectX::XMVECTOR previousLookAt = Camera::GetLookAt();
	DirectX::XMVECTOR previousUp = Camera::GetUp();
	DirectX::XMFLOAT3 vData;

	Camera::SetPosition(0.0f, 5.0f, 0.0f);//Set this to center of stage offset upwards
	Camera::SetLookAt(0.0f, 0.0f, 0.0f);//Set to center of stage
	Camera::SetUp(0.0f, 0.0f, 1.0f);
	Camera::SetWidth(10.0f);//Set width (x) of orthogonal based on stage
	Camera::SetHeight(10.0f);//Set height (z) of orthogonal based on stage
	//Find a depthstencil to use
	//Set VS, PS, RTV, CB
	SetVertexShader(GIcomponent->vertexShader);
	SetPixelShader(GIcomponent->pixelShader);
	SetRenderTargetViewAndDepthStencil(GIcomponent->renderTargetView, GIcomponent->depthStencil);
	SetConstantBuffer(GIcomponent->constantBuffer, SHADER_TO_BIND_RESOURCE::BIND_PIXEL);
	//Update CB
	UpdateConstantBuffer(GIcomponent->constantBuffer, &GIcomponent->shaderData);

	//Render texture to RTV
	d3d11Data->deviceContext->Draw(3, 0);
	//Get texture data from RTV
	ID3D11Texture2D* RTVResource;
	GetTextureByType(RTVResource, TEXTURE_HOLDER_TYPE::RENDER_TARGET_VIEW, GIcomponent->renderTargetView);
	ID3D11Texture2D* stagingResource = nullptr;
	GetTextureByType(stagingResource, TEXTURE_HOLDER_TYPE::TEXTURE, GIcomponent->stagingTexture);
	d3d11Data->deviceContext->CopyResource(RTVResource, stagingResource);


	//float mappingTexture[TEXTURE_DIMENSIONS][TEXTURE_DIMENSIONS] = {0.1f};
	void* mappingTexture = MemLib::spush(sizeof(float) * TEXTURE_DIMENSIONS * TEXTURE_DIMENSIONS);

	D3D11_MAPPED_SUBRESOURCE mappedResource{0};
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(stagingResource, 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map staging resource!" << std::endl;
	}

	// Copy the new data to the buffer
	memcpy(mappedResource.pData, mappingTexture, sizeof(float) * TEXTURE_DIMENSIONS * TEXTURE_DIMENSIONS);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(stagingResource, 0);

	//Another task: Save to a texture with custom size (int8 per pixel)

	float* pixelValue = (float*)mappingTexture;
	for (size_t i = 0; i < TEXTURE_DIMENSIONS; i++)
	{
		for (size_t j = 0; j < TEXTURE_DIMENSIONS; j++)
		{
			GIcomponent->texture[i][j] = (uint8_t) * (pixelValue + i * j + i);
		}
	}


	bool succeeded = MemLib::spop();

	//Set back camera to previous mode
	Camera::ToggleProjection();
	DirectX::XMStoreFloat3(&vData, previousPos);
	Camera::SetPosition(vData.x, vData.y, vData.z);//Set this to center of stage offset upwards
	DirectX::XMStoreFloat3(&vData, previousLookAt);
	Camera::SetLookAt(vData.x, vData.y, vData.z);//Set to center of stage
	DirectX::XMStoreFloat3(&vData, previousUp);
	Camera::SetUp(vData.x, vData.y, vData.z);
	RTVResource->Release();
	stagingResource->Release();
	//Return.
}

bool AddGeometryIndependentComponent(Registry& registry, EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent != NULL)
	{
		return true;
	}
	registry.AddComponent<GeometryIndependentComponent>(stageEntity);
	if (SetupGIAll(registry, stageEntity) == false)
	{
		return false;
	}
	return true;
}

RTV_IDX SetupGIRenderTargetView(Registry& registry, EntityID& stageEntity)
{
	//Get component
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a renderTargetView for the GI
	GIcomponent->renderTargetView = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS::BIND_RENDER_TARGET, (CPU_FLAGS)0, TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS);
	return GIcomponent->renderTargetView;
}
DSV_IDX SetupGIDepthStencil(Registry& registry, EntityID& stageEntity)
{
	//Get component
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a renderTargetView for the GI
	GIcomponent->depthStencil = CreateDepthStencil(TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS);
	return GIcomponent->depthStencil;
}

PS_IDX SetupGIPixelShader(Registry& registry, EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->pixelShader = LoadPixelShader("GICollisionPixel.cso");
	return GIcomponent->pixelShader;
}

VS_IDX SetupGIVertexShader(Registry& registry, EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->vertexShader = LoadVertexShader("TestVS.cso");
	return GIcomponent->vertexShader;
}

CB_IDX SetupGIConstantBuffer(Registry& registry, EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->constantBuffer = CreateConstantBuffer(&GIcomponent->shaderData, sizeof(GIConstantBufferData), 0);
	return GIcomponent->constantBuffer;
}

TX_IDX SetupGIStagingTexture(Registry& registry, EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->stagingTexture = CreateTexture(FORMAT_R8G8B8A8_UNORM,USAGE_FLAGS::USAGE_STAGING, (RESOURCE_FLAGS)0, CPU_FLAGS::READ, TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS);
	return GIcomponent->stagingTexture;
}


bool SetupGIAll(Registry& registry, EntityID& stage)
{
	PS_IDX giPS = SetupGIPixelShader(registry, stage);
	VS_IDX giVS = SetupGIVertexShader(registry, stage);
	CB_IDX giCB = SetupGIConstantBuffer(registry, stage);
	RTV_IDX giRTV = SetupGIRenderTargetView(registry, stage);
	TX_IDX giTX = SetupGIStagingTexture(registry, stage);
	DSV_IDX giDSV = SetupGIDepthStencil(registry, stage);
	if (giPS < 0 || giVS < 0 || giCB < 0 || giRTV < 0)
	{
		return false;
	}
	return true;
}

GeometryIndependentComponent::~GeometryIndependentComponent()
{
	//ReleaseTexture(stagingTexture);
}
