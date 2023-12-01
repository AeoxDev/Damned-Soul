#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "D3D11Helper/D3D11Helper.h"
#include "D3D11Helper/D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "UI/UIRenderer.h"
#include "SDLHandler.h"
#include <assert.h>


ID3D11View* vp_NULL = NULL;
ID3D11RenderTargetView* rtv_NULL = NULL;
ID3D11DepthStencilView* dsv_NULL = NULL;
ID3D11ShaderResourceView* srv_NULL = NULL;
ID3D11UnorderedAccessView* uav_NULL = NULL;
ID3D11RasterizerState* rs_NULL = NULL;


VP_IDX CreateViewport(const size_t& width, const size_t& height)
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	VP_IDX idx = vpHolder->NextIdx();
	vpHolder->vp_map.emplace(idx, vp);

	return idx;
}

void EditViewport(const VP_IDX idx, const size_t& width, const size_t& height)
{
	D3D11_VIEWPORT vp = vpHolder->vp_map[idx];
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
}

bool SetViewport(const VP_IDX idx)
{
	assert(true == vpHolder->vp_map.contains(idx));
	
	d3d11Data->deviceContext->RSSetViewports(1, &(vpHolder->vp_map[idx]));
	return true;
}


RTV_IDX CreateBackBuffer()
{
	uint8_t currentIdx = rtvHolder->NextIdx();

	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr = d3d11Data->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	assert(SUCCEEDED(hr));
	if (FAILED(hr))
	{
		return -1;
	}
	rtvHolder->tx_map.emplace(currentIdx, backBuffer);

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	ID3D11RenderTargetView* tempBB = 0;
	hr = d3d11Data->device->CreateRenderTargetView(backBuffer, NULL, &tempBB);

	assert(SUCCEEDED(hr) == true);
	if (FAILED(hr))
	{
		return -2;
	}
	rtvHolder->rtv_map.emplace(currentIdx, tempBB);
	//rtvHolder->tx_map.emplace(currentIdx, backBuffer);
	//backBuffer->Release();

	return currentIdx;
}

RTV_IDX CreateRenderTargetView(USAGE_FLAGS useFlags, RESOURCE_FLAGS bindFlags, CPU_FLAGS cpuAcess, const size_t& width, const size_t& height, FORMAT format)
{
	uint8_t currentIdx = rtvHolder->NextIdx();

	D3D11_TEXTURE2D_DESC desc = { 0 };
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	// Take the height and width of the loaded image and set it as the dimensions for the texture
	desc.Width = (UINT)width;
	desc.Height = (UINT)height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = (DXGI_FORMAT)format;//DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = (D3D11_USAGE)useFlags;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = cpuAcess;
	desc.MiscFlags = 0;

	ID3D11Texture2D* tempTex = 0;
	// Attempt to create a texture in the device
	HRESULT hr = d3d11Data->device->CreateTexture2D(&desc, NULL, &tempTex);
	assert(!FAILED(hr));
	rtvHolder->tx_map.emplace(currentIdx, tempTex);

	ID3D11RenderTargetView* tempRTV = 0;
	hr = d3d11Data->device->CreateRenderTargetView(rtvHolder->tx_map[currentIdx], NULL, &tempRTV);
	assert(!FAILED(hr));
	rtvHolder->rtv_map.emplace(currentIdx, tempRTV);

	// Set the hash last thing you do
	return currentIdx;

}

DSV_IDX CreateDepthStencil(const size_t& width, const size_t& height)
{
	uint8_t currentIdx = dsvHolder->NextIdx();


	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = (UINT)width;
	textureDesc.Height = (UINT)height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* tempTex = 0;
	HRESULT hr = d3d11Data->device->CreateTexture2D(&textureDesc, nullptr, &tempTex);
	assert(!FAILED(hr));
	
	dsvHolder->ds_map.emplace(currentIdx, tempTex);

	ID3D11DepthStencilView* tempDSV = 0;
	hr = d3d11Data->device->CreateDepthStencilView(dsvHolder->ds_map[currentIdx], 0, &tempDSV);
	assert(!FAILED(hr));
	dsvHolder->dsv_map.emplace(currentIdx, tempDSV);

	return currentIdx;
}

bool SetRenderTargetViewAndDepthStencil(const RTV_IDX idx_rtv, const DSV_IDX idx_dsv)
{
	assert(true == rtvHolder->rtv_map.contains(idx_rtv) && true == dsvHolder->dsv_map.contains(idx_dsv));

	d3d11Data->deviceContext->OMSetRenderTargets(1, &(rtvHolder->rtv_map[idx_rtv]), dsvHolder->dsv_map[idx_dsv]);
	return true;
}

void UnsetRenderTargetViewAndDepthStencil()
{
	d3d11Data->deviceContext->OMSetRenderTargets(1, &rtv_NULL, dsv_NULL);
}

SRV_IDX CreateShaderResourceViewBuffer(const void* data, const size_t& size, const int amount, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags)
{
	uint8_t currentIdx = srvHolder->NextIdx();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (UINT)size * amount;
	bufferDesc.CPUAccessFlags = CPUFlags;
	bufferDesc.BindFlags = resourceFlags;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;  // Hardcoded for particles, might be ok as we probably wont use SRV buffers for anything other than particles
	bufferDesc.StructureByteStride = (UINT)size;

	D3D11_SUBRESOURCE_DATA buffData;
	buffData.pSysMem = data;
	buffData.SysMemPitch = 0;
	buffData.SysMemSlicePitch = 0;

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&bufferDesc, &buffData, &tempBuff);
	assert(!FAILED(hr));

	ID3D11Resource* tempResource = 0;
	//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
	hr = tempBuff->QueryInterface(__uuidof(ID3D11Buffer), (void**)&tempResource);
	assert(!FAILED(hr));
	srvHolder->srv_resource_map.emplace(currentIdx, tempResource);


	//Now create the SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.FirstElement = 0;
	SRVDesc.Buffer.ElementOffset = 0;
	SRVDesc.Buffer.NumElements = amount;
	//SRVDesc.Buffer.ElementWidth = (UINT)size;

	ID3D11ShaderResourceView* tempSRV = 0;
	hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], &SRVDesc, &tempSRV);
	assert(!FAILED(hr));

	srvHolder->srv_map.emplace(currentIdx, tempSRV);
	srvHolder->size.emplace(currentIdx, (uint32_t)size);

	// Release tempBuff as the queryInterface is the resource appended
	tempBuff->Release();

	return currentIdx;
}

SRV_IDX CreateShaderResourceViewTexture(const RESOURCES& resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const size_t& width, const size_t& height)
{
	HRESULT hr;
	uint8_t currentIdx = srvHolder->NextIdx();
	ID3D11Texture2D* tempTex = 0;
	ID3D11Resource* tempResource = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	//This switch firstly, defines the and creates the resource that the SRV is to use
	//To then define and create the SRV that will be holding the resource
	switch (resource)
	{
		case RESOURCE_TEXTURE1D:
			//Define if needed
			break;
		case RESOURCE_TEXTURE1DARRAY:
			//Define if needed
			break;
		case RESOURCE_TEXTURE2D:
			D3D11_TEXTURE2D_DESC textureDesc;
			textureDesc.Width = (UINT)width;
			textureDesc.Height = (UINT)height;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = resourceFlags;
			textureDesc.CPUAccessFlags = CPUFlags;
			textureDesc.MiscFlags = 0;


			hr = d3d11Data->device->CreateTexture2D(&textureDesc, nullptr, &tempTex);
			assert(!FAILED(hr));


			//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
			hr = tempTex->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tempResource);
			assert(!FAILED(hr));
			srvHolder->srv_resource_map.emplace(currentIdx, tempResource);

			SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

			// Release tempTex as the queryInterface is the resource appended
			tempTex->Release();
			break;
		case RESOURCE_TEXTURE2DARRAY:
			//Define if needed
			break;
		case RESOURCE_TEXTURE3D:
			//Define if needed
			break;
		case RESOURCE_TEXCUBE:
			//Define if needed
			break;
		default:
			assert("Did not create requested Shader Resource View, requested case is not defined"[0] == "ERROR"[0]);
			return false;
			break;
	}

	assert(tempResource != nullptr);

	ID3D11ShaderResourceView* tempSRV = 0;
	hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], &SRVDesc, &tempSRV);
	assert(!FAILED(hr));
	srvHolder->srv_map.emplace(currentIdx, tempSRV);

	//Get the size of the texture by mapping and saving the DepthPitch
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//d3d11Data->deviceContext->Map(srvHolder->srv_resource_map[currentIdx], 0, D3D11_MAP_WRITE, 0, &mappedResource);
	//srvHolder->size.emplace(currentIdx, mappedResource.DepthPitch);
	//d3d11Data->deviceContext->Unmap(srvHolder->srv_resource_map[currentIdx], 0);


	return currentIdx;
}

SRV_IDX CreateShaderResourceViewTexture(const int8_t sourceIdx, RESOURCE_FLAGS sourceResource)
{
	uint8_t currentIdx = srvHolder->NextIdx();
	HRESULT hr = NULL;
	ID3D11ShaderResourceView* tempSRV = 0;
	ID3D11Texture2D* tempTex = 0;
	ID3D11Resource* tempResource = 0;


	switch (sourceResource)
	{
	case BIND_RENDER_TARGET:
		tempTex = rtvHolder->tx_map[sourceIdx];
		hr = tempTex->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tempResource);
		if (FAILED(hr))
		{
			assert("ERRPR"[0] == "Failed to create Buffer to be used for Unordered Access View!"[0]);
			return false;
		}

		srvHolder->srv_resource_map.emplace(currentIdx, tempResource);

		hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], NULL, &tempSRV);
		assert(!FAILED(hr));
		srvHolder->srv_map.emplace(currentIdx, tempSRV);

		//Get the size of the texture by mapping and saving the DepthPitch
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//d3d11Data->deviceContext->Map(srvHolder->srv_resource_map[currentIdx], 0, D3D11_MAP_WRITE, 0, &mappedResource);
		//srvHolder->size.emplace(currentIdx, mappedResource.DepthPitch);
		//d3d11Data->deviceContext->Unmap(srvHolder->srv_resource_map[currentIdx], 0);
		break;
	case BIND_SHADER_RESOURCE:
		tempResource = srvHolder->srv_resource_map[sourceIdx];
		srvHolder->srv_resource_map.emplace(currentIdx, tempResource);
		srvHolder->srv_resource_map[currentIdx]->AddRef();


		hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], NULL, &tempSRV);
		assert(FAILED(hr));
		srvHolder->srv_map.emplace(currentIdx, tempSRV);
		srvHolder->size.emplace(currentIdx, srvHolder->size[sourceIdx]);
		break;
	default:
		assert("Did not create requested Shader Resource View (overload), requested case is not defined"[0] == "ERROR"[0]);
		return false;
		break;
	}

	return currentIdx;
}

bool SetShaderResourceView(const SRV_IDX idx, const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	switch (bindto)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->VSSetShaderResources(slot, 1, &srvHolder->srv_map[idx]);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->HSSetShaderResources(slot, 1, &srvHolder->srv_map[idx]);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->DSSetShaderResources(slot, 1, &srvHolder->srv_map[idx]);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->GSSetShaderResources(slot, 1, &srvHolder->srv_map[idx]);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &srvHolder->srv_map[idx]);
		break;
	case BIND_COMPUTE:
		d3d11Data->deviceContext->CSSetShaderResources(slot, 1, &srvHolder->srv_map[idx]);
		break;
	default:
		assert("ERROR"[0] == "Corrupt or incorrent Shader Type to bind!"[0]);
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

void UnsetShaderResourceView(const SHADER_TO_BIND_RESOURCE& bindto, uint8_t slot)
{
	switch (bindto)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->VSSetShaderResources(slot, 1, &srv_NULL);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->HSSetShaderResources(slot, 1, &srv_NULL);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->DSSetShaderResources(slot, 1, &srv_NULL);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->GSSetShaderResources(slot, 1, &srv_NULL);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &srv_NULL);
		break;
	case BIND_COMPUTE:
		d3d11Data->deviceContext->CSSetShaderResources(slot, 1, &srv_NULL);
		break;
	default:
		assert("ERROR"[0] == "Corrupt or incorrent Shader Type to bind!"[0]);
		break;
	}

}

bool DeleteD3D11SRV(const SRV_IDX idx)
{
	assert(srvHolder->srv_map.contains(idx));
	// Release SRV
	srvHolder->srv_map[idx]->Release();
	srvHolder->srv_map.erase(idx);

	srvHolder->size.erase(idx); // Size

	// Release resource if it exists
	if (srvHolder->srv_resource_map.contains(idx))
	{
		srvHolder->srv_resource_map[idx]->Release();
		srvHolder->srv_resource_map.erase(idx);
	}

	return true;
}

void CopyToVertexBuffer(const CB_IDX destination, const SRV_IDX source)
{
	d3d11Data->deviceContext->CopyResource(bfrHolder->buff_map[destination], uavHolder->uav_resource_map[source]);
}

void CopySRVtoSRV(const SRV_IDX destination, const SRV_IDX source)
{
	d3d11Data->deviceContext->CopyResource(srvHolder->srv_resource_map[destination], srvHolder->srv_resource_map[source]);
}

void CopySRVToUAV(const UAV_IDX destination, const SRV_IDX source)
{
	d3d11Data->deviceContext->CopyResource(uavHolder->uav_resource_map[destination], srvHolder->srv_resource_map[source]);
}

void CopyUAVToSRV(const SRV_IDX destination, const UAV_IDX source)
{
	d3d11Data->deviceContext->CopyResource(srvHolder->srv_resource_map[destination], uavHolder->uav_resource_map[source]);

}

SRV_IDX CreateUnorderedAccessViewBuffer(const void* data, const size_t& size, const int amount, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags)
{
	uint8_t currentIdx = uavHolder->NextIdx();

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (UINT)size * amount;
	bufferDesc.CPUAccessFlags = CPUFlags;
	bufferDesc.BindFlags = resourceFlags; //D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;  // Hardcoded for particles, might be ok as we probably wont use UAV buffers for anything other than particles
	bufferDesc.StructureByteStride = (UINT)size;

	D3D11_SUBRESOURCE_DATA buffData;
	buffData.pSysMem = data;
	buffData.SysMemPitch = 0;
	buffData.SysMemSlicePitch = 0;

	ID3D11Buffer* tempBuff = 0;
	HRESULT hr = d3d11Data->device->CreateBuffer(&bufferDesc, &buffData, &tempBuff);
	if (FAILED(hr))
	{
		assert("ERRPR"[0] == "Failed to create Buffer to be used for Unordered Access View!"[0]);
		return false;
	}

	ID3D11Resource* tempResource = 0;
	//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
	hr = tempBuff->QueryInterface(__uuidof(ID3D11Buffer), (void**)&tempResource);
	assert(!FAILED(hr));
	uavHolder->uav_resource_map.emplace(currentIdx, tempResource);


	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = amount;

	ID3D11UnorderedAccessView* tempUAV = 0;
	hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_map[currentIdx], &UAVDesc, &tempUAV);
	assert(!FAILED(hr));

	uavHolder->uav_map.emplace(currentIdx, tempUAV);
	uavHolder->size.emplace(currentIdx, (uint32_t)size);

	tempBuff->Release();
	return currentIdx;
}

SRV_IDX CreateUnorderedAccessViewBuffer(const size_t& size, const int amount, const int16_t idx)
{
	uint8_t currentIdx = uavHolder->NextIdx();

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = amount;

	ID3D11Resource* tempResource = srvHolder->srv_resource_map[(int8_t)idx];
	uavHolder->uav_resource_map.emplace(currentIdx, tempResource);
	uavHolder->uav_resource_map[currentIdx]->AddRef();


	ID3D11UnorderedAccessView* tempUAV = 0;
	HRESULT hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_map[currentIdx], &UAVDesc, &tempUAV);
	assert(!FAILED(hr));
	
	uavHolder->uav_map.emplace(currentIdx, tempUAV);
	uavHolder->size.emplace(currentIdx, (uint32_t)size);


	return currentIdx;
}

SRV_IDX CreateUnorderedAccessViewTexture(const size_t& width, const size_t& height)
{
	uint8_t currentIdx = uavHolder->NextIdx();

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = (UINT)width;
	texDesc.Height = (UINT)height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* tempTex = 0;
	HRESULT hr = d3d11Data->device->CreateTexture2D(&texDesc, nullptr, &tempTex);
	assert(!FAILED(hr));

	ID3D11Resource* tempResource = 0;
	//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
	hr = tempTex->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tempResource);
	assert(!FAILED(hr));
	uavHolder->uav_resource_map.emplace(currentIdx, tempResource);


	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Texture2D.MipSlice = 0;

	ID3D11UnorderedAccessView* tempUAV = 0;
	hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_map[currentIdx], &UAVDesc, &tempUAV);
	assert(!FAILED(hr));

	uavHolder->uav_map.emplace(currentIdx, tempUAV);
	//uavHolder->size.emplace(currentIdx, (uint32_t)size);

	tempTex->Release();
	return currentIdx;
}

SRV_IDX CreateUnorderedAccessViewTexture(const size_t& width, const size_t& height, const int16_t idx)
{
	uint8_t currentIdx = uavHolder->NextIdx();

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Texture2D.MipSlice = 0;


	ID3D11Resource* tempResource = rtvHolder->tx_map[(int8_t)idx];
	uavHolder->uav_resource_map.emplace(currentIdx, tempResource);
	uavHolder->uav_resource_map[currentIdx]->AddRef();


	ID3D11UnorderedAccessView* tempUAV = 0;
	HRESULT hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_map[currentIdx], &UAVDesc, &tempUAV);
	assert(!FAILED(hr));

	uavHolder->uav_map.emplace(currentIdx, tempUAV);
	//uavHolder->size.emplace(currentIdx, (uint32_t)size);

	//uint8_t currentIdx = uavHolder->NextIdx();

	//D3D11_TEXTURE2D_DESC texDesc;
	//texDesc.Width = width;
	//texDesc.Height = height;
	//texDesc.MipLevels = 1;
	//texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//texDesc.SampleDesc.Count = 1;
	//texDesc.SampleDesc.Quality = 0;
	//texDesc.Usage = D3D11_USAGE_DEFAULT;
	//texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	//texDesc.CPUAccessFlags = 0;
	//texDesc.MiscFlags = 0;

	//ID3D11Texture2D* tempTex = 0;
	//HRESULT hr = d3d11Data->device->CreateTexture2D(&texDesc, nullptr, &tempTex);
	//assert(!FAILED(hr));

	//ID3D11Resource* tempResource = 0;
	////QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
	//hr = tempTex->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tempResource);
	//assert(!FAILED(hr));
	//uavHolder->uav_resource_map.emplace(currentIdx, tempResource);


	//D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	//UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	//UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	//UAVDesc.Texture2D.MipSlice = 0;

	//ID3D11UnorderedAccessView* tempUAV = 0;
	//hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_map[currentIdx], &UAVDesc, &tempUAV);
	//assert(!FAILED(hr));

	//uavHolder->uav_map.emplace(currentIdx, tempUAV);
	////uavHolder->size.emplace(currentIdx, (uint32_t)size);
	//tempTex->Release();



	return currentIdx;
}

bool SetUnorderedAcessView(const UAV_IDX idx, uint8_t slot)
{
	d3d11Data->deviceContext->CSSetUnorderedAccessViews(slot, 1, &uavHolder->uav_map[idx], NULL);

	return true;
}

void UnsetUnorderedAcessView(uint8_t slot)
{
	d3d11Data->deviceContext->CSSetUnorderedAccessViews(slot, 1, &uav_NULL, NULL);
}

void ClearRenderTargetView(const RTV_IDX idx, float r, float g, float b, float a)
{
	float color[4] = { r, g, b, a };
	d3d11Data->deviceContext->ClearRenderTargetView(rtvHolder->rtv_map[idx], color);
}

void ClearDepthStencilView(const DSV_IDX idx)
{
	d3d11Data->deviceContext->ClearDepthStencilView(dsvHolder->dsv_map[idx], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}


RS_IDX CreateRasterizerState(const bool cull, const bool solid)
{
	uint8_t currentIdx = rsHolder->NextIdx();

	D3D11_RASTERIZER_DESC desc;
	desc.FillMode = solid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
	desc.CullMode = cull ? D3D11_CULL_BACK : D3D11_CULL_NONE;
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = false;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = true; // always multisample kek
	desc.AntialiasedLineEnable = true;

	ID3D11RasterizerState* tempRaster = 0;
	HRESULT hr = d3d11Data->device->CreateRasterizerState(&desc, &tempRaster);
	assert(!FAILED(hr));
	rsHolder->rs_map.emplace(currentIdx, tempRaster);

	return currentIdx;
}

bool SetRasterizerState(const RS_IDX idx)
{
	assert(true == rsHolder->rs_map.contains(idx));
	d3d11Data->deviceContext->RSSetState(rsHolder->rs_map[idx]);
	return true;
}

void UnsetRasterizerState()
{
	d3d11Data->deviceContext->RSSetState(rs_NULL);
}

bool DeleteD3D11Viewport(const VP_IDX idx)
{
	assert(vpHolder->vp_map.contains(idx));

	vpHolder->vp_map.erase(idx);

	return true;
}

bool DeleteD3D11RenderTargetView(const RTV_IDX idx)
{
	assert(rtvHolder->rtv_map.contains(idx));
	if (rtvHolder->rtv_map.contains(idx))
	{
		rtvHolder->rtv_map[idx]->Release();
		rtvHolder->rtv_map.erase(idx);
	}
	if (rtvHolder->tx_map.contains(idx))
	{
		rtvHolder->tx_map[idx]->Release();
		rtvHolder->tx_map.erase(idx);
	}

	return true;
}


bool DeleteD3D11DepthStencilView(const DSV_IDX idx)
{
	assert(dsvHolder->dsv_map.contains(idx));
	dsvHolder->dsv_map[idx]->Release();
	dsvHolder->dsv_map.erase(idx);
	dsvHolder->ds_map[idx]->Release();
	dsvHolder->ds_map.erase(idx);
	return true;
}

bool DeleteD3D11RasterizerState(const RS_IDX idx)
{
	assert(rsHolder->rs_map.contains(idx));
	rsHolder->rs_map[idx]->Release();
	rsHolder->rs_map.erase(idx);
	return true;
}
