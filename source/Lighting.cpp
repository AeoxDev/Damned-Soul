#include "Lighting.h"

bool CreateLightingConstantBuffer(ID3D11Device* device, ID3D11Buffer** constantBuffer)
{
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.ByteWidth = sizeof(LightingStruct);

    HRESULT hr = device->CreateBuffer(&desc, nullptr, constantBuffer);

    if (FAILED(hr))
    {
        std::cout << "ERROR! COULD NOT CREATE LIGHTING BUFFER!" << std::endl;
        return false;
    }

    return true;
}

void UpdateLightingBuffer(ID3D11DeviceContext* context, ID3D11Buffer* lightingConstantBuffer, LightingStruct& lightingData)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->Map(lightingConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &lightingData, sizeof(LightingStruct));
    context->Unmap(lightingConstantBuffer, 0);
}
