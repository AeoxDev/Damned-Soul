#include "Backend\GeometryIndependent.h"
#include "Camera.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "AllComponents.h"
#include "GameRenderer.h"

#define TEXTURE_DIMENSIONS 16
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
	RS_IDX rasterizerState;
	VP_IDX viewport;
	GIConstantBufferData shaderData;

	//TextureComponent
	int8_t texture[TEXTURE_DIMENSIONS][TEXTURE_DIMENSIONS];

	~GeometryIndependentComponent();
};
struct giPixel
{
	unsigned r;
};
struct giCopyTexture
{
	giPixel texture[TEXTURE_DIMENSIONS*2][TEXTURE_DIMENSIONS];
};
void RenderGeometryIndependentCollisionToTexture(Registry& registry, EntityID& stageEntity, EntityID& modelEntity)
{
	//Find GI component
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == nullptr)
	{
		return;
	}
	ModelComponent* model = registry.GetComponent<ModelComponent>(modelEntity);
	//Find stage component

	//Look at model using orthographic camera

	//Set camera orthographic
	Camera::ToggleProjection();
	
	DirectX::XMVECTOR previousPos = Camera::GetPosition();
	DirectX::XMVECTOR previousLookAt = Camera::GetLookAt();
	DirectX::XMVECTOR previousUp = Camera::GetUp();
	DirectX::XMFLOAT3 vData;

	const VertexBoneless* vertices = model->model.m_bonelessModel->GetVertices();

	unsigned nrVertices = model->model.m_bonelessModel->m_numVertices;

	float greatestX = -1000000000.0f;
	float greatestZ = -1000000000.0f;
	float greatestY = -1000000000.0f;
	float smallestX = 1000000000.0f;
	float smallestZ = 1000000000.0f;
	float smallestY = 1000000000.0f;

	for (unsigned i = 0; i < nrVertices; i++)
	{
		if (vertices[i].m_position[0] < smallestX)
		{
			smallestX = vertices[i].m_position[0];
		}
		if (vertices[i].m_position[0] > greatestX)
		{
			greatestX = vertices[i].m_position[0];
		}

		if (vertices[i].m_position[1] < smallestY)
		{
			smallestY = vertices[i].m_position[1];
		}
		if (vertices[i].m_position[1] > greatestY)
		{
			greatestY = vertices[i].m_position[1];
		}

		if (vertices[i].m_position[2] < smallestZ)
		{
			smallestZ = vertices[i].m_position[2];
		}
		if (vertices[i].m_position[2] > greatestZ)
		{
			greatestZ = vertices[i].m_position[2];
		}
	}

	Camera::SetPosition(0.5f * (greatestX + smallestX),greatestY+ 2.0f, 0.5f * (greatestZ + smallestZ));//Set this to center of stage offset upwards
	Camera::SetLookAt(0.5f * (greatestX + smallestX), smallestY, 0.5f * (greatestZ + smallestZ));//Set to center of stage
	Camera::SetUp(0.0f, 0.0f, 1.0f);
	Camera::SetWidth(greatestX - smallestX + 2.0f);//Set width (x) of orthogonal based on stage
	Camera::SetHeight(greatestZ - smallestZ + 2.0f);//Set height (z) of orthogonal based on stage
	Camera::SetOrthographicDepth(greatestY - smallestY + 8.f);
	Camera::UpdateView();
	Camera::UpdateProjection();
	int16_t cameraIdx = Camera::GetCameraBufferIndex();
	SetConstantBuffer(cameraIdx, SHADER_TO_BIND_RESOURCE::BIND_VERTEX);
	SetRasterizerState(GIcomponent->rasterizerState);
	SetViewport(GIcomponent->viewport);
	//Find a depthstencil to use
	//Set VS, PS, RTV, CB
	SetVertexShader(GIcomponent->vertexShader);
	SetPixelShader(GIcomponent->pixelShader);
	ClearDepthStencilView(GIcomponent->depthStencil);
	ClearRenderTargetView(GIcomponent->renderTargetView, 0.0f, 0.0f, 0.0f, 0.0f);
	SetRenderTargetViewAndDepthStencil(GIcomponent->renderTargetView, GIcomponent->depthStencil);
	SetConstantBuffer(GIcomponent->constantBuffer, SHADER_TO_BIND_RESOURCE::BIND_PIXEL);
	SetVertexBuffer(model->model.m_vertexBuffer);
	SetIndexBuffer(model->model.m_indexBuffer);
	//Update CB
	UpdateConstantBuffer(GIcomponent->constantBuffer, &GIcomponent->shaderData);

	//Render texture to RTV
	model->model.RenderAllSubmeshes();
	//Get texture data from RTV
	ID3D11Texture2D* RTVResource;
	GetTextureByType(RTVResource, TEXTURE_HOLDER_TYPE::RENDER_TARGET_VIEW, GIcomponent->renderTargetView);
	ID3D11Texture2D* stagingResource = nullptr;
	GetTextureByType(stagingResource, TEXTURE_HOLDER_TYPE::TEXTURE, GIcomponent->stagingTexture);
	d3d11Data->deviceContext->CopyResource(stagingResource, RTVResource);

	giCopyTexture* mappingTexture;
	mappingTexture = (giCopyTexture*)MemLib::spush(sizeof(giCopyTexture));

	D3D11_MAPPED_SUBRESOURCE mappedResource{0};
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(stagingResource, 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map staging resource!" << std::endl;
	}

	// Copy the new data to the buffer
	memcpy(mappingTexture, mappedResource.pData, mappedResource.DepthPitch);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(stagingResource, 0);

	//Another task: Save to a texture with custom size (int8 per pixel)

	for (size_t i = 0; i < TEXTURE_DIMENSIONS; i++)
	{
		for (size_t j = 0; j < TEXTURE_DIMENSIONS; j++)
		{
			GIcomponent->texture[i][j] = (int8_t)(mappingTexture->texture[i*2][j]).r;
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
	Camera::UpdateView();
	Camera::UpdateProjection();
	SetViewport(renderStates[backBufferRenderSlot].viewPort);
	//RTVResource->Release();
	//stagingResource->Release();
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
	GIcomponent->renderTargetView = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS::BIND_RENDER_TARGET,
		(CPU_FLAGS)0, TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS, FORMAT_R32_UINT);
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
	GIcomponent->vertexShader = LoadVertexShader("GICollisionVertex.cso", LAYOUT_DESC::DEFAULT);
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
	GIcomponent->stagingTexture = CreateTexture(FORMAT_R32_UINT,USAGE_FLAGS::USAGE_STAGING, (RESOURCE_FLAGS)0, CPU_FLAGS::READ, TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS);
	return GIcomponent->stagingTexture;
}


RS_IDX SetupGIRasterizerState(Registry& registry, EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->rasterizerState = CreateRasterizerState(false, true);
	return GIcomponent->rasterizerState;
}

RS_IDX SetupGIViewport(Registry& registry, EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->viewport = CreateViewport(TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS);
	return GIcomponent->viewport;
}


bool SetupGIAll(Registry& registry, EntityID& stage)
{
	PS_IDX giPS = SetupGIPixelShader(registry, stage);
	VS_IDX giVS = SetupGIVertexShader(registry, stage);
	CB_IDX giCB = SetupGIConstantBuffer(registry, stage);
	RTV_IDX giRTV = SetupGIRenderTargetView(registry, stage);
	TX_IDX giTX = SetupGIStagingTexture(registry, stage);
	DSV_IDX giDSV = SetupGIDepthStencil(registry, stage);
	SetupGIRasterizerState(registry, stage);
	SetupGIViewport(registry, stage);
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
