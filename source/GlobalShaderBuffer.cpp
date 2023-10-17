#include "GlobalShaderBuffer.h"
#include "D3D11Helper.h"
#include <DirectXMath.h>
#include <assert.h>

struct GlobalShaderBuffer //1 // also buffer?
{
    DirectX::XMFLOAT4 dirLightColor;
    DirectX::XMFLOAT4 dirLightDirection;
    DirectX::XMFLOAT4 colorMultiplicative = DirectX::XMFLOAT4{1.0f,1.0f,1.0f,1.0f};
    DirectX::XMFLOAT4 colorAdditive = DirectX::XMFLOAT4{0.0f, 0.0f, 0.0f, 0.0f};
};
GlobalShaderBuffer globalShaderBuffer;
int16_t bufferIndex;
bool needUpdate = false;
void CreateGlobalShaderBuffer()
{
    bufferIndex = CreateConstantBuffer(&globalShaderBuffer, sizeof(GlobalShaderBuffer));
}
void UpdateGlobalShaderBuffer()
{
    if (needUpdate)
    {
        assert(true == UpdateConstantBuffer(bufferIndex, &globalShaderBuffer));
    }
}
int16_t GetGlobalShaderBufferIndex()
{
    return bufferIndex;
}
void GSBSetDirectionalLightColor(float r, float g, float b)
{
    globalShaderBuffer.dirLightColor.x = r;
    globalShaderBuffer.dirLightColor.y = g;
    globalShaderBuffer.dirLightColor.z = b;
    needUpdate = true;
}
void GSBSetDirectionalLightDirection(float dirX, float dirY, float dirZ)
{
    globalShaderBuffer.dirLightDirection.x = dirX;
    globalShaderBuffer.dirLightDirection.y = dirY;
    globalShaderBuffer.dirLightDirection.z = dirZ;
    needUpdate = true;
}
void GSBSetDirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ)
{
    GSBSetDirectionalLightColor(r, g, b);
    GSBSetDirectionalLightDirection(dirX, dirY, dirZ);
}
