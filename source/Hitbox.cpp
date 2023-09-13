#include "Hitbox.h"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Backend/Collision.h"
#include "CollisionFunctions.h"

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
	unsigned availableSlot = FindAvailableSlot(collisionComponent->usedCirclesHitboxes);
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
	//Look at components to find what bit flags should be used
	SetCollisionEvent(registry, entity, (int)availableSlot, NoCollision );

	//Return the chosen slot for the user for further uses.
	return availableSlot;
}

void RemoveHitbox(Registry& registry, EntityID& entity, int hitboxID)
{
	//Set slot flag to 0
	unsigned mask = 0b1;//0b00000000 00000000 00000000 00000001
	// ob0011
	// ob0101
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);

	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		//Find slot flag
		hitbox->usedCirclesHitboxes = hitbox->usedCirclesHitboxes & (~(mask << hitboxID));
		//Set collision flags to 0
		hitbox->circularFlags[hitboxID].ResetToActive();
		hitbox->circularFlags[hitboxID].active = 0;
	}
	else
	{
		int convexSlot = hitboxID - SAME_TYPE_HITBOX_LIMIT;
		//Find slot flag
		hitbox->usedConvexHitboxes = hitbox->usedConvexHitboxes & (~(mask << (convexSlot)));
		//Set collision flags to 0
		hitbox->convexFlags[convexSlot].ResetToActive();
		hitbox->convexFlags[convexSlot].active = 0;
	}
}

int CreateHitbox(Registry& registry, EntityID& entity, int corners, float* cornerPosX, float* cornerPosZ, float offsetX, float offsetZ)
{
	if (corners < 3 && corners >= CONVEX_CORNER_LIMIT)
	{
		return -3;
	}
	//Check for the HitboxComponent, return -1 if failed
	HitboxComponent* collisionComponent = registry.GetComponent<HitboxComponent>(entity);
	if (collisionComponent == nullptr)
	{
		//Component did not exist!
		return -1;
	}
	//We now have the Collision Component, look for the first bit to add a hitbox on
	unsigned availableSlot = FindAvailableSlot(collisionComponent->usedConvexHitboxes);
	if (availableSlot < 0)
	{
		//No available bits, maximum size achieved
		return -2;
	}
	//There is now a slot for the hitbox, create it on the given slot.
	/*collisionComponent->circleHitbox[availableSlot].radius = radius;
	collisionComponent->circleHitbox[availableSlot].offsetX = offsetX;
	collisionComponent->circleHitbox[availableSlot].offsetZ = offsetZ;*/
	float sumX = 0.f;
	float sumZ = 0.f;
	for (int i = 0; i < corners; i++)
	{
		collisionComponent->convexHitbox[availableSlot].cornerX[i] = cornerPosX[i];
		sumX += cornerPosX[i];

		collisionComponent->convexHitbox[availableSlot].cornerZ[i] = cornerPosZ[i];
		sumZ += cornerPosZ[i];
	}
	collisionComponent->convexHitbox[availableSlot].cornerAmount = corners;
	//Calculate centroid
	collisionComponent->convexHitbox[availableSlot].centerX = offsetX + (sumX / (float)corners);
	collisionComponent->convexHitbox[availableSlot].centerZ = offsetZ + (sumZ / (float)corners);
	//Calculate boundingRadius
	//Get longest line from centroid
	float longestDistance = 0.f;
	for (int i = 0; i < corners; i++)
	{
		float dx, dz = 0.f;
		dx = cornerPosX[i] - collisionComponent->convexHitbox[availableSlot].centerX;
		dx = dx * dx;

		dz = cornerPosX[i] - collisionComponent->convexHitbox[availableSlot].centerZ;
		dz = dz * dz;

		float tempDist = std::sqrt(dx * dx + dz * dz);

		if (tempDist > longestDistance)
		{
			longestDistance = tempDist;
		}
	}
	collisionComponent->convexHitbox[availableSlot].boundingRadius = longestDistance;
	
	//Set normals
	float lineX, lineZ = 0.f;
	float scalar = 0.f;
	for (int i = 0; i < corners; i++)
	{
		//First get line
		lineX = cornerPosX[(i + 1) % corners] - cornerPosX[i];
		lineZ = cornerPosZ[(i + 1) % corners] - cornerPosZ[i];
		//Rotate 90 degrees for normal
		collisionComponent->convexHitbox[availableSlot].normalX[i] = -lineZ;
		collisionComponent->convexHitbox[availableSlot].normalZ[i] = lineX;
		//Get line from centroid to corner
		lineX = cornerPosX[i] - collisionComponent->convexHitbox[availableSlot].centerX;
		lineX = cornerPosZ[i] - collisionComponent->convexHitbox[availableSlot].centerZ;
		//Check if scalar is positive, if negative, reverse normal direction
		scalar = (lineX * collisionComponent->convexHitbox[availableSlot].normalX[i]) + (lineZ * collisionComponent->convexHitbox[availableSlot].normalZ[i]);
		if (scalar < 0.f)
		{
			collisionComponent->convexHitbox[availableSlot].normalX[i] *= -1.0f;
			collisionComponent->convexHitbox[availableSlot].normalZ[i] *= -1.0f;
		}
	}

	float line2X, line2Z = 0.f;
	float magnitudeX, magnitudeZ = 0.f;
	float radians = 0.f;
	bool reverse = false;
	//Check if convex
	for (int i = 0; i < corners; i++)
	{
		//First get line
		lineX = cornerPosX[(i + 1) % corners] - cornerPosX[i];
		lineZ = cornerPosZ[(i + 1) % corners] - cornerPosZ[i];

		//Second line
		line2X = cornerPosX[(i + 2) % corners] - cornerPosX[(i + 1) % corners];
		line2Z = cornerPosZ[(i + 2) % corners] - cornerPosZ[(i + 1) % corners];

		// dot product
		scalar = (lineX * line2X) + (lineZ * line2Z);

		//calculate magnitude of the lnes
		magnitudeX = std::sqrt(lineX * lineX + line2X * line2X);
		magnitudeZ = std::sqrt(lineZ * lineZ + line2Z * line2Z);

		//angle in radians
		radians = std::acos(scalar / (magnitudeX * magnitudeZ));

		if (i == 0 && radians > 3.14159265359f * 0.5f)
		{
			reverse = true;
		}
		if (reverse)
		{
			radians = 3.14159265359f - radians;
		}
		if (radians > 3.14159265359f * 0.5f)
		{
			//Concave shape, sadge
			RemoveHitbox(registry, entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
			return -4;
		}
	}

	//Set to active
	collisionComponent->convexFlags[availableSlot].ResetToActive();
	//Look at components to find what bit flags should be used
	SetCollisionEvent(registry, entity, (int)availableSlot + SAME_TYPE_HITBOX_LIMIT, NoCollision);

	//Return the chosen slot for the user for further uses.
	return availableSlot + SAME_TYPE_HITBOX_LIMIT;
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

void SetHitboxIsPlayer(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent *hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isPlayer = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID].isPlayer = setFlag;
	}
}

void SetHitboxIsEnemy(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isEnemy = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID].isEnemy = setFlag;
	}
}

void SetHitboxHitPlayer(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitPlayer = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID].hitPlayer = setFlag;
	}
}

void SetHitboxHitEnemy(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitEnemy = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID].hitEnemy = setFlag;
	}
}

void UpdatePhysics(Registry& registry)
{
	ResetCollisionVariables(registry);
	HandleMoveableCollision(registry);
	HandleDamageCollision(registry);
	HandleStaticCollision(registry);
}

void SetCollisionEvent(Registry& registry, EntityID& entity, int hitboxID, void* function)
{
	//Find hitbox
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->onCircleCollision[hitboxID].CollisionFunction = (void(*)(OnCollisionParameters&))function;
	}
	else
	{
		hitbox->onConvexCollision[hitboxID].CollisionFunction = (void(*)(OnCollisionParameters&))function;
	}
	
}
