#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include <iostream>
#include <DirectXMath.h>

ID3D11Buffer* bfr_NULL = NULL;

CB_IDX CreateConstantBuffer(const void* data, const size_t size, const uint8_t slot)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC; // Needs to be updated
	desc.ByteWidth = (UINT)size + (16 - ((UINT)size % 16));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Is a constant buffer
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU needs to be able to write
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA buffData;
	buffData.pSysMem = data;
	buffData.SysMemPitch = 0;
	buffData.SysMemSlicePitch = 0;

	uint16_t currentIdx = bfrHolder->currentCount;

	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &buffData, &bfrHolder->buff_arr[currentIdx]);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Constant Buffer!" << std::endl;
		return -1;
	}

	uint32_t* metadata = bfrHolder->metadata_arr[currentIdx];
	//metadata[0] = bindto; // Set shader to bind
	metadata[1] = slot; // Set slot to use
	metadata[2] = (uint32_t)size;


	return bfrHolder->currentCount++;
}


bool SetConstantBuffer(const CB_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto)
{
	ID3D11Buffer* setter = bfrHolder->buff_arr[idx];
	//SHADER_TO_BIND_RESOURCE whichShader = (SHADER_TO_BIND_RESOURCE)bfrHolder->metadata_arr[idx][0];
	uint8_t slot = bfrHolder->metadata_arr[idx][1];

	switch (bindto)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->VSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->HSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->DSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->GSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_COMPUTE:
		d3d11Data->deviceContext->CSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	default:
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

bool SetConstantBuffer(const CB_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto, bool particles)
{
	ID3D11Buffer* setter = bfrHolder->buff_arr[idx];
	//SHADER_TO_BIND_RESOURCE whichShader = (SHADER_TO_BIND_RESOURCE)bfrHolder->metadata_arr[idx][0];
	uint8_t slot = bfrHolder->metadata_arr[idx][1];

	switch (bindto)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->VSSetConstantBuffers(slot, 1, &bfr_NULL);
		d3d11Data->deviceContext->VSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->HSSetConstantBuffers(slot, 1, &bfr_NULL);
		d3d11Data->deviceContext->HSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->DSSetConstantBuffers(slot, 1, &bfr_NULL);
		d3d11Data->deviceContext->DSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->GSSetConstantBuffers(slot, 1, &bfr_NULL);
		d3d11Data->deviceContext->GSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetConstantBuffers(slot, 1, &bfr_NULL);
		d3d11Data->deviceContext->PSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	case BIND_COMPUTE:
		d3d11Data->deviceContext->CSSetConstantBuffers(slot, 1, &bfr_NULL);
		d3d11Data->deviceContext->CSSetConstantBuffers(slot, 1, &bfrHolder->buff_arr[idx]);
		break;
	default:
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}


bool UpdateConstantBuffer(const CB_IDX idx, const void* data)
{
	if (bfrHolder->currentCount < idx || idx < 0)
	{
		std::cerr << "Index for update Constant Buffer out of range!" << std::endl;
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(bfrHolder->buff_arr[idx], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map Constant Buffer!" << std::endl;
		return false;
	}

	// Copy the new data to the buffer
	memcpy(mappedResource.pData, data, bfrHolder->metadata_arr[idx][2]);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(bfrHolder->buff_arr[idx], 0);

	return true;
}



void SetWorldMatrix(float x, float y, float z, const SHADER_TO_BIND_RESOURCE& bindto)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto);
}

void SetWorldMatrix(float x, float y, float z, float rotationY, const SHADER_TO_BIND_RESOURCE& bindto)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = DirectX::XMMatrixRotationY(rotationY);
	world = world * DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto);
}

void SetWorldMatrix(float x, float y, float z, float dirX, float dirY, float dirZ, const SHADER_TO_BIND_RESOURCE& bindto)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR v = DirectX::XMVECTOR{ 0.0f, 0.0f,0.0f };
	DirectX::XMVECTOR f = DirectX::XMVECTOR{dirX, dirY, dirZ};
	DirectX::XMVECTOR up = DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f };
	world = DirectX::XMMatrixLookAtLH(v, f, up);
	world = world * DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto);
}

void SetWorldMatrix(float x, float y, float z, float dirX, float dirY, float dirZ, float scaleX, float scaleY, float ScaleZ, const SHADER_TO_BIND_RESOURCE& bindto)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR v = DirectX::XMVECTOR{ 0.0f, 0.0f,0.0f };
	DirectX::XMVECTOR f = DirectX::XMVECTOR{ dirX, dirY, dirZ };
	//DirectX::XMVECTOR s = DirectX::XMVECTOR{ scaleX, scaleY, ScaleZ};
	DirectX::XMVECTOR up = DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f };
	world = DirectX::XMMatrixScaling(scaleX, scaleY, ScaleZ);
	world = DirectX::XMMatrixLookAtLH(v, f, up);
	world = world * DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto);
}

void UpdateWorldMatrix(const void* data, const SHADER_TO_BIND_RESOURCE& bindto)
{
	static CB_IDX constantBufferIdx = -1;
	if (constantBufferIdx == -1)
	{
		DirectX::XMMATRIX emptyWorld;
		constantBufferIdx = CreateConstantBuffer(&emptyWorld, sizeof(emptyWorld), 0);
		SetConstantBuffer(constantBufferIdx, bindto);
	}
		
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(bfrHolder->buff_arr[constantBufferIdx], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map World Constant Buffer!" << std::endl;
		return;
	}

	// Copy the new data to the buffer
	memcpy(mappedResource.pData, data, bfrHolder->metadata_arr[constantBufferIdx][2]);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(bfrHolder->buff_arr[constantBufferIdx], 0);
}

VB_IDX CreateVertexBuffer(const void* data, const size_t& size, const size_t& count, const USAGE_FLAGS& useFlags)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = (D3D11_USAGE)useFlags; 
	desc.ByteWidth = (UINT)size * (UINT)count;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Is a vertex buffer
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA buffData;
	buffData.pSysMem = data;
	buffData.SysMemPitch = 0;
	buffData.SysMemSlicePitch = 0;

	uint16_t currentIdx = bfrHolder->currentCount;

	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &buffData, &bfrHolder->buff_arr[currentIdx]);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Vertex Buffer!" << std::endl;
		return -1;
	}

	bfrHolder->metadata_arr[currentIdx][0] = (uint32_t)size;
	bfrHolder->metadata_arr[currentIdx][1] = (uint32_t)count;

	return bfrHolder->currentCount++;
}

// Set an active constant buffer by index (shader and slot data contained in buffer)
bool SetVertexBuffer(const VB_IDX idx)
{
	if (bfrHolder->currentCount < idx || idx < 0)
	{
		std::cerr << "Index for Vertex Buffer out of range!" << std::endl;
		return false;
	}
	UINT offset = 0;
	d3d11Data->deviceContext->IASetVertexBuffers(0, 1, &(bfrHolder->buff_arr[idx]), &(bfrHolder->metadata_arr[idx][0]), &offset);
	return true;
}


// Create an Index Buffer with provided data and return a unique index to it
IB_IDX CreateIndexBuffer(const uint32_t* data, const size_t& size, const size_t& count)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = (UINT)(size * count) + (16 - ((UINT)(size * count) % 16)); // Will most likely not matter, since it takes list
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // Is an index buffer
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA buffData;
	buffData.pSysMem = data;
	buffData.SysMemPitch = 0;
	buffData.SysMemSlicePitch = 0;

	uint16_t currentIdx = bfrHolder->currentCount;

	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &buffData, &bfrHolder->buff_arr[currentIdx]);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Index Buffer!" << std::endl;
		return -1;
	}

	bfrHolder->metadata_arr[currentIdx][0] = (uint32_t)size;
	bfrHolder->metadata_arr[currentIdx][1] = (uint32_t)count;

	return bfrHolder->currentCount++;
}

// Set an active Index Buffer buffer by index
bool SetIndexBuffer(const IB_IDX idx)
{
	if (bfrHolder->currentCount < idx || idx < 0)
	{
		std::cerr << "Index for Index Buffer out of range!" << std::endl;
		return false;
	}

	UINT offset = 0;
	d3d11Data->deviceContext->IASetIndexBuffer(bfrHolder->buff_arr[idx], DXGI_FORMAT_R32_UINT, offset);
	return true;
}

