#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "UIRenderer.h"
#include <iostream>


ID3D11View* vp_NULL = NULL;
ID3D11RenderTargetView* rtv_NULL = NULL;
ID3D11DepthStencilView* dsv_NULL = NULL;
ID3D11ShaderResourceView* srv_NULL = NULL;
ID3D11UnorderedAccessView* uav_NULL = NULL;
ID3D11RasterizerState* rs_NULL = NULL;


VP_IDX CreateViewport(const size_t& width, const size_t& height)
{
	D3D11_VIEWPORT* vp = &vpHolder->vp_arr[vpHolder->currentCount];
	vp->TopLeftX = 0;
	vp->TopLeftY = 0;
	vp->Width = static_cast<float>(width);
	vp->Height = static_cast<float>(height);
	vp->MinDepth = 0;
	vp->MaxDepth = 1;

	return (vpHolder->currentCount)++;
}

bool SetViewport(const VP_IDX idx)
{
	if (vpHolder->currentCount < idx || idx < 0)
	{
		std::cerr << "Viewport index out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->RSSetViewports(1, &(vpHolder->vp_arr[idx]));
	return true;
}


RTV_IDX CreateRenderTargetView()
{
	// get the address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(d3d11Data->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	//D3D11_RENDER_TARGET_VIEW_DESC desc;
	//desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//desc.Texture2D.MipSlice = 0;

	// use the back buffer address to create the render target
	// null as description to base it on the backbuffers values
	HRESULT hr = d3d11Data->device->CreateRenderTargetView(backBuffer, NULL, &(rtvHolder->rtv_arr[rtvHolder->currentCount]));
	backBuffer->Release();

	if (FAILED(hr))
	{
		std::cerr << "Failed to create Render Target View!" << std::endl;
		return false;
	}

	return (rtvHolder->currentCount)++;

}


DSV_IDX CreateDepthStencil(const size_t& width, const size_t& height)
{
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

	HRESULT hr = d3d11Data->device->CreateTexture2D(&textureDesc, nullptr, &(dsvHolder->ds_arr[dsvHolder->currentCount]));
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Depth Stencil Texture!" << std::endl;
		return false;
	}

	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//depthStencilDesc.DepthEnable = true;
	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;

	hr = d3d11Data->device->CreateDepthStencilView(dsvHolder->ds_arr[dsvHolder->currentCount], 0, &(dsvHolder->dsv_arr[dsvHolder->currentCount]));
	if (FAILED(hr))
	{
		dsvHolder->ds_arr[dsvHolder->currentCount]->Release(); // Release if failed
		std::cerr << "Failed to create Depth Stencil View!" << std::endl;
		return false;
	}

	return (dsvHolder->currentCount)++;
}

bool SetRenderTargetViewAndDepthStencil(const RTV_IDX idx_rtv, const DSV_IDX idx_dsv)
{
	if (rtvHolder->currentCount < idx_rtv || idx_rtv < 0 || dsvHolder->currentCount < idx_dsv || idx_dsv < 0)
	{
		std::cerr << "Failed to set render target view and depth stencil view, out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->OMSetRenderTargets(1, &(rtvHolder->rtv_arr[idx_rtv]), dsvHolder->dsv_arr[idx_dsv]);
	return true;
}

SRV_IDX CreateShaderResourceView(const void* data, const size_t size, const SHADER_TO_BIND_RESOURCE& bindto, const RESOURCES& resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const uint8_t slot)
{
	HRESULT hr;
	uint16_t currentIdx = srvHolder->currentCount;

	//NOTE TODO: Hardcoded amount, dlete this
	int TESTDELLATER = 100;

	//Check what resource to create, then create it

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	//This switch firstly, defines the and creates the resource that the SRV is to use
	//To then define and create the SRV that will be holding the resource
	switch (resource)
	{
	case RESOURCE_BUFFER:
		ID3D11Buffer* buffer;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = size * TESTDELLATER; // Should be times the amount of elements
		bufferDesc.CPUAccessFlags = CPUFlags;
		bufferDesc.BindFlags = resourceFlags; //D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;  // Hardcoded for particles, might be ok as we probably wont use UAV buffers for anything other than particles
		bufferDesc.StructureByteStride = size;

		D3D11_SUBRESOURCE_DATA buffData;
		buffData.pSysMem = data;
		buffData.SysMemPitch = 0;
		buffData.SysMemSlicePitch = 0;

		hr = d3d11Data->device->CreateBuffer(&bufferDesc, &buffData, &buffer);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create Buffer to be used for Shader Resource View!" << std::endl;
			return false;
		}

		//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
		hr = buffer->QueryInterface(__uuidof(ID3D11Buffer), (void**)&srvHolder->srv_resource_arr[currentIdx]);
		if (FAILED(hr)) {
			std::cerr << "Failed to QueryInterface into buffer" << std::endl;
			return false;
		}


		//Now create the SRV
		SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		SRVDesc.Buffer.FirstElement = 0;
		SRVDesc.Buffer.ElementOffset = 0;
		SRVDesc.Buffer.ElementWidth = size;
		SRVDesc.Buffer.FirstElement = 0;

		hr = d3d11Data->device->CreateShaderResourceView(srvHolder->srv_resource_arr[currentIdx], &SRVDesc, &(srvHolder->srv_arr[currentIdx]));
		if (FAILED(hr))
		{
			std::cerr << "Failed to create Shader Resource View!" << std::endl;
			return false;
		}

		break;
	case RESOURCE_TEXTURE1D:
		//Define if needed
		break;
	case RESOURCE_TEXTURE1DARRAY:
		//Define if needed
		break;
	case RESOURCE_TEXTURE2D:
		//Define if needed
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
		break;
	}



	

	uint32_t* metadata = srvHolder->metadata_arr[currentIdx];
	metadata[0] = bindto;
	metadata[1] = slot; // Set slot to use
	metadata[2] = size;

	return (srvHolder->currentCount)++;
}

bool SetShaderResourceView(const SRV_IDX idx)
{
	ID3D11ShaderResourceView* setter = srvHolder->srv_arr[idx];
	SHADER_TO_BIND_RESOURCE whichShader = (SHADER_TO_BIND_RESOURCE)srvHolder->metadata_arr[idx][0];
	uint8_t slot = srvHolder->metadata_arr[idx][1];

	switch (whichShader)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->VSSetShaderResources(slot, 1, &srvHolder->srv_arr[idx]);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->HSSetShaderResources(slot, 1, &srvHolder->srv_arr[idx]);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->DSSetShaderResources(slot, 1, &srvHolder->srv_arr[idx]);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->GSSetShaderResources(slot, 1, &srvHolder->srv_arr[idx]);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &srvHolder->srv_arr[idx]);
		break;
	case BIND_COMPUTE:
		d3d11Data->deviceContext->CSSetShaderResources(slot, 1, &srvHolder->srv_arr[idx]);
		break;
	default:
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

bool UnloadShaderResourceView(const SRV_IDX idx)
{
	ID3D11ShaderResourceView* setter = srvHolder->srv_arr[idx];
	SHADER_TO_BIND_RESOURCE whichShader = (SHADER_TO_BIND_RESOURCE)srvHolder->metadata_arr[idx][0];
	uint8_t slot = srvHolder->metadata_arr[idx][1];

	//In this overload, the slot is first set to NULL then the SRV is set
	switch (whichShader)
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
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

void CopyToVertexBuffer(const CB_IDX destination, const SRV_IDX source)
{
	ID3D11Resource* vertexBufferResource = bfrHolder->buff_arr[destination];
	d3d11Data->deviceContext->CopyResource(vertexBufferResource, srvHolder->srv_resource_arr[source]);
}

UAV_IDX CreateUnorderedAcessView(const void* data, const size_t size, const RESOURCES &resource, RESOURCE_FLAGS resourceFlags, const CPU_FLAGS& CPUFlags, const uint8_t slot)
{
	HRESULT hr;
	uint16_t currentIdx = uavHolder->currentCount;

	//NOTE TODO: Hardcoded amount, dlete this
	int TESTDELLATER = 100;

	//Check what resource to create, then create it


	switch (resource)
	{
	case RESOURCE_BUFFER:
		ID3D11Buffer* buffer;

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = size * TESTDELLATER; // Should be times the amount of elements
		bufferDesc.CPUAccessFlags = CPUFlags;
		bufferDesc.BindFlags = resourceFlags; //D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;  // Hardcoded for particles, might be ok as we probably wont use UAV buffers for anything other than particles
		bufferDesc.StructureByteStride = size;

		hr = d3d11Data->device->CreateBuffer(&bufferDesc, nullptr, &buffer);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create Buffer to be used for Unordered Access View!" << std::endl;
			return false;
		}

		//QueryInterface is a way to cast COM objects, this takes the recently created buffer and puts it into the resource array.
		hr = buffer->QueryInterface(__uuidof(ID3D11Buffer), (void**)&uavHolder->uav_resource_arr[currentIdx]);
		if (FAILED(hr)) {
			std::cerr << "Failed to QueryInterface into buffer" << std::endl;
			return false;
		}

		break;
	case RESOURCE_TEXTURE1D:
		//Define if needed
		break;
	case RESOURCE_TEXTURE1DARRAY:
		//Define if needed
		break;
	case RESOURCE_TEXTURE2D:
		//Define when STB image is implemented

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
		break;
	}


	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = TESTDELLATER; //Hard coded value

	hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_arr[currentIdx], &UAVDesc, &(uavHolder->uav_arr[currentIdx]));
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Unordered Access View!" << std::endl;
		return false;
	}


	uint32_t* metadata = uavHolder->metadata_arr[currentIdx];
	metadata[0] = slot; // Set slot to use
	metadata[1] = size;

	return (uavHolder->currentCount)++;
}

UAV_IDX CreateUnorderedAcessView(const void* data, const size_t size, const SRV_IDX SRVIndex, const uint8_t slot)
{
	HRESULT hr;
	uint16_t currentIdx = uavHolder->currentCount;

	//NOTE TODO: Hardcoded amount, dlete this
	int TESTDELLATER = 100;

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = TESTDELLATER; //Hard coded value

	// Set the UAVs resource to the resource created for the SRV
	uavHolder->uav_resource_arr[currentIdx] = srvHolder->srv_resource_arr[SRVIndex];

	hr = d3d11Data->device->CreateUnorderedAccessView(uavHolder->uav_resource_arr[currentIdx], &UAVDesc, &(uavHolder->uav_arr[currentIdx]));
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Unordered Access View!" << std::endl;
		return false;
	}


	uint32_t* metadata = uavHolder->metadata_arr[currentIdx];
	metadata[0] = slot; // Set slot to use
	metadata[1] = size;

	return (uavHolder->currentCount)++;
}

bool SetUnorderedAcessView(const UAV_IDX idx)
{
	ID3D11UnorderedAccessView* setter = uavHolder->uav_arr[idx];
	uint8_t slot = uavHolder->metadata_arr[idx][0];

	d3d11Data->deviceContext->CSSetUnorderedAccessViews(slot, 1, &uavHolder->uav_arr[idx], NULL);

	return true;
}

bool UnloadUnorderedAcessView(const UAV_IDX idx)
{
	ID3D11UnorderedAccessView* setter = uavHolder->uav_arr[idx];
	uint8_t slot = uavHolder->metadata_arr[idx][0];

	d3d11Data->deviceContext->CSSetUnorderedAccessViews(slot, 1, &uav_NULL, NULL);

	return true;
}

void ClearRenderTargetView(const RTV_IDX idx)
{
	float color[4] = { 1.0f, 0.0f, 0.84f, 0.0f };
	d3d11Data->deviceContext->ClearRenderTargetView(rtvHolder->rtv_arr[idx], color);
}

void ClearDepthStencilView(const DSV_IDX idx)
{
	d3d11Data->deviceContext->ClearDepthStencilView(dsvHolder->dsv_arr[idx], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}


RS_IDX CreateRasterizerState(const bool cull, const bool solid)
{
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

	HRESULT hr = d3d11Data->device->CreateRasterizerState(&desc, &(rsHolder->rs_arr[rsHolder->currentCount]));
	if (FAILED(hr))
	{
		std::cerr << "Failed to create rasterizer state!" << std::endl;
		return -1;
	}

	return (rsHolder->currentCount)++;
}

bool SetRasterizerState(const RS_IDX idx)
{
	if (idx < 0 || rsHolder->currentCount < idx)
	{
		std::cerr << "Rasterizer state out of range!" << std::endl;
		return false;
	}

	d3d11Data->deviceContext->RSSetState(rsHolder->rs_arr[idx]);
	return true;
}
