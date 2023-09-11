#include "Hitbox.h"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Backend/Collision.h"

HitboxVisualizeVariables hvv;

bool SetupHitboxVisualizer()
{
	InitializeBufferAndSRV();
	CreateShadersLayoutAndRasterState();
	return true;
}

int CreateHitbox(Registry& registry, EntityID& entity, float radius, float offsetX, float offsetZ)
{
	//Check for the HitboxComponent, return -1 if failed
	HitboxComponent* collisionComponent = registry.GetComponent<HitboxComponent>(entity);
	if (collisionComponent == nullptr)
	{
		//Component did not exist!
		return -1;
	}
	//We now have the Collision Component, look for the first bit to add a hitbox on
	unsigned availableSlot = FindAvailableSlot(collisionComponent->activeCirclesHitboxes);
	if (availableSlot < 0)
	{
		//No available bits, maximum size achieved
		return -2;
	}
	//There is now a slot for the hitbox, create it on the given slot.
	collisionComponent->circleHitbox[availableSlot].radius = radius;
	collisionComponent->circleHitbox[availableSlot].offsetX = offsetX;
	collisionComponent->circleHitbox[availableSlot].offsetZ = offsetZ;
	//Set to active
	collisionComponent->circularFlags[availableSlot].ResetToActive();
	//Return the chosen slot for the user for further uses.
	return availableSlot;
}

int CreateHitbox(Registry& registry, EntityID& entity, int corners, float* cornerPosX, float* cornerPosY, float offsetX, float offsetZ)
{
	return 0;
}

void AddHitboxComponent(Registry& registry, EntityID& entity)
{
	registry.AddComponent<HitboxComponent>(entity);
}

void CreateHitbox(int isCube, std::vector<DirectX::XMFLOAT3>& vertexBuffer) //Update this to work with ECS when creating components
{
	Hitbox hitbox;
	hitbox.isCube = isCube;

	float biggestX = 0.0f;
	float smallestX = 0.0f;
	float biggestY = 0.0f;
	float smallestY = 0.0f;
	float biggestZ = 0.0f;
	float smallestZ = 0.0f;

	for (int i = 0; i < (int)vertexBuffer.size(); i++)
	{
		//Take out the vertex with the largest and smallest X value.
		if (vertexBuffer[i].x > biggestX)
		{
			biggestX = vertexBuffer[i].x;
		}
		else if (vertexBuffer[i].x < smallestX)
		{
			smallestX = vertexBuffer[i].x;
		}

		//Take out the vertex with the largest and smallest Y value.
		if (vertexBuffer[i].y > biggestY)
		{
			biggestY = vertexBuffer[i].y;
		}
		else if (vertexBuffer[i].y < smallestY)
		{
			smallestY = vertexBuffer[i].y;
		}

		//Take out the vertex with the largest and smallest Z value.
		if (vertexBuffer[i].z > biggestZ)
		{
			biggestZ = vertexBuffer[i].z;
		}
		else if (vertexBuffer[i].z < smallestZ)
		{
			smallestZ = vertexBuffer[i].z;
		}
	}

	//Calculate the middle point of the mesh
	hitbox.middlePoint = DirectX::XMFLOAT3((biggestX + smallestX) / 2.0f, (biggestY + smallestY) / 2.0f, (biggestZ + smallestZ) / 2.0f);

	if (isCube)
	{
		//Create width, height, and depth
		hitbox.widthOrRadius = biggestX - smallestX;
		hitbox.height = biggestY - smallestY;
		hitbox.depth = biggestZ - smallestZ;
	}
	else
	{
		//Create radius for a sphere
		float largest = biggestX - smallestX;

		if (largest < (biggestY - smallestY))
		{
			largest = biggestY - smallestY;
		}

		if (largest < (biggestZ - smallestZ))
		{
			largest = biggestZ - smallestZ;
		}

		hitbox.widthOrRadius = largest;
	}

	//Push the new hitbox into the list of hitboxes
	hvv.hitboxes.push_back(hitbox);
}

void InitializeBufferAndSRV()
{
	//Create the structured buffer description
	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.ByteWidth = sizeof(Hitbox) * UINT(hvv.hitboxes.size());
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buffDesc.StructureByteStride = sizeof(Hitbox);

	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = &hvv.hitboxes[0];
	initialData.SysMemPitch = 0;
	initialData.SysMemSlicePitch = 0;

	//Create the SRV description
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc{};
	SRVdesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVdesc.Buffer.FirstElement = 0;
	SRVdesc.Buffer.NumElements = UINT(hvv.hitboxes.size());
	SRVdesc.Buffer.ElementOffset = 0;

	d3d11Data->device->CreateBuffer(&buffDesc, &initialData, &hvv.hitboxStructuredBuffer);
	if (hvv.hitboxStructuredBuffer != NULL)
	{
		d3d11Data->device->CreateShaderResourceView(hvv.hitboxStructuredBuffer, &SRVdesc, &hvv.hitboxStructuredSRV);
	}
}

void UpdateHitboxBuffer()
{
	//Clear previous buffer
	if (hvv.hitboxStructuredBuffer) hvv.hitboxStructuredBuffer->Release();
	if (hvv.hitboxStructuredSRV) hvv.hitboxStructuredSRV->Release();

	InitializeBufferAndSRV();
}

void DebugRenderHitbox(ID3D11Buffer*& worldMatrix, ID3D11Buffer*& viewAndProjectionMatrix)
{
	//Assuming the backbuffer is bound to the output merger
	
	//Clear the input assembly
	ID3D11Buffer* nullBuffer = nullptr;
	UINT stride = 0;
	UINT offset = 0;
	d3d11Data->deviceContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	d3d11Data->deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	d3d11Data->deviceContext->IASetInputLayout(nullptr);

	//Input assembler
	d3d11Data->deviceContext->IASetInputLayout(hvv.hitboxInputLayout);
	d3d11Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	d3d11Data->deviceContext->RSSetState(hvv.hitboxWireframeRaster);

	//Vertex Shader
	d3d11Data->deviceContext->VSSetShader(hvv.vShader, nullptr, 0);
	d3d11Data->deviceContext->VSSetShaderResources(0, 1, &hvv.hitboxStructuredSRV);
	d3d11Data->deviceContext->VSSetConstantBuffers(0, 1, &worldMatrix); //Constant Buffer Filled With World Matrix

	//Geometry Shader
	d3d11Data->deviceContext->GSSetShader(hvv.gShader, nullptr, 0);
	d3d11Data->deviceContext->GSSetConstantBuffers(0, 1, &viewAndProjectionMatrix);

	//Pixel Shader
	d3d11Data->deviceContext->PSSetShader(hvv.pShader, nullptr, 0);

	//Draw
	d3d11Data->deviceContext->Draw(UINT(hvv.hitboxes.size()), 0);
}

void CreateShadersLayoutAndRasterState()
{
	//Vertex Shader
	std::string shaderData;
	std::ifstream reader;
	reader.open("../bin/DebugHitboxVertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open VS file!" << std::endl;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(d3d11Data->device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &hvv.vShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
	}
	std::string vShaderByteCode = shaderData;

	//Geometry Shader
	shaderData.clear();
	reader.close();
	reader.open("../bin/DebugHitboxGeometryShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open GS file!" << std::endl;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(d3d11Data->device->CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, &hvv.gShader)))
	{
		std::cerr << "Failed to create geometry shader!" << std::endl;
	}

	//Pixel Shader
	shaderData.clear();
	reader.close();
	reader.open("../bin/DebugHitboxPixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open PS file!" << std::endl;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(d3d11Data->device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &hvv.pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
	}

	//Input Layout
	D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//Array of input descriptions, number of elements, pointer to compiled shader, size of compiled shader, pointer to input-layout.
	if (FAILED(d3d11Data->device->CreateInputLayout(inputDesc, 1, vShaderByteCode.c_str(), vShaderByteCode.length(), &hvv.hitboxInputLayout)))
	{
		std::cerr << "Failed to create hitbox input layout!" << std::endl;
	}

	//Raster State
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FrontCounterClockwise = false;

	if (FAILED(d3d11Data->device->CreateRasterizerState(&rasterDesc, &hvv.hitboxWireframeRaster)))
	{
		std::cerr << "Failed to create hitbox raster state!" << std::endl;
	}
}

void DestroyHitboxVisualizeVariables()
{
	//Release COM-objects
	if (hvv.vShader) hvv.vShader->Release();
	if (hvv.gShader) hvv.gShader->Release();
	if (hvv.pShader) hvv.pShader->Release();
	if (hvv.hitboxInputLayout) hvv.hitboxInputLayout->Release();
	if (hvv.hitboxWireframeRaster) hvv.hitboxWireframeRaster->Release();
	if (hvv.hitboxStructuredBuffer) hvv.hitboxStructuredBuffer->Release();
	if (hvv.hitboxStructuredSRV) hvv.hitboxStructuredSRV->Release();
	hvv.hitboxes.clear();
}
