#include "Hitbox.h"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Backend/Collision.h"
#include "CollisionFunctions.h"
#include "Backend\GeometryIndependent.h"
#include "AllComponents.h"
#include "GameRenderer.h"

HitboxVisualizeVariables hvv;

bool SetupHitboxVisualizer(Registry& registry)
{
	InitializeBufferAndSRV(registry);
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



int CreateHitbox(Registry& registry, EntityID& entity, int corners, float cornerPosX[], float cornerPosZ[])
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
	collisionComponent->convexHitbox[availableSlot].centerX = (sumX / (float)corners);
	collisionComponent->convexHitbox[availableSlot].centerZ = (sumZ / (float)corners);
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
		lineZ = cornerPosZ[i] - collisionComponent->convexHitbox[availableSlot].centerZ;
		//Check if scalar is positive, if negative, reverse normal direction
		scalar = (lineX * collisionComponent->convexHitbox[availableSlot].normalX[i]) + (lineZ * collisionComponent->convexHitbox[availableSlot].normalZ[i]);
		if (scalar <= 0.f)
		{
			collisionComponent->convexHitbox[availableSlot].normalX[i] *= -1.0f;
			collisionComponent->convexHitbox[availableSlot].normalZ[i] *= -1.0f;
		}
		float magnitude = sqrt(collisionComponent->convexHitbox[availableSlot].normalX[i] * collisionComponent->convexHitbox[availableSlot].normalX[i] +
			collisionComponent->convexHitbox[availableSlot].normalZ[i] * collisionComponent->convexHitbox[availableSlot].normalZ[i]); // very logical line, yes.
		collisionComponent->convexHitbox[availableSlot].normalX[i] /= magnitude;
		collisionComponent->convexHitbox[availableSlot].normalZ[i] /= magnitude;
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
		magnitudeX = std::sqrt(lineX * lineX + lineZ * lineZ);
		magnitudeZ = std::sqrt(line2X * line2X + line2Z * line2Z);

		//angle in radians
		radians = std::acos(scalar / (magnitudeX * magnitudeZ));

		if (i == 0 && radians - 0.0001f> 3.14159265359f * 0.5f)
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
		//Check if middle point of neighboring points is on the inside
		//First get middle point
		float middleX = (cornerPosX[i] + cornerPosX[(i + 1) % corners] + cornerPosX[(i + 2) % corners]) * 0.333f;
		float middleZ = (cornerPosZ[i] +cornerPosZ[(i + 1) % corners] + cornerPosZ[(i + 2) % corners]) * 0.333f;
		float middleToCorner1X = cornerPosX[i] - middleX;
		float middleToCorner1Z = cornerPosZ[i] - middleZ;
		float middleToCorner2X = cornerPosX[(i + 2) % corners] - middleX;
		float middleToCorner2Z = cornerPosZ[(i + 2) % corners] - middleZ;
		//Then do scalar product to verify direction
		scalar = (middleToCorner1X * collisionComponent->convexHitbox[availableSlot].normalX[i]) + (middleToCorner1Z * collisionComponent->convexHitbox[availableSlot].normalZ[i]);
		if (scalar < 0.0f)
		{
			RemoveHitbox(registry, entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
			return -5;
		}
		scalar = (middleToCorner2X * collisionComponent->convexHitbox[availableSlot].normalX[(i + 1) % corners]) + (middleToCorner2Z * collisionComponent->convexHitbox[availableSlot].normalZ[(i + 1) % corners]);
		if (scalar < 0.0f)
		{
			RemoveHitbox(registry, entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
			return -5;
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

void CreateProximityHitbox(Registry& registry, EntityID& entity, std::string fileName)
{
	//Check if the proximityComponent already exists.
	ProximityHitboxComponent* proximityComponent = registry.GetComponent<ProximityHitboxComponent>(entity);
	if (proximityComponent == nullptr)
	{
		//Component did not exist!
		return;
	}

	//Read file and fill up the pointList and tell if clockwise or counter-clockwise
	std::string filePath = "HitboxFiles/" + fileName + ".box";
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return; //File not found
	}

	std::string line;
	ProximityPoint currentPoint;

	while (std::getline(file, line)) //For each line in the file
	{
		std::stringstream ss(line); //Load line into a stringstream

		std::string word;

		ss >> word; //Prefix

		if (word == "Clockwise") //Set clockwise variable to the value read from file
		{
			ss >> word;
			proximityComponent->clockwise = std::stoi(word);
		}
		else if (word == "Point") //Start of a new point, add index to ProximityPoint
		{
			ss >> word;
			currentPoint.index = std::stoi(word);
		}
		else if (word == "X") //X coordinate for the ProximityPoint
		{
			ss >> word;
			currentPoint.x = (float)(std::stoi(word));
		}
		else if (word == "Z") //Z coordinate for the ProximityPoint, also marks a completed point to add to component list.
		{
			ss >> word;
			currentPoint.z = (float)(std::stoi(word));

			proximityComponent->pointList.push_back(currentPoint);
		}
	}
}

void AddProximityHitboxComponent(Registry& registry, EntityID& entity)
{
	registry.AddComponent<ProximityHitboxComponent>(entity);
}

void InitializeBufferAndSRV(Registry& registry)
{
	hvv.hitboxes.clear();
	//Loop through the registry, find hitboxComponents, add to a struct and link to a buffer.
	for (auto entity : View<HitboxComponent>(registry))
	{
		HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);

		for (int i = 0; i < SAME_TYPE_HITBOX_LIMIT; i++)
		{
			if (hitbox->circularFlags[i].active != 0)
			{
				VisualHitbox vh;
				vh.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //Change color
				vh.middlePoint = DirectX::XMFLOAT3(hitbox->circleHitbox[i].offsetX, 0.0f, hitbox->circleHitbox[i].offsetZ);
				vh.radius = hitbox->circleHitbox[i].radius;
				hvv.hitboxes.push_back(vh);
			}
			else if (hitbox->convexFlags[i].active != 0)
			{
				VisualHitbox vh;
				vh.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //Change color
				vh.middlePoint = DirectX::XMFLOAT3(hitbox->convexHitbox[i].centerX, 0.0f, hitbox->convexHitbox[i].centerZ);
				vh.radius = hitbox->convexHitbox[i].boundingRadius;
				hvv.hitboxes.push_back(vh);
			}
			else
			{
				break;
			}
		}
	}

	//Create the structured buffer description
	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.ByteWidth = sizeof(VisualHitbox) * UINT(hvv.hitboxes.size());
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buffDesc.StructureByteStride = sizeof(VisualHitbox);

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

void UpdateHitboxBuffer(Registry& registry)
{
	//Clear previous buffer
	if (hvv.hitboxStructuredBuffer) hvv.hitboxStructuredBuffer->Release();
	if (hvv.hitboxStructuredSRV) hvv.hitboxStructuredSRV->Release();

	InitializeBufferAndSRV(registry);
}

void DebugRenderHitbox(ID3D11Buffer*& worldMatrix) //ID3D11Buffer*& viewAndProjectionMatrix)
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
	//d3d11Data->deviceContext->VSSetConstantBuffers(0, 1, &worldMatrix); //Constant Buffer Filled With World Matrix

	//Geometry Shader
	d3d11Data->deviceContext->GSSetShader(hvv.gShader, nullptr, 0);
	d3d11Data->deviceContext->GSSetConstantBuffers(0, 1, &worldMatrix); //Technically viewAndProjection
	//d3d11Data->deviceContext->GSSetConstantBuffers(0, 1, &viewAndProjectionMatrix);

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

void SetHitboxActive(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].active = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].active = setFlag;
	}
}

void SetHitboxIsStage(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isStage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isStage = setFlag;
	}
}

void SetHitboxIsWall(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isWall = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isWall = setFlag;
	}
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
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].isPlayer = setFlag;
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
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].isEnemy = setFlag;
	}
}

void SetHitboxIsStaticHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isStaticHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isStaticHazard = setFlag;
	}
}

void SetHitboxIsDynamicHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isDynamicHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isDynamicHazard = setFlag;
	}
}

void SetHitboxIsMoveable(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].isMoveable = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].isMoveable = setFlag;
	}
}

void SetHitboxHitStage(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitStage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitStage = setFlag;
	}
}

void SetHitboxHitWall(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitWall = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitWall = setFlag;
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
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].hitPlayer = setFlag;
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
		hitbox->convexFlags[hitboxID- SAME_TYPE_HITBOX_LIMIT].hitEnemy = setFlag;
	}
}

void SetHitboxHitStaticHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitStaticHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitStaticHazard = setFlag;
	}
}

void SetHitboxHitDynamicHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].hitDynamicHazard = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].hitDynamicHazard = setFlag;
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
		hitbox->onConvexCollision[hitboxID- SAME_TYPE_HITBOX_LIMIT].CollisionFunction = (void(*)(OnCollisionParameters&))function;
	}
	
}


void SetupTestHitbox()
{
	//Put into scne
	Registry collisionRegistry;
	EntityID player = collisionRegistry.CreateEntity();
	AddHitboxComponent(collisionRegistry, player);
	int circle = CreateHitbox(collisionRegistry, player, 1.0f, 15.0f, 15.0f);
	SetHitboxIsPlayer(collisionRegistry, player, circle);
	SetHitboxHitEnemy(collisionRegistry, player, circle);
	SetHitboxHitWall(collisionRegistry, player, circle);
	float triangleX[3] = { 0.f, 1.f, 0.5f };
	float triangleZ[3] = { 0.f, 0.f, 1.f };
	int triangle = CreateHitbox(collisionRegistry, player, 3, triangleX, triangleZ);
	SetHitboxIsPlayer(collisionRegistry, player, triangle);
	SetHitboxHitEnemy(collisionRegistry, player, triangle);

	float convexPentaX[5]{ 0.5f, 1.5f, 1.5f, 1.0f, 0.5f };
	float convexPentaZ[5]{ -0.5f, -0.5f, .5f, 1.f, .5f };

	EntityID enemy1 = collisionRegistry.CreateEntity();
	AddHitboxComponent(collisionRegistry, enemy1);
	int circle2 = CreateHitbox(collisionRegistry, enemy1, 1.0f, -11.0f, 1.0f);
	SetHitboxIsEnemy(collisionRegistry, enemy1, circle2);
	SetHitboxHitPlayer(collisionRegistry, enemy1, circle2);
	SetHitboxHitWall(collisionRegistry, enemy1, circle2);
	int enemyConvex = CreateHitbox(collisionRegistry, enemy1, 5, convexPentaX, convexPentaZ);
	SetHitboxIsEnemy(collisionRegistry, enemy1, enemyConvex);
	SetHitboxHitPlayer(collisionRegistry, enemy1, enemyConvex);

	EntityID wall = collisionRegistry.CreateEntity();
	AddProximityHitboxComponent(collisionRegistry, wall);
	CreateProximityHitbox(collisionRegistry, wall);
	//EntityID enemy2 = collisionRegistry.CreateEntity();
	//AddHitboxComponent(collisionRegistry, enemy2);
	//int circle3 = CreateHitbox(collisionRegistry, enemy2, 1.0f, 2.0f, 2.0f);
	//SetHitboxIsEnemy(collisionRegistry, enemy2, circle3);
	//SetHitboxHitPlayer(collisionRegistry, enemy2, circle3);

	EntityID stage = collisionRegistry.CreateEntity();
	AddGeometryIndependentComponent(collisionRegistry, stage);
	GeometryIndependentColliderComponent* GeoIndie = collisionRegistry.GetComponent<GeometryIndependentColliderComponent>(stage);
	SetupGIAll(collisionRegistry, stage);

	EntityID stageModel = collisionRegistry.CreateEntity();
	collisionRegistry.AddComponent<ModelComponent>(stageModel);
	ModelComponent* m = collisionRegistry.GetComponent<ModelComponent>(stageModel);
	m->model.Load("PlaceholderScene.mdl");

	RenderGeometryIndependentCollisionToTexture(collisionRegistry, stage, stageModel);
	//UpdatePhysics(collisionRegistry);
}