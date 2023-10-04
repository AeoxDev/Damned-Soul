#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "STB_Helper.h"
#include <iostream>

ID3D11SamplerState* smp_NULL = nullptr;

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
	uint16_t& currentIdx = txHolder->_nextIdx;

    // Check hash
    const uint64_t hash = C_StringToHash(name);
    
    // If the texture is already loaded, return the index to that texture
	for (auto& [key, val] : txHolder->hash_map)
	{
		if (hash == val)
		{
			return key;
		}
	}
	txHolder->img_map.emplace(currentIdx, Image());
	Image& img = txHolder->img_map[currentIdx];
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
	ID3D11Texture2D* tempTex = 0;
	HRESULT hr = d3d11Data->device->CreateTexture2D(&desc, &data, &tempTex);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create ID3D11Texture2D from '" << name << "'" << std::endl;
		return false;
	}
	txHolder->tx_map.emplace(currentIdx, tempTex);

	ID3D11ShaderResourceView* tempSRV = 0;
	hr = d3d11Data->device->CreateShaderResourceView(txHolder->tx_map[currentIdx], nullptr, &tempSRV);
	if (FAILED(hr))
	{
		txHolder->tx_map[currentIdx]->Release();
		std::cerr << "Failed to create ID3D11ShaderResourceView for '" << name << "'" << std::endl;
		return false;
	}
	txHolder->srv_map.emplace(currentIdx, tempSRV);

	// Set the hash last thing you do
	txHolder->hash_map.emplace(currentIdx, hash);
	
	return txHolder->_nextIdx++;
}

TX_IDX CreateTexture(FORMAT format, USAGE_FLAGS useFlags, RESOURCE_FLAGS bindFlags, CPU_FLAGS cpuAcess, const size_t& width, const size_t& height)
{
	// Check hash
	uint16_t& currentIdx = txHolder->_nextIdx;

	D3D11_TEXTURE2D_DESC desc;
	// Take the height and width of the loaded image and set it as the dimensions for the texture
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = (DXGI_FORMAT)format;// DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = (D3D11_USAGE)useFlags;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = (D3D11_CPU_ACCESS_FLAG)cpuAcess;
	desc.MiscFlags = 0;

	ID3D11Texture2D* tempTex = 0;
	// Attempt to create a texture in the device
	HRESULT hr = d3d11Data->device->CreateTexture2D(&desc, nullptr, &tempTex);
	if (FAILED(hr))
	{
		std::cerr << "Failed to create empty ID3D11Texture2D" << std::endl;
		return false;
	}
	txHolder->tx_map.emplace(currentIdx, tempTex);

	// Set the hash last thing you do
	txHolder->hash_map.emplace(currentIdx, currentIdx);
		
	return txHolder->_nextIdx++;
}

bool SetTexture(const TX_IDX idx, const SHADER_TO_BIND_RESOURCE& shader, uint8_t slot)
{
	if (txHolder->_nextIdx < idx || idx < 0)
	{
		std::cerr << "Failed to set compute shader: Index out of range!" << std::endl;
		return false;
	}

	switch (shader)
	{
	case BIND_VERTEX:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_map[idx]);
		break;
	case BIND_HULL:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_map[idx]);
		break;
	case BIND_DOMAIN:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_map[idx]);
		break;
	case BIND_GEOMETRY:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_map[idx]);
		break;
	case BIND_PIXEL:
		d3d11Data->deviceContext->PSSetShaderResources(slot, 1, &txHolder->srv_map[idx]);
		break;
	default:
		std::cerr << "Corrupt or incorrent Shader Type to bind!" << std::endl;
		return false;
		break; // Yes, this break is unnessecary, but it looks nice
	}

	return true;
}

void ReleaseTX(const TX_IDX idx)
{
	if (txHolder->srv_map[idx] != nullptr)
		txHolder->srv_map[idx]->Release();
	if (txHolder->tx_map[idx] != nullptr)
		txHolder->tx_map[idx]->Release();
}

void GetTextureByType(ID3D11Texture2D*& out, TEXTURE_HOLDER_TYPE type, int16_t idx)
{
	uint8_t tempIdx = (uint8_t)idx;

	switch (type)
	{
	case TEXTURE:
		out = txHolder->tx_map[idx];
		break;
	case SHADER_RESOURCE_VIEW:
		//Might crash, might need to use queryinterface
		out = (ID3D11Texture2D*)srvHolder->srv_resource_map[tempIdx];
		break;
	case RENDER_TARGET_VIEW:
		out = rtvHolder->tx_map[tempIdx];
		break;
	case UNORDERED_ACCESS_VIEW:
		//Might crash, might need to use queryinterface
		out = (ID3D11Texture2D*)uavHolder->uav_resource_map[tempIdx];
		break;
	default:
		break;
	}
}


SMP_IDX CreateSamplerState()
{
	uint8_t currentIdx = smpHolder->_nextIdx;
	 
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

	ID3D11SamplerState* tempSamp = 0;
	HRESULT hr = d3d11Data->device->CreateSamplerState(&samplerDesc, &tempSamp);
	if FAILED(hr)
	{
		std::cout << "Failed to create Sampler State" << std::endl;
		return -1;
	}
	smpHolder->smp_map.emplace(currentIdx, tempSamp);

	return smpHolder->_nextIdx++;
}

void SetSamplerState(const SMP_IDX idx, uint8_t slot)
{
	d3d11Data->deviceContext->PSSetSamplers(slot, 1, &(smpHolder->smp_map[idx]));
}

void UnsetSamplerState(uint8_t slot)
{
	d3d11Data->deviceContext->PSSetSamplers(slot, 1, &smp_NULL);
}

void ReleaseSMP(const SMP_IDX idx)
{
	if (smpHolder->smp_map[idx] != nullptr)
		smpHolder->smp_map[idx]->Release();
}
