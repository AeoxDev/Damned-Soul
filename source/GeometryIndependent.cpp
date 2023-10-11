#include "Backend\GeometryIndependent.h"
#include "Camera.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "Components.h"
#include "GameRenderer.h"
#include "Registry.h"

#define TEXTURE_DIMENSIONS 128
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
	float width, height, offsetX, offsetZ;
	//TextureComponent
	int8_t texture[TEXTURE_DIMENSIONS][TEXTURE_DIMENSIONS];

	~GeometryIndependentComponent();
};
struct giPixel
{
	unsigned r, g, b, a;
};
struct giCopyTexture
{
	giPixel texture[TEXTURE_DIMENSIONS][TEXTURE_DIMENSIONS];
};

void RenderGeometryIndependentCollisionToTexture(EntityID& stageEntity)
{
	//Find GI component
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	ModelBonelessComponent* model = registry.GetComponent<ModelBonelessComponent>(stageEntity);
	//Find stage component

	//Look at model using orthographic camera

	//Set camera orthographic
	Camera::ToggleProjection();
	
	DirectX::XMVECTOR previousPos = Camera::GetPosition();
	DirectX::XMVECTOR previousLookAt = Camera::GetLookAt();
	DirectX::XMVECTOR previousUp = Camera::GetUp();
	DirectX::XMFLOAT3 vData;

	const VertexBoneless* vertices = LOADED_MODELS[model->model].m_data->GetBonelessVertices();

	unsigned nrVertices = LOADED_MODELS[model->model].m_data->m_numVertices;

	//Get the offset, facing direction and scale of the object
	//For now we ignore the facing direction for the vertex.
	float x = 0.0f, y = 0.0f, z = 0.0f, rX = 0.0f, rY = 0.0f, rZ = -1.0f, sX = 1.0f, sY = 1.0f, sZ = 1.0f;
	TransformComponent* transformComponent = registry.GetComponent<TransformComponent>(stageEntity);
	if (transformComponent != nullptr)
	{
		x = transformComponent->positionX;
		y = transformComponent->positionY;
		z = transformComponent->positionZ;
		rX = transformComponent->facingX;
		rY = transformComponent->facingY;
		rZ = transformComponent->facingZ;
		sX = transformComponent->scaleX;
		sY = transformComponent->scaleY;
		sZ = transformComponent->scaleZ;
	}

	float greatestX = -1000000000.0f;
	float greatestZ = -1000000000.0f;
	float greatestY = -1000000000.0f;
	float smallestX = 1000000000.0f;
	float smallestZ = 1000000000.0f;
	float smallestY = 1000000000.0f;

	for (unsigned i = 0; i < nrVertices; i++)
	{
		float xPos = sX * vertices[i].m_position[0] + x;
		if (xPos < smallestX)
		{
			smallestX = xPos;
		}
		if (xPos > greatestX)
		{
			greatestX = xPos;
		}
		float yPos = sY * vertices[i].m_position[1] + y;
		if (yPos < smallestY)
		{
			smallestY = yPos;
		}
		if (yPos > greatestY)
		{
			greatestY = yPos;
		}
		float zPos = sZ * vertices[i].m_position[2] + z;
		if (zPos < smallestZ)
		{
			smallestZ = zPos;
		}
		if (zPos > greatestZ)
		{
			greatestZ = zPos;
		}
	}
	GIcomponent->width = 10.0f + greatestX - smallestX;
	GIcomponent->height = 10.0f + greatestZ - smallestZ;
	GIcomponent->offsetX = 0.5f * (greatestX + smallestX);
	GIcomponent->offsetZ = 0.5f * (greatestZ + smallestZ);
	Camera::SetPosition(GIcomponent->offsetX,greatestY+ 2.0f, GIcomponent->offsetZ, false);//Set this to center of stage offset upwards
	Camera::SetLookAt(GIcomponent->offsetX, smallestY, GIcomponent->offsetZ);//Set to center of stage
	Camera::SetUp(0.0f, 0.0f, 1.0f);
	Camera::SetWidth(GIcomponent->width);//Set width (x) of orthogonal based on stage
	Camera::SetHeight(GIcomponent->height);//Set height (z) of orthogonal based on stage
	Camera::SetOrthographicDepth(greatestY - smallestY + 8.f);
	Camera::UpdateView();
	Camera::UpdateProjection();
	int16_t cameraIdx = Camera::GetCameraBufferIndex();
	SetWorldMatrix(x, y, z, rX, rY, -rZ, sX, sY, sZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
	SetConstantBuffer(cameraIdx, SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 1);
	SetRasterizerState(GIcomponent->rasterizerState);
	SetViewport(GIcomponent->viewport);
	//Find a depthstencil to use
	//Set VS, PS, RTV, CB
	SetVertexShader(GIcomponent->vertexShader);
	SetPixelShader(GIcomponent->pixelShader);
	ClearDepthStencilView(GIcomponent->depthStencil);
	ClearRenderTargetView(GIcomponent->renderTargetView, 0.0f, 0.0f, 0.0f, 0.0f);
	SetRenderTargetViewAndDepthStencil(GIcomponent->renderTargetView, GIcomponent->depthStencil);
	SetConstantBuffer(GIcomponent->constantBuffer, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 0);
	SetVertexBuffer(LOADED_MODELS[model->model].m_vertexBuffer);
	SetIndexBuffer(LOADED_MODELS[model->model].m_indexBuffer);
	

	//Update CB
	UpdateConstantBuffer(GIcomponent->constantBuffer, &GIcomponent->shaderData);

	//Render texture to RTV
	LOADED_MODELS[model->model].RenderAllSubmeshes();
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
			GIcomponent->texture[i][j] = (int8_t)(mappingTexture->texture[i][j]).r;
		}
	}


	bool succeeded = MemLib::spop();

	//Set back camera to previous mode
	Camera::ToggleProjection();
	DirectX::XMStoreFloat3(&vData, previousPos);
	Camera::SetPosition(vData.x, vData.y, vData.z, false);//Set this to center of stage offset upwards
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

bool AddGeometryIndependentComponent(EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent != NULL)
	{
		return true;
	}
	registry.AddComponent<GeometryIndependentComponent>(stageEntity);
	if (SetupGIAll(stageEntity) == false)
	{
		return false;
	}
	return true;
}

void ReleaseGI(EntityID& entity )
{
	//Get entity with UI, release components.
	GeometryIndependentComponent* gi = registry.GetComponent<GeometryIndependentComponent>(entity);
	//DeleteD3D11Viewport(gi->viewport);
	DeleteD3D11Buffer(gi->constantBuffer);
	DeleteD3D11Texture(gi->stagingTexture);
	DeleteD3D11RenderTargetView(gi->renderTargetView);
	DeleteD3D11PixelShader(gi->pixelShader);
	DeleteD3D11VertexShader(gi->vertexShader);
	DeleteD3D11DepthStencilView(gi->depthStencil);
	DeleteD3D11RasterizerState(gi->rasterizerState);
	//Release here
}

RTV_IDX SetupGIRenderTargetView(EntityID& stageEntity)
{
	//Get component
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a renderTargetView for the GI
	GIcomponent->renderTargetView = CreateRenderTargetView(USAGE_FLAGS::USAGE_DEFAULT, RESOURCE_FLAGS::BIND_RENDER_TARGET,
		(CPU_FLAGS)0, TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS, FORMAT_R32G32B32A32_UINT);
	return GIcomponent->renderTargetView;
}
DSV_IDX SetupGIDepthStencil(EntityID& stageEntity)
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

PS_IDX SetupGIPixelShader(EntityID& stageEntity)
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

VS_IDX SetupGIVertexShader(EntityID& stageEntity)
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

CB_IDX SetupGIConstantBuffer(EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->constantBuffer = CreateConstantBuffer(&GIcomponent->shaderData, sizeof(GIConstantBufferData));
	return GIcomponent->constantBuffer;
}

TX_IDX SetupGIStagingTexture(EntityID& stageEntity)
{
	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stageEntity);
	if (GIcomponent == NULL)
	{
		return -1;
	}
	//Create a pixelshader for the GI
	GIcomponent->stagingTexture = CreateTexture(FORMAT_R32G32B32A32_UINT,USAGE_FLAGS::USAGE_STAGING, (RESOURCE_FLAGS)0, CPU_FLAGS::READ, TEXTURE_DIMENSIONS, TEXTURE_DIMENSIONS);
	return GIcomponent->stagingTexture;
}


RS_IDX SetupGIRasterizerState(EntityID& stageEntity)
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

RS_IDX SetupGIViewport(EntityID& stageEntity)
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


bool SetupGIAll(EntityID& stage)
{
	PS_IDX giPS = SetupGIPixelShader(stage);
	VS_IDX giVS = SetupGIVertexShader(stage);
	CB_IDX giCB = SetupGIConstantBuffer(stage);
	RTV_IDX giRTV = SetupGIRenderTargetView(stage);
	TX_IDX giTX = SetupGIStagingTexture(stage);
	DSV_IDX giDSV = SetupGIDepthStencil(stage);
	SetupGIRasterizerState(stage);
	SetupGIViewport(stage);
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

int PixelValueOnPosition(GeometryIndependentComponent*& gi, float x, float z)
{
	//Get component for gi
	//Calculate size per pixel:
	float pixelX = gi->width / TEXTURE_DIMENSIONS;
	float pixelZ = gi->height / TEXTURE_DIMENSIONS;
	//Calculate offset:
	float offX = gi->width * 0.5f - gi->offsetX;
	float offZ = gi->height * 0.5f- gi->offsetZ;
	//Translate position to pixel using the size.
	float px = (x + offX) /pixelX;
	float pz = (-z + offZ) / pixelZ;
	//Check if pixel in bounds
	if (px < TEXTURE_DIMENSIONS && px >= 0)
	{
		if (pz < TEXTURE_DIMENSIONS && pz >= 0)
		{
			if (gi->texture[(int)pz][(int)px] == 0)
			{
				return gi->texture[(int)pz][(int)px];
			}
			return gi->texture[(int)pz][(int)px];
		}
	}
	return 0;
}