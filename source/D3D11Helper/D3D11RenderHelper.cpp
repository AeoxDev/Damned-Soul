#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "UIRenderer.h"
#include "SDLHandler.h"
#include <iostream>


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

	vpHolder->vp_map.emplace(vpHolder->_nextIdx, vp);

	return (vpHolder->_nextIdx)++;
}

bool SetViewport(const VP_IDX idx)
{
	if (vpHolder->_nextIdx < idx || idx < 0)
	{
		std::cerr << "Viewport index out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->RSSetViewports(1, &(vpHolder->vp_map[idx]));
	return true;
}


RTV_IDX CreateBackBuffer()
{
	uint8_t currentIdx = rtvHolder->_nextIdx;

	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(d3d11Data->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}


	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	ID3D11RenderTargetView* tempBB = 0;
	HRESULT hr = d3d11Data->device->CreateRenderTargetView(backBuffer, NULL, &tempBB);

	if (FAILED(hr))
	{
		std::cerr << "Failed to create Render Target View!" << std::endl;
		return false;
	}
	rtvHolder->rtv_map.emplace(currentIdx, tempBB);
	backBuffer->Release();

	return (rtvHolder->_nextIdx)++;
}

RTV_IDX CreateRenderTargetView(USAGE_FLAGS useFlags, RESOURCE_FLAGS bindFlags, CPU_FLAGS cpuAcess, const size_t& width, const size_t& height, FORMAT format)
{
	uint8_t currentIdx = rtvHolder->_nextIdx;

	D3D11_TEXTURE2D_DESC desc;
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
	if (FAILED(hr))
	{
		std::cerr << "Failed to create ID3D11Texture2D for RenderTargetView" << std::endl;
		return false;
	}
	rtvHolder->tx_map.emplace(currentIdx, tempTex);

	ID3D11RenderTargetView* tempRTV = 0;
	hr = d3d11Data->device->CreateRenderTargetView(rtvHolder->tx_map[currentIdx], NULL, &tempRTV);
	if (FAILED(hr))
	{
		// If the RTV failed to create, release the texture
		rtvHolder->tx_map[currentIdx]->Release();
		std::cerr << "Failed to create ID3D11RenderTargetView" << std::endl;
		return false;
	}
	rtvHolder->rtv_map.emplace(currentIdx, tempRTV);

	// Set the hash last thing you do
	return (rtvHolder->_nextIdx)++;

}

DSV_IDX CreateDepthStencil(const size_t& width, const size_t& height)
{
	uint8_t currentIdx = dsvHolder->_nextIdx;


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
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Depth Stencil Texture!" << std::endl;
		return false;
	}
	dsvHolder->ds_map.emplace(currentIdx, tempTex);

	ID3D11DepthStencilView* tempDSV = 0;
	hr = d3d11Data->device->CreateDepthStencilView(dsvHolder->ds_map[currentIdx], 0, &tempDSV);
	if (FAILED(hr))
	{
		dsvHolder->ds_map[currentIdx]->Release(); // Release if failed
		std::cerr << "Failed to create Depth Stencil View!" << std::endl;
		return false;
	}
	dsvHolder->dsv_map.emplace(currentIdx, tempDSV);

	return (dsvHolder->_nextIdx)++;
}

bool SetRenderTargetViewAndDepthStencil(const RTV_IDX idx_rtv, const DSV_IDX idx_dsv)
{
	if (rtvHolder->_nextIdx < idx_rtv || idx_rtv < 0 || dsvHolder->_nextIdx < idx_dsv || idx_dsv < 0)
	{
		std::cerr << "Failed to set render target view and depth stencil view, out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->OMSetRenderTargets(1, &(rtvHolder->rtv_map[idx_rtv]), dsvHolder->dsv_map[idx_dsv]);
	return true;
}

void UnsetRenderTargetViewAndDepthStencil()
{
	d3d11Data->deviceContext->OMSetRenderTargets(1, &rtv_NULL, dsv_NULL);
}

SRV_IDX CreateShaderResourceViewBuffer(const void* data, const size_t& size, const int amount, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags)
{
	uint8_t currentIdx = srvHolder->_nextIdx;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (UINT)size * amount;
	bufferDesc.CPUAccessFlags = CPUFlags;
	bufferDesc.BindFlags = resourceFlags;
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
		std::cerr << "Failed to create Buffer to be used for Shader Resource View!" << std::endl;
		return false;
	}

	ID3D11Resource* tempResource = 0;
	//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
	hr = tempBuff->QueryInterface(__uuidof(ID3D11Buffer), (void**)&tempResource);
	if (FAILED(hr)) {
		std::cerr << "Failed to QueryInterface into buffer" << std::endl;
		tempBuff->Release();
		return false;
	}
	srvHolder->srv_resource_map.emplace(currentIdx, tempResource);


	//Now create the SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.FirstElement = 0;
	SRVDesc.Buffer.ElementOffset = 0;
	SRVDesc.Buffer.ElementWidth = (UINT)size;
	SRVDesc.Buffer.FirstElement = 0;

	ID3D11ShaderResourceView* tempSRV = 0;
	hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], &SRVDesc, &tempSRV);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Shader Resource View!" << std::endl;
		srvHolder->srv_resource_map[currentIdx]->Release();
		return false;
	}
	srvHolder->srv_map.emplace(currentIdx, tempSRV);
	srvHolder->size.emplace(currentIdx, (uint32_t)size);

	return (srvHolder->_nextIdx)++;
}

SRV_IDX CreateShaderResourceViewTexture(const RESOURCES& resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const size_t& width, const size_t& height)
{
	HRESULT hr;
	uint8_t currentIdx = srvHolder->_nextIdx;
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
			if (FAILED(hr))
			{
				std::cerr << "Failed to create Depth Stencil Texture!" << std::endl;
				return false;
			}


			//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
			hr = tempTex->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tempResource);
			if (FAILED(hr)) {
				std::cerr << "Failed to QueryInterface into buffer" << std::endl;
				tempTex->Release();
				return false;
			}
			srvHolder->srv_resource_map.emplace(currentIdx, tempResource);

			SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
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
			std::cerr << "Did not create requested Shader Resource View, requested case is not defined" << std::endl;
			return false;
			break;
	}

	if (tempResource == nullptr)
	{
		std::cerr << "tempResource was null after creation, something went wrong. SRV not created" << std::endl;
		return false;
	}

	ID3D11ShaderResourceView* tempSRV = 0;
	hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], &SRVDesc, &tempSRV);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Shader Resource View!" << std::endl;
		srvHolder->srv_resource_map[currentIdx]->Release();
		return false;
	}
	srvHolder->srv_map.emplace(currentIdx, tempSRV);

	//Get the size of the texture by mapping and saving the DepthPitch
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//d3d11Data->deviceContext->Map(srvHolder->srv_resource_map[currentIdx], 0, D3D11_MAP_WRITE, 0, &mappedResource);
	//srvHolder->size.emplace(currentIdx, mappedResource.DepthPitch);
	//d3d11Data->deviceContext->Unmap(srvHolder->srv_resource_map[currentIdx], 0);


	return (srvHolder->_nextIdx)++;
}

SRV_IDX CreateShaderResourceViewTexture(const int8_t sourceIdx, RESOURCE_FLAGS sourceResource)
{
	uint8_t currentIdx = srvHolder->_nextIdx;
	HRESULT hr = NULL;
	ID3D11ShaderResourceView* tempSRV = 0;
	ID3D11Resource* tempResource = 0;


	switch (sourceResource)
	{
	case BIND_RENDER_TARGET:
		tempResource = rtvHolder->tx_map[sourceIdx];
		srvHolder->srv_resource_map.emplace(currentIdx, tempResource);
		srvHolder->srv_resource_map[currentIdx]->AddRef();

		hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], NULL, &tempSRV);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create Shader Resource View from a Render Target View!" << std::endl;
			srvHolder->srv_resource_map[currentIdx]->Release();
			return false;
		}
		srvHolder->srv_map.emplace(currentIdx, tempSRV);

		//Get the size of the texture by mapping and saving the DepthPitch
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//d3d11Data->deviceContext->Map(srvHolder->srv_resource_map[currentIdx], 0, D3D11_MAP_WRITE, 0, &mappedResource);
		//srvHolder->size.emplace(currentIdx, mappedResource.DepthPitch);
		//d3d11Data->deviceContext->Unmap(srvHolder->srv_resource_map[currentIdx], 0);

		break;
	case BIND_SHADER_RESOURCE:
		tempResource = rtvHolder->tx_map[sourceIdx];
		srvHolder->srv_resource_map.emplace(currentIdx, tempResource);
		srvHolder->srv_resource_map[currentIdx]->AddRef();


		hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_map[currentIdx], NULL, &tempSRV);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create Shader Resource View from another Shader Resource View!" << std::endl;
			srvHolder->srv_resource_map[currentIdx]->Release();
			return false;
		}
		srvHolder->srv_map.emplace(currentIdx, tempSRV);
		srvHolder->size.emplace(currentIdx, srvHolder->size[sourceIdx]);

		break;
	default:
		std::cerr << "Did not create requested Shader Resource View (overload), requested case is not defined" << std::endl;
		return false;
		break;
	}

	return (srvHolder->_nextIdx)++;
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
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
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
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
		break;
	}

}

void CopyToVertexBuffer(const CB_IDX destination, const SRV_IDX source)
{
	d3d11Data->deviceContext->CopyResource(bfrHolder->buff_map[destination], srvHolder->srv_resource_map[source]);
}

SRV_IDX CreateUnorderedAccessViewBuffer(const void* data, const size_t& size, const int amount, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags)
{
	uint8_t currentIdx = uavHolder->_nextIdx;

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
		std::cerr << "Failed to create Buffer to be used for Unordered Access View!" << std::endl;
		return false;
	}

	ID3D11Resource* tempResource = 0;
	//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
	hr = tempBuff->QueryInterface(__uuidof(ID3D11Buffer), (void**)&tempResource);
	if (FAILED(hr)) {
		std::cerr << "Failed to QueryInterface into buffer" << std::endl;
		tempBuff->Release();
		return false;
	}
	uavHolder->uav_resource_map.emplace(currentIdx, tempResource);


	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = amount;

	ID3D11UnorderedAccessView* tempUAV = 0;
	hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_map[currentIdx], &UAVDesc, &tempUAV);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Unordered Access View!" << std::endl;
		return false;
	}
	uavHolder->uav_map.emplace(currentIdx, tempUAV);

	uavHolder->size.emplace(currentIdx, (uint32_t)size);

	return (uavHolder->_nextIdx)++;
}

SRV_IDX CreateUnorderedAccessViewBuffer(const size_t& size, const int amount, const int16_t idx)
{
	uint8_t currentIdx = uavHolder->_nextIdx;

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = amount;

	ID3D11Resource* tempResource = srvHolder->srv_resource_map[idx];
	uavHolder->uav_resource_map.emplace(currentIdx, tempResource);
	uavHolder->uav_resource_map[currentIdx]->AddRef();


	ID3D11UnorderedAccessView* tempUAV = 0;
	HRESULT hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_map[currentIdx], &UAVDesc, &tempUAV);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Unordered Access View from another resource!" << std::endl;
		uavHolder->uav_resource_map[currentIdx]->Release();
		return false;
	}
	uavHolder->uav_map.emplace(currentIdx, tempUAV);
	uavHolder->size.emplace(currentIdx, (uint32_t)size);


	return (uavHolder->_nextIdx)++;
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
	uint8_t currentIdx = rsHolder->_nextIdx;

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
	if (FAILED(hr))
	{
		std::cerr << "Failed to create rasterizer state!" << std::endl;
		return -1;
	}
	rsHolder->rs_map.emplace(currentIdx, tempRaster);

	return (rsHolder->_nextIdx)++;
}

bool SetRasterizerState(const RS_IDX idx)
{
	if (idx < 0 || rsHolder->_nextIdx < idx)
	{
		std::cerr << "Rasterizer state out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->RSSetState(rsHolder->rs_map[idx]);
	return true;
}

void UnsetRasterizerState()
{
	d3d11Data->deviceContext->RSSetState(rs_NULL);
}
