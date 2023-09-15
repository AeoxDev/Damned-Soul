#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include <iostream>
#include <fstream>



PS_IDX LoadPixelShader(const char* name)//(ID3D11PixelShader* pixelShader)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	if (false == reader.is_open())
	{
		std::cerr << "Could not open PS test file!" << std::endl;
		return -1;
	}

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	HRESULT hr = d3d11Data->device->CreatePixelShader(shaderData, size, NULL, &pixHolder->ps_arr[pixHolder->currentCount]);
	if (FAILED(hr))
	{
		MemLib::spop(); // Free if failiure
		std::cerr << "Failed to create PS test shader!" << std::endl;
		return -1;
	}

	MemLib::spop(); // Free if it succeeded because it is destined to die
	return pixHolder->currentCount++;
}

bool SetPixelShader(const PS_IDX idx)
{
	if (pixHolder->currentCount < idx)
	{
		std::cerr << "Failed to set pixel shader: Index out of range!" << std::endl;
		return false;
	}
	else if (idx < 0)
	{
		std::cerr << "Failed to set pixel shader: Index must be 0 or greater!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->PSSetShader(pixHolder->ps_arr[idx], nullptr, 0);
	return true;
}

// Does this string reference create more memory used on the stack?
bool CreateInputLayout(const char* vShaderByteCode, const unsigned int& size, ID3D11InputLayout*& il, const bool skeletonLayout)
{
	D3D11_INPUT_ELEMENT_DESC nonSkeletalInputDesc[4] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"MATERIAL", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_INPUT_ELEMENT_DESC skeletalInputDesc[6] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"MATERIAL", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONE_WIEGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	D3D11_INPUT_ELEMENT_DESC* chosen = skeletonLayout ? skeletalInputDesc : nonSkeletalInputDesc;

	HRESULT hr = d3d11Data->device->CreateInputLayout(skeletalInputDesc, 4, vShaderByteCode, size, &il);

	if (FAILED(hr))
	{
		std::cerr << "Failed to create Input Layout!" << std::endl;
		return false;
	}

	return true;
}

VS_IDX LoadVertexShader(const char* name)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	if (false == reader.is_open())
	{
		std::cerr << "Could not open VS test file!" << std::endl;
		return -1;
	}

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	HRESULT hr = d3d11Data->device->CreateVertexShader(shaderData, size, NULL, &vrtHolder->vs_arr[vrtHolder->currentCount]); // Does not increment here
	if (FAILED(hr))
	{
		MemLib::spop(); // Pop if failiure
		std::cerr << "Failed to create Vertex Shader!" << std::endl;
		return -1;
	}

	// Try to create accompanying input layout
	if (false == CreateInputLayout(shaderData, size, vrtHolder->il_arr[vrtHolder->currentCount], false))
	{
		MemLib::spop(); // Pop if failiure
		vrtHolder->vs_arr[vrtHolder->currentCount]->Release(); // Release the newly created shader if the input layout failed
		std::cerr << "Failed to create Input Layout for Vertex Shader!" << std::endl;
		return -1;
	}

	// Free the temp memory
	MemLib::spop();

	// Return and increment (in that order)
	return vrtHolder->currentCount++;
}

bool SetVertexShader(const VS_IDX idx)
{
	if (vrtHolder->currentCount < idx)
	{
		std::cerr << "Failed to set vertex shader/input layout pair: Index out of range!" << std::endl;
		return false;
	}
	else if (idx < 0)
	{
		std::cerr << "Failed to set vertex shader/input layout pair: Index must be 0 or greater!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->VSSetShader(vrtHolder->vs_arr[idx], nullptr, 0);
	d3d11Data->deviceContext->IASetInputLayout(vrtHolder->il_arr[idx]);
	return true;
}


CS_IDX LoadComputeShader(const char* name)
{
	std::ifstream reader;

	reader.open(name, std::ios::binary | std::ios::ate);
	if (false == reader.is_open())
	{
		std::cerr << "Could not open CS test file!" << std::endl;
		return -1;
	}

	// Allocate the byte data on the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* shaderData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(shaderData, size);
	reader.close();

	HRESULT hr = d3d11Data->device->CreateComputeShader(shaderData, size, NULL, &comHolder->cs_arr[comHolder->currentCount]); // Does not increment here
	if (FAILED(hr))
	{
		MemLib::spop(); // Pop if failiure
		std::cerr << "Failed to create Compute Shader!" << std::endl;
		return -1;
	}

	// Free the temp memory
	MemLib::spop();

	// Return and increment (in that order)
	return comHolder->currentCount++;
}

bool SetComputeShader(const CS_IDX idx)
{
	if (comHolder->currentCount < idx)
	{
		std::cerr << "Failed to set compute shader: Index out of range!" << std::endl;
		return false;
	}
	else if (idx < 0)
	{
		std::cerr << "Failed to set compute shader: Index must be 0 or greater!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->CSSetShader(comHolder->cs_arr[idx], nullptr, 0);
	return true;
}