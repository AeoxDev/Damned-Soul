#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include <iostream>
#include <DirectXMath.h>

ID3D11Buffer* bfr_NULL = NULL;

CB_IDX CreateConstantBuffer(const void* data, const size_t size)
{
	uint8_t currentIdx = bfrHolder->_nextIdx;

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

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &buffData, &tempBuff);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Constant Buffer!" << std::endl;
		return -1;
	}
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);

	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return bfrHolder->_nextIdx++;
}

CB_IDX CreateConstantBuffer(const size_t size)
{
	uint8_t currentIdx = bfrHolder->_nextIdx;

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC; // Needs to be updated
	desc.ByteWidth = (UINT)size + (16 - ((UINT)size % 16));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Is a constant buffer
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU needs to be able to write
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, NULL, &tempBuff);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Constant Buffer!" << std::endl;
		return -1;
	}
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);

	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return bfrHolder->_nextIdx++;
}


bool SetConstantBuffer(const CB_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	switch (bindto)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->VSSetConstantBuffers(slot, 1, &bfrHolder->buff_map[idx]);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->HSSetConstantBuffers(slot, 1, &bfrHolder->buff_map[idx]);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->DSSetConstantBuffers(slot, 1, &bfrHolder->buff_map[idx]);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->GSSetConstantBuffers(slot, 1, &bfrHolder->buff_map[idx]);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetConstantBuffers(slot, 1, &bfrHolder->buff_map[idx]);
		break;
	case BIND_COMPUTE:
		d3d11Data->deviceContext->CSSetConstantBuffers(slot, 1, &bfrHolder->buff_map[idx]);
		break;
	default:
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

bool UnloadConstantBuffer(const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	switch (bindto)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->VSSetConstantBuffers(slot, 1, &bfr_NULL);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->HSSetConstantBuffers(slot, 1, &bfr_NULL);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->DSSetConstantBuffers(slot, 1, &bfr_NULL);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->GSSetConstantBuffers(slot, 1, &bfr_NULL);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetConstantBuffers(slot, 1, &bfr_NULL);
		break;
	case BIND_COMPUTE:
		d3d11Data->deviceContext->CSSetConstantBuffers(slot, 1, &bfr_NULL);
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
	if (bfrHolder->_nextIdx < idx || idx < 0)
	{
		std::cerr << "Index for update Constant Buffer out of range!" << std::endl;
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(bfrHolder->buff_map[idx], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map Constant Buffer!" << std::endl;
		return false;
	}

	// Copy the new data to the buffer
	memcpy(mappedResource.pData, data, bfrHolder->size[idx]);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(bfrHolder->buff_map[idx], 0);

	return true;
}


void UpdateWorldMatrix(const void* data, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	static CB_IDX idx = -1;
	if (idx == -1)
	{
		DirectX::XMMATRIX emptyWorld;
		idx = CreateConstantBuffer(&emptyWorld, sizeof(emptyWorld));
		SetConstantBuffer(idx, bindto, slot);
	}
		
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(bfrHolder->buff_map[idx], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map World Constant Buffer!" << std::endl;
		return;
	}

	// Copy the new data to the buffer
	memcpy(mappedResource.pData, data, bfrHolder->size[idx]);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(bfrHolder->buff_map[idx], 0);
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

	uint16_t currentIdx = bfrHolder->_nextIdx;

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &buffData, &tempBuff);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Vertex Buffer!" << std::endl;
		return -1;
	}
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);
	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return bfrHolder->_nextIdx++;
}

VB_IDX CreateVertexBuffer(const size_t& size, const size_t& count, const USAGE_FLAGS& useFlags)
{
	uint16_t currentIdx = bfrHolder->_nextIdx;

	D3D11_BUFFER_DESC desc;
	desc.Usage = (D3D11_USAGE)useFlags;
	desc.ByteWidth = (UINT)size * (UINT)count;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Is a vertex buffer
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, NULL, &tempBuff);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Vertex Buffer!" << std::endl;
		return -1;
	}
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);
	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return bfrHolder->_nextIdx++;
}

// Set an active constant buffer by index (shader and slot data contained in buffer)
bool SetVertexBuffer(const VB_IDX idx)
{
	if (bfrHolder->_nextIdx < idx || idx < 0)
	{
		std::cerr << "Index for Vertex Buffer out of range!" << std::endl;
		return false;
	}
	UINT offset = 0;
	d3d11Data->deviceContext->IASetVertexBuffers(0, 1, &(bfrHolder->buff_map[idx]), &(bfrHolder->size[idx]), &offset);
	return true;
}


// Create an Index Buffer with provided data and return a unique index to it
IB_IDX CreateIndexBuffer(const uint32_t* data, const size_t& size, const size_t& count)
{
	uint16_t currentIdx = bfrHolder->_nextIdx;

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

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &buffData, &tempBuff);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Index Buffer!" << std::endl;
		return -1;
	}
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);

	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return bfrHolder->_nextIdx++;
}

// Set an active Index Buffer buffer by index
bool SetIndexBuffer(const IB_IDX idx)
{
	if (bfrHolder->_nextIdx < idx || idx < 0)
	{
		std::cerr << "Index for Index Buffer out of range!" << std::endl;
		return false;
	}

	UINT offset = 0;
	d3d11Data->deviceContext->IASetIndexBuffer(bfrHolder->buff_map[idx], DXGI_FORMAT_R32_UINT, offset);
	return true;
}

