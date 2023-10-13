#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include <DirectXMath.h>
#include <assert.h>

ID3D11Buffer* bfr_NULL = nullptr;

CB_IDX CreateConstantBuffer(const void* data, const size_t size)
{
	uint16_t currentIdx = bfrHolder->NextIdx();

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
	assert(!FAILED(hr));

	size_t s = sizeof(uint16_t) + sizeof(ID3D11Buffer*);

	bfrHolder->buff_map.emplace(currentIdx, tempBuff);
	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return currentIdx;
}

CB_IDX CreateConstantBuffer(const size_t size)
{
	uint16_t currentIdx = bfrHolder->NextIdx();

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC; // Needs to be updated
	desc.ByteWidth = (UINT)size + (16 - ((UINT)size % 16));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Is a constant buffer
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU needs to be able to write
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, NULL, &tempBuff);
	assert(!FAILED(hr));
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);
	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return currentIdx;
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
		assert("ERROR"[0] == "Corrupt or incorrent Shader Type to bind!"[0]);
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

void UnsetConstantBuffer(const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
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
		assert("ERROR"[0] == "Corrupt or incorrent Shader Type to bind!"[0]);
		break; // Yes, this break is unnessecary, but it looks nice
	}
}

bool UpdateConstantBuffer(const CB_IDX idx, const void* data)
{
	assert(true == bfrHolder->buff_map.contains(idx));

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(bfrHolder->buff_map[idx], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(!FAILED(hr));

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
	assert(!FAILED(hr));

	// Copy the new data to the buffer
	memcpy(mappedResource.pData, data, bfrHolder->size[idx]);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(bfrHolder->buff_map[idx], 0);
}

void SetWorldMatrix(float x, float y, float z, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto, slot);
}

void SetWorldMatrix(float x, float y, float z, float rotationY, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = DirectX::XMMatrixRotationY(rotationY);
	world = world * DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto, slot);
}

void SetWorldMatrix(float x, float y, float z, float dirX, float dirY, float dirZ, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR v = DirectX::XMVECTOR{ 0.0f, 0.0f,0.0f };
	DirectX::XMVECTOR f = DirectX::XMVECTOR{ dirX, dirY, dirZ };
	DirectX::XMVECTOR up = DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f };
	world = DirectX::XMMatrixLookAtLH(v, f, up);
	world = world * DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto, slot);
}

void SetWorldMatrix(float x, float y, float z, float dirX, float dirY, float dirZ, float scaleX, float scaleY, float ScaleZ, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR v = DirectX::XMVECTOR{ 0.0f, 0.0f,0.0f };
	DirectX::XMVECTOR f = DirectX::XMVECTOR{ dirX, dirY, dirZ };
	//DirectX::XMVECTOR s = DirectX::XMVECTOR{ scaleX, scaleY, ScaleZ};
	DirectX::XMVECTOR up = DirectX::XMVECTOR{ 0.0f, 1.0f, 0.0f };
	world = DirectX::XMMatrixScaling(scaleX, scaleY, ScaleZ);
	world = world * DirectX::XMMatrixLookAtLH(v, f, up);
	world = world * DirectX::XMMatrixTranslation(x, y, z);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMFLOAT4X4 in;
	DirectX::XMStoreFloat4x4(&in, world);
	UpdateWorldMatrix(&in, bindto, 0);
}

SB_IDX CreateStructuredBuffer(const void* data, const size_t& size, const size_t& count, SRV_IDX& srvIdx)
{
	// Get the next index
	SB_IDX currentIdx = (SB_IDX)bfrHolder->NextIdx();

	// The description for the structured buffer
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = ((UINT)size * (UINT)count);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = size;

	D3D11_SUBRESOURCE_DATA sbData;
	sbData.pSysMem = data;
	sbData.SysMemPitch = 0;
	sbData.SysMemSlicePitch = 0;

	ID3D11Buffer* tempBuff;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &sbData, &tempBuff);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Structured Buffer!" << std::endl;
		srvIdx = -1;
		return -1;
	}

	D3D11_BUFFER_SRV buffSrv;
	buffSrv.FirstElement = 0;
	buffSrv.NumElements = (UINT)count;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer = buffSrv;
	SRV_IDX currentSrvIdx = srvHolder->NextIdx();
	ID3D11ShaderResourceView* tempSrv;
	hr = d3d11Data->device->CreateShaderResourceView(tempBuff, &srvDesc, &tempSrv);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create SRV for Structured Buffer!" << std::endl;
		tempBuff->Release();
		srvIdx = -1;
		return -1;
	}

	// Emplace buffer
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);
	bfrHolder->size.emplace(currentIdx, (uint32_t)size);
	bfrHolder->count.emplace(currentIdx, (uint32_t)count);

	// Emplace SRV
	srvHolder->srv_map.emplace(currentSrvIdx, tempSrv);
	srvHolder->size.emplace(currentSrvIdx, (uint32_t)count);

	// Return index, SRV is "returned" via parameter
	srvIdx = currentSrvIdx;
	return currentIdx;
}

bool UpdateStructuredBuffer(const SB_IDX idx, const void* data)
{
	if (false == bfrHolder->buff_map.contains(idx))
	{
		std::cerr << "Index for update Structured Buffer out of range!" << std::endl;
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ID3D11Buffer*& temp = bfrHolder->buff_map[idx];
	HRESULT hr = d3d11Data->deviceContext->Map(temp, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (FAILED(hr))
	{
		std::cerr << "Failed to map Structured Buffer!" << std::endl;
		return false;
	}
	std::memcpy(mappedSubresource.pData, data, bfrHolder->size[idx] * bfrHolder->count[idx]);
	d3d11Data->deviceContext->Unmap(temp, 0);
	return true;
}

void UpdateWorldMatrix(const void* data, const SHADER_TO_BIND_RESOURCE& bindto)
{
	static CB_IDX constantBufferIdx = -1;
	if (constantBufferIdx == -1)
	{
		DirectX::XMMATRIX emptyWorld;
		constantBufferIdx = CreateConstantBuffer(&emptyWorld, sizeof(emptyWorld));
		SetConstantBuffer(constantBufferIdx, bindto, 0);
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Map the buffer
	HRESULT hr = d3d11Data->deviceContext->Map(bfrHolder->buff_map[constantBufferIdx], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	assert(!FAILED(hr));

	// Copy the new data to the buffer
	memcpy(mappedResource.pData, data, bfrHolder->size[constantBufferIdx]);
	// Unmap the resource
	d3d11Data->deviceContext->Unmap(bfrHolder->buff_map[constantBufferIdx], 0);

}
VB_IDX CreateVertexBuffer(const void* data, const size_t& size, const size_t& count, const USAGE_FLAGS& useFlags)
{
	uint16_t currentIdx = bfrHolder->NextIdx();

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

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, &buffData, &tempBuff);
	assert(!FAILED(hr));
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);
	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return currentIdx;
}

VB_IDX CreateVertexBuffer(const size_t& size, const size_t& count, const USAGE_FLAGS& useFlags)
{
	uint16_t currentIdx = bfrHolder->NextIdx();

	D3D11_BUFFER_DESC desc;
	desc.Usage = (D3D11_USAGE)useFlags;
	desc.ByteWidth = (UINT)size * (UINT)count;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Is a vertex buffer
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&desc, NULL, &tempBuff);
	assert(!FAILED(hr));
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);
	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return currentIdx;
}

// Set an mode constant buffer by index (shader and slot data contained in buffer)
bool SetVertexBuffer(const VB_IDX idx)
{
	assert(true == bfrHolder->buff_map.contains(idx));
	UINT offset = 0;
	d3d11Data->deviceContext->IASetVertexBuffers(0, 1, &(bfrHolder->buff_map[idx]), &(bfrHolder->size[idx]), &offset);
	return true;
}

void UnsetVertexBuffer()
{
	d3d11Data->deviceContext->IASetVertexBuffers(0, 1, &bfr_NULL, 0, 0);
}


// Create an Index Buffer with provided data and return a unique index to it
IB_IDX CreateIndexBuffer(const uint32_t* data, const size_t& size, const size_t& count)
{
	uint16_t currentIdx = bfrHolder->NextIdx();

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
	assert(!FAILED(hr));
	bfrHolder->buff_map.emplace(currentIdx, tempBuff);

	bfrHolder->size.emplace(currentIdx, (uint32_t)size);

	return currentIdx;
}

// Set an mode Index Buffer buffer by index
bool SetIndexBuffer(const IB_IDX idx)
{
	assert(true == bfrHolder->buff_map.contains(idx));

	UINT offset = 0;
	d3d11Data->deviceContext->IASetIndexBuffer(bfrHolder->buff_map[idx], DXGI_FORMAT_R32_UINT, offset);
	return true;
}

void UnsetIndexBuffer()
{
	UINT offset = 0;
	d3d11Data->deviceContext->IASetIndexBuffer(bfr_NULL, DXGI_FORMAT_R32_UINT, offset);
}

bool DeleteD3D11Buffer(const CB_IDX idx)
{
	assert(bfrHolder->buff_map.contains(idx));

	bfrHolder->buff_map[idx]->Release();
	bfrHolder->buff_map.erase(idx);
	bfrHolder->size.erase(idx);
	if (bfrHolder->count.contains(idx))
		bfrHolder->count.erase(idx);

	return true;
}
