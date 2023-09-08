#include "Hitbox.h"

void CreateHitbox(ID3D11Device*& device, int isCube, std::vector<DirectX::XMFLOAT3>& vertexBuffer, HitboxList& list)
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
	list.hitboxes.push_back(hitbox);

	//Update the hitbox buffer and SRV
	DestroyBufferAndSRV(list);
	InitializeBufferAndSRV(device, list);
}

void InitializeBufferAndSRV(ID3D11Device*& device, HitboxList& list)
{
	//Create the structured buffer description
	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.ByteWidth = sizeof(Hitbox) * UINT(list.hitboxes.size());
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buffDesc.StructureByteStride = sizeof(Hitbox);

	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = &list.hitboxes[0];
	initialData.SysMemPitch = 0;
	initialData.SysMemSlicePitch = 0;

	//Create the SRV description
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc{};
	SRVdesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVdesc.Buffer.FirstElement = 0;
	SRVdesc.Buffer.NumElements = UINT(list.hitboxes.size());
	SRVdesc.Buffer.ElementOffset = 0;

	//Create the world matrix constant buffer description
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = sizeof(WorldMatrix);
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	//World matrix
	WorldMatrix world =
	{
		DirectX::XMMatrixIdentity()
	};

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = &world;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&constantBufferDesc, &initData, &list.worldMatrixBuffer)))
	{
		std::cerr << "Failed to create the world matrix constant buffer!" << std::endl;
	}

	device->CreateBuffer(&buffDesc, &initialData, &list.hitboxStructuredBuffer);
	if (list.hitboxStructuredBuffer != NULL)
	{
		device->CreateShaderResourceView(list.hitboxStructuredBuffer, &SRVdesc, &list.hitboxStructuredSRV);
	}


	//Delete everything in this function under this line later

	//Create the view And projection constant buffer description
	D3D11_BUFFER_DESC viewConstantBufferDesc{};
	viewConstantBufferDesc.ByteWidth = sizeof(WorldMatrix);
	viewConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	viewConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	viewConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	viewConstantBufferDesc.MiscFlags = 0;
	viewConstantBufferDesc.StructureByteStride = 0;

	float fovRadians = (90.0f / 360.0f) * DirectX::XM_2PI;
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, 2, 0.1f, 1000.0f);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);;
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(pos, target, up);

	//viewAndProj
	WorldMatrix viewAndProj =
	{
		viewMatrix * projectionMatrix
	};

	D3D11_SUBRESOURCE_DATA initoData{};
	initoData.pSysMem = &viewAndProj;
	initoData.SysMemPitch = 0;
	initoData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&viewConstantBufferDesc, &initoData, &list.viewAndProjMatrix)))
	{
		std::cerr << "Failed to create the viewAndProj matrix constant buffer!" << std::endl;
	}
}

void DebugRenderHitbox(ID3D11DeviceContext*& immediateContext, HitboxList& list)//, ID3D11Buffer*& viewAndProjectionMatrix)
{
	//Assuming the backbuffer is bound to the output merger
	
	//Clear the input assembly
	ID3D11Buffer* nullBuffer = nullptr;
	UINT stride = 0;
	UINT offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	immediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	immediateContext->IASetInputLayout(nullptr);

	//Input assembler
	immediateContext->IASetInputLayout(list.hitboxInputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//Vertex Shader
	immediateContext->VSSetShader(list.vShader, nullptr, 0);
	immediateContext->VSSetShaderResources(0, 1, &list.hitboxStructuredSRV);
	immediateContext->VSSetConstantBuffers(0, 1, &list.worldMatrixBuffer); //Constant Buffer Filled With World Matrix

	//Geometry Shader
	immediateContext->GSSetShader(list.gShader, nullptr, 0);
	immediateContext->GSSetConstantBuffers(0, 1, &list.viewAndProjMatrix); //Delete this later
	//immediateContext->GSSetConstantBuffers(0, 1, &viewAndProjectionMatrix); //Constant Buffer Filled With View And Projection Matrix

	//Pixel Shader
	immediateContext->PSSetShader(list.pShader, nullptr, 0);

	//Draw
	immediateContext->Draw(UINT(list.hitboxes.size()), 0);
}

void CreateShadersLayoutAndRasterState(ID3D11Device*& device, HitboxList& list)
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

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &list.vShader)))
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

	if (FAILED(device->CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, &list.gShader)))
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

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &list.pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
	}

	//Input Layout
	D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//Array of input descriptions, number of elements, pointer to compiled shader, size of compiled shader, pointer to input-layout.
	if (FAILED(device->CreateInputLayout(inputDesc, 1, vShaderByteCode.c_str(), vShaderByteCode.length(), &list.hitboxInputLayout)))
	{
		std::cerr << "Failed to create hitbox input layout!" << std::endl;
	}

	//Raster State
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FrontCounterClockwise = false;

	if (FAILED(device->CreateRasterizerState(&rasterDesc, &list.hitboxWireframeRaster)))
	{
		std::cerr << "Failed to create hitbox raster state!" << std::endl;
	}
}

void DestroyShaders(HitboxList& list)
{
	//Release COM-objects
	if (list.vShader) list.vShader->Release();
	if (list.gShader) list.gShader->Release();
	if (list.pShader) list.pShader->Release();
}

void DestroyHitboxResources(HitboxList& list)
{
	//Release COM-objects
	if (list.hitboxInputLayout) list.hitboxInputLayout->Release();
	if (list.hitboxWireframeRaster) list.hitboxWireframeRaster->Release();
}

void DestroyBufferAndSRV(HitboxList& list)
{
	//Release COM-objects
	if (list.hitboxStructuredBuffer) list.hitboxStructuredBuffer->Release();
	if (list.worldMatrixBuffer) list.worldMatrixBuffer->Release();
	if (list.hitboxStructuredSRV) list.hitboxStructuredSRV->Release();
}

void DestroyHitboxes(HitboxList& list)
{
	//Destroy everything associated with hitboxes
	DestroyShaders(list);
	DestroyHitboxResources(list);
	DestroyBufferAndSRV(list);
	list.hitboxes.clear();
}
