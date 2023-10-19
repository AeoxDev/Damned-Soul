#include "Light.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "MemLib/ML_Vector.hpp"

#include "D3D11Helper.h"
#include <assert.h>
#include "LightComponent.h"

#include "EntityFramework.h"
#include "Registry.h"
#define LIGHT_COMPONENT_ARRAY_LIMIT 32

//struct LightingStruct
//{
//	DirectX::XMFLOAT4	ambientColor; // Kind of like background lighting
//	DirectX::XMFLOAT4	diffuseColor; // Color of the light
//	DirectX::XMFLOAT4	specularColor; // reflection, shiny 
//	float intensity; // focus of the light. Basically tigher/focused vs broader/stofter
//};

struct LightComponent
{
    int slot;
};

struct LightShaderData
{
    int type = 0;// 1 pointlight, 2 spotlight
    float fallofFactor = 1.0f;//attenuation
    float lightRange = 1.0f;
    float lightCone;
    DirectX::XMFLOAT4 lightColor;
    DirectX::XMFLOAT4 lightDirection;
    DirectX::XMFLOAT4 lightPosition;
};

struct LightComponentShaderBuffer
{
    int firstLight = 0;//First light in array
    int lastLight = 31;//Last light in array
    float padding1 = 0.0;
    float padding2 = 0.0;
    DirectX::XMFLOAT4 dirLightColor;
    DirectX::XMFLOAT4 dirLightDirection;
    LightShaderData lights[LIGHT_COMPONENT_ARRAY_LIMIT];
};

struct PointLightStruct //2 // also buffer?
{
    DirectX::XMFLOAT4 m_pointLightColor;
    DirectX::XMFLOAT4 m_pointLightPosition;
    DirectX::XMFLOAT4 m_pointLightRange;
};

struct SpotLightStruct //3 // also buffer?
{
    DirectX::XMFLOAT4 m_spotLightColor;
    DirectX::XMFLOAT4 m_spotLightDirection;
    DirectX::XMFLOAT4 m_spotLightPosition;
    DirectX::XMFLOAT4 m_spotLightRange;
    DirectX::XMFLOAT4 m_spotLightCone;
};

int16_t m_pointLightBufferIndex;

PointLightStruct PointLight;

int16_t m_spotLightBufferIndex;

SpotLightStruct SpotLight;
LightComponentShaderBuffer lightShaderBuffer;
int16_t lightBufferIndex;
bool updateBuffer = false;
//ML_Vector<int> *freeSlot;//Stack of all free slots in the light component array
int lastSlot = 0;
int freeSlotStack[LIGHT_COMPONENT_ARRAY_LIMIT];

int PopSlotStack()
{
    return freeSlotStack[--lastSlot];
}
void PushSlotStack(int& value)
{
    assert(lastSlot < LIGHT_COMPONENT_ARRAY_LIMIT);
    freeSlotStack[lastSlot++] = value;
}

void Light::SetColor(int type, const float x, const float y, const float z)
{
    assert(type > 0 && type <= 3);
    if (type == 1)
    {
        lightShaderBuffer.dirLightColor.x = x;
        lightShaderBuffer.dirLightColor.y = y;
        lightShaderBuffer.dirLightColor.z = z;
        lightShaderBuffer.dirLightColor.w = 1.0f;
        //UpdateConstantBuffer(Light::GetLightBufferIndex(1), &DirLight);//Should be moved
        updateBuffer = true;
    }
    else if (type == 2)
    {
        PointLight.m_pointLightColor.x = x;
        PointLight.m_pointLightColor.y = y;
        PointLight.m_pointLightColor.z = z;
        UpdateConstantBuffer(Light::GetLightBufferIndex(2), &PointLight);

    }
    else if (type == 3)
    {
        SpotLight.m_spotLightColor.x = x;
        SpotLight.m_spotLightColor.y = y;
        SpotLight.m_spotLightColor.z = z;
        UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);

    }
}

void Light::SetPosition(int type, const float x, const float y, const float z)
{
    assert(type > 1 && type <= 3);
    if(type == 2)
    {
        PointLight.m_pointLightPosition.x = x;
        PointLight.m_pointLightPosition.y = y;
        PointLight.m_pointLightPosition.z = z;
        UpdateConstantBuffer(Light::GetLightBufferIndex(2), &PointLight);
    }
    else if (type == 3)
    {
        SpotLight.m_spotLightPosition.x = x;
        SpotLight.m_spotLightPosition.y = y;
        SpotLight.m_spotLightPosition.z = z;
        UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);
    }
}

void Light::SetDirection(int type, const float x, const float y, const float z)
{
    assert(type > 0 && type <= 3 && type != 2);
    if (type == 1)
    {
        float length = sqrtf(x * x + y * y + z * z);
        lightShaderBuffer.dirLightDirection.x = x/ length;
        lightShaderBuffer.dirLightDirection.y = y/ length;
        lightShaderBuffer.dirLightDirection.z = z/ length;
        lightShaderBuffer.dirLightDirection.w = 0.0f;
        //UpdateConstantBuffer(Light::GetLightBufferIndex(1), &DirLight);
        updateBuffer = true;
    }
    else if (type == 3)
    {
        float length = sqrtf(x * x + y * y + z * z);
        SpotLight.m_spotLightDirection.x = x/ length;
        SpotLight.m_spotLightDirection.y = y/ length;
        SpotLight.m_spotLightDirection.z = z/ length;
        UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);
    }
}

void Light::SetRange(int type, const float range)
{
    assert(type > 1 && type <= 3);
    if (type == 2)
    {
        PointLight.m_pointLightRange.x = range;
        UpdateConstantBuffer(Light::GetLightBufferIndex(2), &PointLight);
    }
    else if (type == 3)
    {
        SpotLight.m_spotLightRange.x = range;
        UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);
    }
}

void Light::SetSpotLightCone(const float angle)
{
    SpotLight.m_spotLightCone.x = 3.1415f * angle / 180.0f;
    UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);
}

DirectX::XMVECTOR Light::GetColor(int type)
{
    assert(type > 0 && type <= 3);
    if (type == 1)
    {
        return DirectX::XMLoadFloat4(&lightShaderBuffer.dirLightColor); 
    }
    else if (type == 2)
    {
        return DirectX::XMLoadFloat4(&PointLight.m_pointLightColor);
    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight.m_spotLightColor);
    }
    return DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f };
}

DirectX::XMVECTOR Light::GetPosition(int type)
{
    assert(type > 1 && type <= 3);
    if (type == 2)
    {
        return DirectX::XMLoadFloat4(&PointLight.m_pointLightPosition);

    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight.m_spotLightPosition);
    }
    return DirectX::XMVECTOR{0.0f, 0.0f, 0.0f, 0.0f};
}

DirectX::XMVECTOR Light::GetDirection(int type)
{
    assert(type > 0 && type <= 3 && type != 2);
    if (type == 1)
    {
        return DirectX::XMLoadFloat4(&lightShaderBuffer.dirLightDirection);
        
    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight.m_spotLightDirection);
    }
    return DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f };
}

int16_t Light::GetLightBufferIndex(int type)
{
    assert(type > 0 && type <= 3);
    if (type == 1)
    {
        return lightBufferIndex;

    }
    else if (type == 2)
    {
        return m_pointLightBufferIndex;

    }
    else if (type == 3)
    {
        return m_spotLightBufferIndex;
    }
    return -1;
}

int16_t Light::GetLightBufferIndex()
{
    return lightBufferIndex;
}

void Light::CreateLight(int type) //inte klar --constantbuffer
{
    assert(type > 0 && type <= 3);
    if (type == 1)
    {
        //m_directionLightBufferIndex = CreateConstantBuffer(&(DirLight.m_dirLightColor), sizeof(DirectionLightStruct));
        SetColor(type, 1.0f, 1.0f, 1.0f);
        SetDirection(type, -1.0f, -1.0f, 1.0f);
        updateBuffer = true;

        //DirLight = MemLib::palloc(sizeof(DirectionLightStruct))
      
        //Default done, update now

        //Prepare the buffer to creation
 
        
        //UpdateConstantBuffer(m_directionLightBufferIndex, &(DirLight.m_dirLightColor));
    

    }
    else if (type == 2)
    {
        m_pointLightBufferIndex = CreateConstantBuffer(&(PointLight.m_pointLightColor), sizeof(PointLightStruct));
        SetColor(type, 3.0f, 0.0f, 0.0f);
        SetPosition(type, -78.0f, 3.0f, -32.0f);
        SetRange(type, 30.0f);
       
       /* PointLight = MemLib::palloc(sizeof(PointLightStruct));*/

   

        //Default done, update now

        //Prepare the buffer to creation


        UpdateConstantBuffer(m_pointLightBufferIndex, &(PointLight.m_pointLightColor));

    }
    else if (type == 3)
    {
        m_spotLightBufferIndex = CreateConstantBuffer(&(SpotLight.m_spotLightColor), sizeof(SpotLightStruct));
        SetColor(type, 1.0f, 1.0f, 1.0f);
        SetPosition(type, 50.0f, 5.0f, 40.0f);
        SetDirection(type, -1.0f, -1.0f, -1.0f);
        SetRange(type, 50.0f);
        SetSpotLightCone(90.0f);//Not in radians

        /*SpotLight = MemLib::palloc(sizeof(SpotLightStruct));*/


        //Default done, update now

        //Prepare the buffer to creation


        UpdateConstantBuffer(m_spotLightBufferIndex, &(SpotLight.m_spotLightColor));
    }
}

//Setup all light buffers and shader data
void Light::SetupLight()
{
    //freeSlot = (ML_Vector<int>*)MemLib::spush(sizeof(freeSlot));
    //freeSlot->Initialize();
    //Push back to stack from 31 to 0
    for (int i = LIGHT_COMPONENT_ARRAY_LIMIT - 1; i >= 0; --i)
    {
        //freeSlot->push_back(i);
        PushSlotStack(i);
        //freeSlotStack[lastSlot++] = i;
    }
    lightBufferIndex = CreateConstantBuffer(&lightShaderBuffer, sizeof(LightComponentShaderBuffer));
}

void Light::UpdateLight()
{
    if (updateBuffer)
    {
        UpdateConstantBuffer(lightBufferIndex, &lightShaderBuffer);
        updateBuffer = false;
    }
    
}

void Light::FreeLight()
{
    //MemLib::pfree(DirLight);
    //MemLib::pfree(PointLight);
    //MemLib::pfree(SpotLight);
    //freeSlot->~ML_Vector();
    lastSlot = 0;
    for (int i = LIGHT_COMPONENT_ARRAY_LIMIT - 1; i >= 0; --i)
    {
        //freeSlot->push_back(i);
        PushSlotStack(i);
        lightShaderBuffer.lights[i].type = 0;
        //freeSlotStack[lastSlot++] = i;
    }
    updateBuffer = true;
}


void SetDirectionLight(float colorRed, float colorGreen, float colorBlue, float directionX, float directionY, float directionZ)
{
    Light::SetColor(1, colorRed, colorGreen, colorBlue);
    Light::SetDirection(1, directionX, directionY, directionZ);
}
//Point light
int CreatePointLight(EntityID& entity, float colorRed, float colorGreen, float colorBlue, float positionX, float positionY, float positionZ, float range, float fallofFactor)
{
    RemoveLight(entity);
    int slot = PopSlotStack();
    lightShaderBuffer.lights[slot].type = LightType::pointLight;
    lightShaderBuffer.lights[slot].lightColor.x = colorRed;
    lightShaderBuffer.lights[slot].lightColor.y = colorGreen;
    lightShaderBuffer.lights[slot].lightColor.z = colorBlue; 
    lightShaderBuffer.lights[slot].lightPosition.x = positionX;
    lightShaderBuffer.lights[slot].lightPosition.y = positionY;
    lightShaderBuffer.lights[slot].lightPosition.z = positionZ;
    lightShaderBuffer.lights[slot].lightRange = range;
    lightShaderBuffer.lights[slot].fallofFactor = fallofFactor;
    updateBuffer = true;
    //Add light component to entity
    LightComponent* lightComponent = registry.AddComponent<LightComponent>(entity);
    lightComponent->slot = slot;
    return slot;
}
//Spot light, angle is in degrees, not radians
int CreateSpotLight(EntityID& entity, float colorRed, float colorGreen, float colorBlue, float positionX, float positionY, float positionZ, float range, float fallofFactor,
    float directionX, float directionY, float directionZ, float angle)
{
    RemoveLight(entity);
    int slot = PopSlotStack();
    lightShaderBuffer.lights[slot].type = LightType::spotlight;
    lightShaderBuffer.lights[slot].lightColor.x = colorRed;
    lightShaderBuffer.lights[slot].lightColor.y = colorGreen;
    lightShaderBuffer.lights[slot].lightColor.z = colorBlue;
    lightShaderBuffer.lights[slot].lightPosition.x = positionX;
    lightShaderBuffer.lights[slot].lightPosition.y = positionY;
    lightShaderBuffer.lights[slot].lightPosition.z = positionZ;
    lightShaderBuffer.lights[slot].lightRange = range;
    lightShaderBuffer.lights[slot].fallofFactor = fallofFactor;
    lightShaderBuffer.lights[slot].lightDirection.x = directionX;
    lightShaderBuffer.lights[slot].lightDirection.y = directionY;
    lightShaderBuffer.lights[slot].lightDirection.z = directionZ;
    lightShaderBuffer.lights[slot].lightCone = 3.1415f * angle / 180.0f;;
  
    updateBuffer = true;
    //Add light component to entity
    LightComponent* lightComponent = registry.AddComponent<LightComponent>(entity);
    lightComponent->slot = slot;
    return slot;
}

void RemoveLight(EntityID& entity)
{
    //Get the light component
    LightComponent* light = registry.AddComponent<LightComponent>(entity);
    //Set type to inactive
    if (light == nullptr || lightShaderBuffer.lights[light->slot].type == 0)
    {
        return;
    }
    lightShaderBuffer.lights[light->slot].type = 0;
    //Push back slot to stack
    PushSlotStack(light->slot);

    updateBuffer = true;
}