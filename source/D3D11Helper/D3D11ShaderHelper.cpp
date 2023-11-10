#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "D3D11Helper/D3D11Helper.h"
#include "D3D11Helper/D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "GameRenderer.h"
#include <fstream>
#include <assert.h>

// Theese shaders exist to enable removing shaders from the pipeline
ID3D11VertexShader* vrtShader_NULL = nullptr;
ID3D11PixelShader* pixShader_NULL = nullptr;
ID3D11ComputeShader* comShader_NULL = nullptr;
ID3D11GeometryShader* geoShader_NULL = nullptr;
ID3D11InputLayout* inputLayout_NULL = nullptr;

PS_IDX LoadPixelShader(const char* name)//(ID3D11PixelShader* pixelShader)
{
	std::ifstream reader;
	reader.open(name, std::ios::binary | std::ios::ate);
	assert(true == reader.is_open());


	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	ID3D11PixelShader* tempPS = 0;
	HRESULT hr = d3d11Data->device->CreatePixelShader(shaderData, size, NULL, &tempPS);
	assert(!FAILED(hr));
	PS_IDX idx = pixHolder->NextIdx();

	pixHolder->ps_map.emplace(idx, tempPS);

	MemLib::spop(); // Free if it succeeded because it is destined to die
	return idx;
}

bool SetPixelShader(const PS_IDX idx)
{
	assert(true == pixHolder->ps_map.contains(idx));
	assert(idx >= 0);

	d3d11Data->deviceContext->PSSetShader(pixHolder->ps_map[idx], nullptr, 0);
	return true;
}

void UnsetPixelShader()
{
	d3d11Data->deviceContext->PSSetShader(pixShader_NULL, nullptr, 0);
}

// Does this string reference create more memory used on the stack?
bool CreateInputLayout(const char* vShaderByteCode, const unsigned int& size, ID3D11InputLayout*& il, const LAYOUT_DESC layout)
{
	struct Layout
	{
		D3D11_INPUT_ELEMENT_DESC* desc;
		UINT size;
	};

	//Default layout / Non skeletal layout
	D3D11_INPUT_ELEMENT_DESC defaultInputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	Layout defaultLayout = { defaultInputDesc, ARRAYSIZE(defaultInputDesc) };

	//Skeletal Layout
	D3D11_INPUT_ELEMENT_DESC skeletalInputDesc[5] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_WIEGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	Layout skeletalLayout = { skeletalInputDesc, ARRAYSIZE(skeletalInputDesc) };

	//Screen/UI Layout
	D3D11_INPUT_ELEMENT_DESC UIInputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	Layout screenLayout = { UIInputDesc, ARRAYSIZE(UIInputDesc) };

	D3D11_INPUT_ELEMENT_DESC ParticleInputDesc[6] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TIME", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"ROTATIONZ", 0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"RGB", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	Layout particleLayout = { ParticleInputDesc, ARRAYSIZE(ParticleInputDesc) };

	//Must correspond to enum layout in d3d11helper.h
	Layout layouts[] =
	{
		defaultLayout,
		skeletalLayout,
		screenLayout,
		particleLayout,
	};

	Layout chosen = layouts[layout];
	HRESULT hr = d3d11Data->device->CreateInputLayout(chosen.desc, chosen.size, vShaderByteCode, size, &il);

	assert(!FAILED(hr));

	return true;
}

VS_IDX LoadVertexShader(const char* name, LAYOUT_DESC layout)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	assert(true == reader.is_open());

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	ID3D11VertexShader* tempVS = 0;
	HRESULT hr = d3d11Data->device->CreateVertexShader(shaderData, size, NULL, &tempVS); // Does not increment here
	assert(!FAILED(hr));

	VS_IDX idx = vrtHolder->NextIdx();
	vrtHolder->vs_map.emplace(idx, tempVS);

	ID3D11InputLayout* tempIL = 0;
	// Try to create accompanying input layout
	bool inputLayoutCreated = CreateInputLayout(shaderData, size, tempIL, layout);
	assert(true == inputLayoutCreated);
	vrtHolder->il_map.emplace(idx, tempIL);


	// Free the temp memory
	MemLib::spop();

	// Return and increment (in that order)
	return idx;
}

bool SetVertexShader(const VS_IDX idx)
{
	assert(true == vrtHolder->vs_map.contains(idx));

	assert(idx >= 0);

	d3d11Data->deviceContext->VSSetShader(vrtHolder->vs_map[idx], nullptr, 0);
	d3d11Data->deviceContext->IASetInputLayout(vrtHolder->il_map[idx]);
	return true;
}

bool SetVertexShader(const VS_IDX idx, bool particle)
{
	assert(true == vrtHolder->vs_map.contains(idx));

	assert(idx >= 0);

	d3d11Data->deviceContext->VSSetShader(vrtHolder->vs_map[idx], nullptr, 0);
	d3d11Data->deviceContext->IASetInputLayout(inputLayout_NULL);
	return true;
}

void UnsetVertexShader()
{
	d3d11Data->deviceContext->VSSetShader(vrtShader_NULL, nullptr, 0);

}

CS_IDX LoadComputeShader(const char* name)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	assert(true == reader.is_open());

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	ID3D11ComputeShader* tempCS = 0;
	HRESULT hr = d3d11Data->device->CreateComputeShader(shaderData, size, NULL, &tempCS); // Does not increment here
	assert(!FAILED(hr));
	CS_IDX idx = comHolder->NextIdx();
	comHolder->cs_map.emplace(idx, tempCS);

	// Free the temp memory
	MemLib::spop();

	// Return and increment (in that order)
	return idx;
}

bool SetComputeShader(const CS_IDX idx)
{
	assert(true == comHolder->cs_map.contains(idx));
	assert(idx >= 0);

	d3d11Data->deviceContext->CSSetShader(comHolder->cs_map[idx], nullptr, 0);
	return true;
}

void UnsetComputeShader()
{
	d3d11Data->deviceContext->CSSetShader(comShader_NULL, nullptr, 0);
}

GS_IDX LoadGeometryShader(const char* name)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	assert(true == reader.is_open());

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	ID3D11GeometryShader* tempGS = 0;
	HRESULT hr = d3d11Data->device->CreateGeometryShader(shaderData, size, NULL, &tempGS); // Does not increment here
	assert(!FAILED(hr));
	GS_IDX idx = geoHolder->NextIdx();
	geoHolder->gs_map.emplace(idx, tempGS);

	// Free the temp memory
	MemLib::spop();

	// Return and increment (in that order)
	return idx;
}

bool SetGeometryShader(const GS_IDX idx)
{
	assert(true == geoHolder->gs_map.contains(idx));
	assert(idx >= 0);

	d3d11Data->deviceContext->GSSetShader(geoHolder->gs_map[idx], nullptr, 0);
	return true;
}

void UnsetGeometryShader()
{
	d3d11Data->deviceContext->GSSetShader(geoShader_NULL, nullptr, 0);
}

void SetTopology(TOPOLOGY topology)
{
	switch (topology)
	{
	case TRIANGLELIST:
		d3d11Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case POINTLIST:
		d3d11Data->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;
	default:
		break;
	}
}

bool DeleteD3D11PixelShader(const PS_IDX idx)
{
	assert(pixHolder->ps_map.contains(idx));
	pixHolder->ps_map[idx]->Release();
	pixHolder->ps_map.erase(idx);


	return true;
}

bool DeleteD3D11VertexShader(const VS_IDX idx)
{
	assert(vrtHolder->vs_map.contains(idx));
	vrtHolder->vs_map[idx]->Release();
	vrtHolder->vs_map.erase(idx);
	vrtHolder->il_map[idx]->Release();
	vrtHolder->il_map.erase(idx);
	return true;
}