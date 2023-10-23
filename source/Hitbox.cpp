#include "Hitbox.h"
#include "D3D11Graphics.h"
#include "D3D11Helper.h"
#include <string>
#include <fstream>
#include <sstream>
#include "Backend/Collision.h"
#include "CollisionFunctions.h"
#include "Backend\GeometryIndependent.h"
#include "Components.h"
#include "GameRenderer.h"
#include "Registry.h"
#include "EntityFramework.h"

HitboxVisualizeVariables hvv;

bool SetupHitboxVisualizer()
{
	InitializeBufferAndSRV();
	CreateShadersLayoutAndRasterState();
	return true;
}

int CreateHitbox(EntityID& entity, float radius, float offsetX, float offsetZ)
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
	//Set to mode
	collisionComponent->circularFlags[availableSlot].ResetToActive();
	//Look at components to find what bit flags should be used
	SetCollisionEvent(entity, (int)availableSlot, NoCollision );

	//Return the chosen slot for the user for further uses.
	return availableSlot;
}

void RemoveHitbox(EntityID& entity, int hitboxID)
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



int CreateHitbox (EntityID& entity, int corners, float cornerPosX[], float cornerPosZ[])
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
			RemoveHitbox(entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
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
			RemoveHitbox(entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
			return -5;
		}
		scalar = (middleToCorner2X * collisionComponent->convexHitbox[availableSlot].normalX[(i + 1) % corners]) + (middleToCorner2Z * collisionComponent->convexHitbox[availableSlot].normalZ[(i + 1) % corners]);
		if (scalar < 0.0f)
		{
			RemoveHitbox(entity, availableSlot + SAME_TYPE_HITBOX_LIMIT);
			return -5;
		}
	}

	//Set to mode
	collisionComponent->convexFlags[availableSlot].ResetToActive();
	//Look at components to find what bit flags should be used
	SetCollisionEvent(entity, (int)availableSlot + SAME_TYPE_HITBOX_LIMIT, NoCollision);

	//Return the chosen slot for the user for further uses.
	return availableSlot + SAME_TYPE_HITBOX_LIMIT;
}

void AddHitboxComponent(EntityID& entity)
{
	registry.AddComponent<HitboxComponent>(entity);
}

void CreateProximityHitbox(EntityID& entity, std::string fileName)
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

void AddProximityHitboxComponent(EntityID& entity)
{
	registry.AddComponent<ProximityHitboxComponent>(entity);
}

void InitializeBufferAndSRV()
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

void UpdateHitboxBuffer( )
{
	//Clear previous buffer
	if (hvv.hitboxStructuredBuffer) hvv.hitboxStructuredBuffer->Release();
	if (hvv.hitboxStructuredSRV) hvv.hitboxStructuredSRV->Release();

	InitializeBufferAndSRV();
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
	assert(reader.is_open());


	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	assert(!FAILED(d3d11Data->device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &hvv.vShader)));
	std::string vShaderByteCode = shaderData;

	//Geometry Shader
	shaderData.clear();
	reader.close();
	reader.open("../bin/DebugHitboxGeometryShader.cso", std::ios::binary | std::ios::ate);
	assert(reader.is_open());

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	assert(!FAILED(d3d11Data->device->CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, &hvv.gShader)));

	//Pixel Shader
	shaderData.clear();
	reader.close();
	reader.open("../bin/DebugHitboxPixelShader.cso", std::ios::binary | std::ios::ate);
	assert(reader.is_open());

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	assert(!FAILED(d3d11Data->device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &hvv.pShader)));

	//Input Layout
	D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//Array of input descriptions, number of elements, pointer to compiled shader, size of compiled shader, pointer to input-layout.
	assert(!FAILED(d3d11Data->device->CreateInputLayout(inputDesc, 1, vShaderByteCode.c_str(), vShaderByteCode.length(), &hvv.hitboxInputLayout)));

	//Raster State
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FrontCounterClockwise = false;

	assert(!FAILED(d3d11Data->device->CreateRasterizerState(&rasterDesc, &hvv.hitboxWireframeRaster)));
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

void SetHitboxActive(EntityID& entity, int hitboxID, bool setFlag)
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
	ResetAttackTrackerFlags(entity);
}

void SetHitboxIsStage(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxIsWall(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxIsPlayer(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxIsEnemy(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxIsStaticHazard(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxIsDynamicHazard(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxIsMoveable(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxCanTakeDamage(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].canTakeDamage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].canTakeDamage = setFlag;
	}
	ResetAttackTrackerFlags(entity);
}

void SetHitboxCanDealDamage(EntityID& entity, int hitboxID, bool setFlag)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circularFlags[hitboxID].canDealDamage = setFlag;
	}
	else
	{
		hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].canDealDamage = setFlag;
	}
	ResetAttackTrackerFlags(entity);
}

void SetHitboxHitStage(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxHitWall(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxHitPlayer(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxHitEnemy(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxHitStaticHazard(EntityID& entity, int hitboxID, bool setFlag)
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

void SetHitboxHitDynamicHazard(EntityID& entity, int hitboxID, bool setFlag)
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

void SetupEnemyCollisionBox(EntityID& entity, float radius, bool affectedByStaticHazards)
{
	AddHitboxComponent(entity);
	EnemyComponent* enemyComp = registry.GetComponent<EnemyComponent>(entity);
	int hID = CreateHitbox(entity, radius*0.5f, 0.f, 0.f);
	SetCollisionEvent(entity, hID, HardCollision);
	SetHitboxIsEnemy(entity, hID);
	SetHitboxHitPlayer(entity, hID);
	SetHitboxHitEnemy(entity, hID);
	SetHitboxActive(entity, hID);
	SetHitboxIsMoveable(entity, hID);
	SetHitboxHitWall(entity, hID);

	int sID = CreateHitbox(entity, radius, 0.f, 0.f);
	SetCollisionEvent(entity, sID, SoftCollision);
	SetHitboxIsEnemy(entity, sID);
	SetHitboxHitPlayer(entity, sID);
	//SetHitboxHitEnemy(entity, sID);
	SetHitboxActive(entity, sID);
	SetHitboxIsMoveable(entity, sID);
	SetHitboxHitStaticHazard(entity, sID, affectedByStaticHazards);
	SetHitboxCanTakeDamage(entity, sID);

	SetHitboxCanDealDamage(entity, sID, false);

	enemyComp->attackHitBoxID = CreateHitbox(entity, radius * 1.2f, 0.f, -1.5f);
	SetCollisionEvent(entity, enemyComp->attackHitBoxID, AttackCollision);
	//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
	SetHitboxHitPlayer(entity, enemyComp->attackHitBoxID);
	SetHitboxActive(entity, enemyComp->attackHitBoxID);
	SetHitboxIsMoveable(entity, enemyComp->attackHitBoxID);
	SetHitboxCanTakeDamage(entity, enemyComp->attackHitBoxID, false);
	SetHitboxCanDealDamage(entity, enemyComp->attackHitBoxID, false);
}

void SetupLavaCollisionBox(EntityID& entity, float radius)
{
	AddHitboxComponent(entity);
	int staticID = CreateHitbox(entity, radius, 0.f, 0.0f);
	SetCollisionEvent(entity, staticID, StaticHazardAttackCollision);
	//SetHitboxHitEnemy(entity, enemyComp->attackHitBoxID);
	SetHitboxHitPlayer(entity, staticID);
	SetHitboxActive(entity, staticID);
	//SetHitboxIsStaticHazard(entity, staticID);
	//SetHitboxCanTakeDamage(entity, staticID, false);
	SetHitboxCanDealDamage(entity, staticID, true);
}

void SetupPlayerCollisionBox(EntityID& entity, float radius)
{
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(entity);

	AddHitboxComponent(entity);

	int hID = CreateHitbox(entity, radius * 0.5f, 0.f, -0.f);
	SetCollisionEvent(entity, hID, HardCollision);
	SetHitboxIsPlayer(entity, hID);
	SetHitboxHitWall(entity, hID);
	SetHitboxHitEnemy(entity, hID);
	SetHitboxHitStage(entity, hID);
	SetHitboxActive(entity, hID);
	SetHitboxIsMoveable(entity, hID);
	SetHitboxHitStaticHazard(entity, hID, true);
	SetHitboxCanDealDamage(entity, hID, false);
	SetHitboxCanTakeDamage(entity, hID, true);

	/*float cornersX[] = { -0.2f, 0.2f, 0.2f, -0.2f };
	float cornersZ[] = { -4.0f, -4.0f, 2.0f, 2.0f };
	int hID = CreateHitbox(entity, 4, cornersX, cornersZ);
	SetCollisionEvent(entity, hID, HardCollision);
	SetHitboxIsPlayer(entity, hID);
	SetHitboxHitEnemy(entity, hID);
	SetHitboxHitStage(entity, hID);
	SetHitboxActive(entity, hID);
	SetHitboxIsMoveable(entity, hID);*/

	int sID = CreateHitbox(entity, radius * 0.75f, .0f, -0.0f);
	SetCollisionEvent(entity, sID, SoftCollision);
	SetHitboxIsPlayer(entity, sID, false);
	SetHitboxHitEnemy(entity, sID);
	SetHitboxActive(entity, sID);
	SetHitboxIsMoveable(entity, sID);
	SetHitboxCanDealDamage(entity, sID, false);
	SetHitboxCanTakeDamage(entity, sID, true);
	SetHitboxHitStaticHazard(entity, sID, true);
	playerComp->softHitboxID = sID;

	playerComp->attackHitboxID = CreateHitbox(entity, radius * 1.5f, 0.f, -1.5f);
	SetCollisionEvent(entity, playerComp->attackHitboxID, AttackCollision);
	SetHitboxHitEnemy(entity, playerComp->attackHitboxID);
	SetHitboxActive(entity, playerComp->attackHitboxID);
	SetHitboxCanTakeDamage(entity, playerComp->attackHitboxID, false);
	SetHitboxCanDealDamage(entity, playerComp->attackHitboxID, false);
}

bool HitboxCanHitGI(EntityID& entity)
{
	HitboxComponent* h = registry.GetComponent<HitboxComponent>(entity);
	if ((h->circularFlags[0].isMoveable && h->circularFlags[0].active) || (h->convexFlags[0].isMoveable && h->convexFlags[0].active))
	{
		return true;
	}
	return false;
}

void SetCollisionEvent(EntityID& entity, int hitboxID, void* function)
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

void ResetAttackTrackerFlags(EntityID& entity)
{
	//If hitboxID = -1 reset all trackers
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	for (size_t i = 0; i < HIT_TRACKER_LIMIT; i++)
	{
		hitbox->hitTracker[i].active = false;
	}
}


void SetupTestHitbox()
{
	//Put into scne
	EntityID player = registry.CreateEntity();
	AddHitboxComponent(player);
	int circle = CreateHitbox(  player, 1.0f, 15.0f, 15.0f);
	SetHitboxIsPlayer(  player, circle);
	SetHitboxHitEnemy(  player, circle);
	SetHitboxHitWall(  player, circle);
	float triangleX[3] = { 0.f, 1.f, 0.5f };
	float triangleZ[3] = { 0.f, 0.f, 1.f };
	int triangle = CreateHitbox(  player, 3, triangleX, triangleZ);
	SetHitboxIsPlayer(  player, triangle);
	SetHitboxHitEnemy(  player, triangle);

	float convexPentaX[5]{ 0.5f, 1.5f, 1.5f, 1.0f, 0.5f };
	float convexPentaZ[5]{ -0.5f, -0.5f, .5f, 1.f, .5f };

	EntityID enemy1 =  registry.CreateEntity();
	AddHitboxComponent(  enemy1);
	int circle2 = CreateHitbox(  enemy1, 1.0f, -11.0f, 1.0f);
	SetHitboxIsEnemy(  enemy1, circle2);
	SetHitboxHitPlayer(  enemy1, circle2);
	SetHitboxHitWall(  enemy1, circle2);
	int enemyConvex = CreateHitbox(  enemy1, 5, convexPentaX, convexPentaZ);
	SetHitboxIsEnemy(  enemy1, enemyConvex);
	SetHitboxHitPlayer(  enemy1, enemyConvex);

	EntityID wall =  registry.CreateEntity();
	AddProximityHitboxComponent(  wall);
	CreateProximityHitbox(  wall);
	//EntityID enemy2 =  registry.CreateEntity();
	//AddHitboxComponent( registry, enemy2);
	//int circle3 = CreateHitbox( registry, enemy2, 1.0f, 2.0f, 2.0f);
	//SetHitboxIsEnemy( registry, enemy2, circle3);
	//SetHitboxHitPlayer( registry, enemy2, circle3);

	/*EntityID stage =  registry.CreateEntity();
	AddGeometryIndependentComponent( registry, stage);
	GeometryIndependentColliderComponent* GeoIndie =  registry.GetComponent<GeometryIndependentColliderComponent>(stage);
	SetupGIAll( registry, stage);

	EntityID stageModel =  registry.CreateEntity();
	 registry.AddComponent<ModelBonelessComponent>(stageModel);
	ModelBonelessComponent* m =  registry.GetComponent<ModelBonelessComponent>(stageModel);
	m->model.Load("PlaceholderScene.mdl");
	RenderGeometryIndependentCollisionToTexture( registry, stage, stageModel);*/
	
	//UpdatePhysics( registry);
}

EntityID CreateAndRenderGeometryIndependentCollision(EntityID& m)
{
	EntityID stage = registry.CreateEntity();
	AddGeometryIndependentComponent(stage);
	GeometryIndependentColliderComponent* GeoIndie = registry.GetComponent<GeometryIndependentColliderComponent>(stage);

	RenderGeometryIndependentCollisionToTexture(stage);
	ReleaseGI(stage);
	return stage;
}

void RenderGeometryIndependentCollision(EntityID& m)
{
	AddGeometryIndependentComponent(m);
	GeometryIndependentColliderComponent* GeoIndie = registry.GetComponent<GeometryIndependentColliderComponent>(m);
	/*while (true)
	{
		RenderGeometryIndependentCollisionToTexture(m);
		Present();
	}*/
	RenderGeometryIndependentCollisionToTexture(m);
	ReleaseGI(m);
	return;
}
