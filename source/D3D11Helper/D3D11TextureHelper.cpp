#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "STB_Helper.h"
#include <iostream>

// djb2 | Do we need a faster hash?
uint64_t C_StringToHash(const char* c_str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *c_str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

TX_IDX LoadTexture(const char* name)
{
    // Check hash
    const uint64_t hash = C_StringToHash(name);
    
    // If the texture is already loaded, return the index to that texture
	for (unsigned int i = 0; i < txHolder->currentCount; ++i)
	{
		if (hash == txHolder->hash_arr[i])
		{
			return i;
		}
	}
        
	uint16_t& current = txHolder->currentCount;
	Image& img = txHolder->img_arr[current];
	if (false == img.load(name))
	{
		std::cerr << "Failed to open texture file \"" << name << "\"!" << std::endl;
		return -1;
	}

	D3D11_TEXTURE2D_DESC desc;
	// Take the height and width of the loaded image and set it as the dimensions for the texture
	desc.Width = img.m_width;
	desc.Height = img.m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &(*img.m_data); // This might get ruined by defragmentation
	// The width of the texture
	data.SysMemPitch = img.m_width * 4;// img.m_channels;
	// This is a 2d texture, and it does not need (or have) a depth
	data.SysMemSlicePitch = 0;

	// Attempt to create a texture in the device
	HRESULT hr = d3d11Data->device->CreateTexture2D(&desc, &data, &(txHolder->tx_arr[current]));
	if (FAILED(hr))
	{
		std::cerr << "Failed to create ID3D11Texture2D from '" << name << "'" << std::endl;
		return false;
	}

	hr = d3d11Data->device->CreateShaderResourceView(txHolder->tx_arr[current], nullptr, &(txHolder->srv_arr[current]));
	if (FAILED(hr))
	{
		txHolder->tx_arr[current]->Release();
		std::cerr << "Failed to create ID3D11ShaderResourceView for '" << name << "'" << std::endl;
		return false;
	}

	// Set the hash last thing you do
	txHolder->hash_arr[current] = hash;
	return current++;
}

bool SetTexture(const TX_IDX idx, const uint8_t slot, const SHADER_TO_BIND_RESOURCE& shader)
{
	if (txHolder->currentCount < idx || idx < 0)
	{
		std::cerr << "Failed to set compute shader: Index out of range!" << std::endl;
		return false;
	}

	switch (shader)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_arr[idx]);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_arr[idx]);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_arr[idx]);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_arr[idx]);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_arr[idx]);
		break;
	default:
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

void GetTextureByType(ID3D11Texture2D*& out, TEXTURE_HOLDER_TYPE type, IDX_UNION idx)
{
	switch (type)
	{
	case TEXTURE:
		out = txHolder->tx_arr[idx.TX];
		break;
	case SHADER_RESOURCE_VIEW:
		//Might crash, might need to use queryinterface
		out = (ID3D11Texture2D*)srvHolder->srv_resource_arr[idx.VIEWS];
		break;
	case RENDER_TARGET_VIEW:
		out = rtvHolder->tx_arr[idx.VIEWS];
		break;
	case UNORDERED_ACCESS_VIEW:
		//Might crash, might need to use queryinterface
		out = (ID3D11Texture2D*)uavHolder->uav_resource_arr[idx.VIEWS];
		break;
	default:
		break;
	}
}


SMP_IDX CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = d3d11Data->device->CreateSamplerState(&samplerDesc, &smpHolder->smp_arr[smpHolder->currentCount]);
	if FAILED(hr)
	{
		std::cout << "Failed to create Sampler State" << std::endl;
		return -1;
	}

	return smpHolder->currentCount++;
}

void SetSamplerState(const SMP_IDX idx)
{
	d3d11Data->deviceContext->PSSetSamplers(0, 1, &(smpHolder->smp_arr[idx]));
}