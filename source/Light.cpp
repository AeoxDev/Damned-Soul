#include "Light.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "MemLib/ML_Vector.hpp"

#include "D3D11Helper.h"
#include <assert.h>
#include "LightComponent.h"

#include "EntityFramework.h"
#include "Registry.h"
#include <DirectXMath.h>
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
    int slot = -1;
};

struct LightOffset
{
    DirectX::XMFLOAT4 lightDirectionOffset;
    DirectX::XMFLOAT4 lightPositionOffset;
    float angleY = 0.0f;
};

struct LightShaderData
{
    int type = 0;// 1 pointlight, 2 spotlight
    float fallofFactor = 1.0f;//attenuation
    float lightRange = 1.0f;
    float lightCone;
    DirectX::XMFLOAT4 lightColor;
    DirectX::XMFLOAT4 lightDirection;//Post transform direction
    DirectX::XMFLOAT4 lightPosition;//Post transform position
   
};

struct LightComponentShaderBuffer
{
    int firstLight = 0;//First light in array
    int lastLight = 31;//Last light in array
    float padding1 = 0.0;
    float padding2 = 0.0;
    DirectX::XMFLOAT4 dirLightColor;
    DirectX::XMFLOAT4 dirLightDirection;
    DirectX::XMFLOAT4 colorMultiplier;
    DirectX::XMFLOAT4 colorAdditive;
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
LightOffset lightOffsets[LIGHT_COMPONENT_ARRAY_LIMIT];
int16_t lightBufferIndex;
bool updateBuffer = false;
//ML_Vector<int> *freeSlot;//Stack of all free slots in the light component array
int lastSlot = 0;
int freeSlotStack[LIGHT_COMPONENT_ARRAY_LIMIT];

int PopSlotStack()
{
    lastSlot -= 1;
    return freeSlotStack[lastSlot];
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

int16_t Light::GetLightBufferIndex(int type)
{
    assert(type > 1 && type <= 3);
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

void Light::SetColorHue(const float& multiplicativeRed, const float& multiplicativeGreen, const float& multiplicativeBlue, const float& additiveRed, const float& additiveGreen, const float& additiveBlue)
{
    //Only do GPU call if there is a need to update the hue to avoid communication with GPU.
    if (lightShaderBuffer.colorMultiplier.x != multiplicativeRed || lightShaderBuffer.colorMultiplier.y != multiplicativeGreen || lightShaderBuffer.colorMultiplier.z != multiplicativeBlue ||
        lightShaderBuffer.colorAdditive.x != additiveRed || lightShaderBuffer.colorAdditive.y != additiveGreen || lightShaderBuffer.colorAdditive.z != additiveBlue)
    {
        lightShaderBuffer.colorMultiplier.x = multiplicativeRed;
        lightShaderBuffer.colorMultiplier.y = multiplicativeGreen;
        lightShaderBuffer.colorMultiplier.z = multiplicativeBlue;
        lightShaderBuffer.colorAdditive.x = additiveRed;
        lightShaderBuffer.colorAdditive.y = additiveGreen;
        lightShaderBuffer.colorAdditive.z = additiveBlue;
        updateBuffer = true;
        UpdateLight();
    }   
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

        //DirLight = MemLib::palloc(sizeof(GlobalShaderBuffer))
      
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
    //Set the offsets
     //Set the offsets
    lightOffsets[slot].lightDirectionOffset.z = -1.0f;

    lightOffsets[slot].lightPositionOffset.x = positionX;
    lightOffsets[slot].lightPositionOffset.y = positionY;
    lightOffsets[slot].lightPositionOffset.z = positionZ;
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
    //Set the offsets
    lightOffsets[slot].lightDirectionOffset.x = directionX;
    lightOffsets[slot].lightDirectionOffset.y = directionY;
    lightOffsets[slot].lightDirectionOffset.z = directionZ;
    lightOffsets[slot].lightPositionOffset.x = positionX;
    lightOffsets[slot].lightPositionOffset.y = positionY;
    lightOffsets[slot].lightPositionOffset.z = positionZ;
    lightShaderBuffer.lights[slot].lightCone = 3.1415f * angle / 180.0f;
  
    updateBuffer = true;
    //Add light component to entity
    LightComponent* lightComponent = registry.AddComponent<LightComponent>(entity);
    lightComponent->slot = slot;
    return slot;
}

void RemoveLight(EntityID& entity)
{
    //Get the light component
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    //Set type to inactive
    if (light == nullptr || light->slot < 0 || lightShaderBuffer.lights[light->slot].type == 0)
    {
        //light->slot = -1;
        return;
    }
    lightShaderBuffer.lights[light->slot].type = 0;
    //Push back slot to stack
    PushSlotStack(light->slot);
    //Make sure to set the slot to -1 so that new lights wont be removed by an old one.
    light->slot = -1;
    updateBuffer = true;
}

void OffsetPosition(EntityID& entity, const float& x, const float& y, const float& z)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    
    //If there is actual movement, then do a gpu call
    if (lightShaderBuffer.lights[light->slot].lightPosition.x != lightOffsets[light->slot].lightPositionOffset.x + x ||
        lightShaderBuffer.lights[light->slot].lightPosition.y != lightOffsets[light->slot].lightPositionOffset.y + y ||
        lightShaderBuffer.lights[light->slot].lightPosition.z != lightOffsets[light->slot].lightPositionOffset.z + z)
    {
        //Offset position by x, y, z respectively.
        lightShaderBuffer.lights[light->slot].lightPosition.x = lightOffsets[light->slot].lightPositionOffset.x + x;
        lightShaderBuffer.lights[light->slot].lightPosition.y = lightOffsets[light->slot].lightPositionOffset.y + y;
        lightShaderBuffer.lights[light->slot].lightPosition.z = lightOffsets[light->slot].lightPositionOffset.z + z;
        updateBuffer = true;
    }
   
}
void OffsetFacing(EntityID& entity, const float& x, const float& y, const float& z)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);

    float angleY = acosf(x);

    if (z < 0.0f)
    {
        angleY *= -1.0f;
    }
    //Check if angle is changed or not
    if (angleY != lightOffsets[light->slot].angleY)
    {
        float offsetXx = -sinf(angleY);
        float offsetZz = -sinf(angleY);
        float offsetXz = -cosf(angleY);
        float offsetZx = cosf(angleY);
        lightShaderBuffer.lights[light->slot].lightDirection.x = lightOffsets[light->slot].lightDirectionOffset.x * offsetXx + lightOffsets[light->slot].lightDirectionOffset.z * offsetXz;
        lightShaderBuffer.lights[light->slot].lightDirection.z = lightOffsets[light->slot].lightDirectionOffset.z * offsetZz + lightOffsets[light->slot].lightDirectionOffset.x * offsetZx;
        float length = sqrtf((lightShaderBuffer.lights[light->slot].lightDirection.x * lightShaderBuffer.lights[light->slot].lightDirection.x)
            + (lightShaderBuffer.lights[light->slot].lightDirection.y * lightShaderBuffer.lights[light->slot].lightDirection.y)
            + (lightShaderBuffer.lights[light->slot].lightDirection.z * lightShaderBuffer.lights[light->slot].lightDirection.z));
        lightShaderBuffer.lights[light->slot].lightDirection.x /= length;
        lightShaderBuffer.lights[light->slot].lightDirection.y /= length;
        lightShaderBuffer.lights[light->slot].lightDirection.z /= length;
        updateBuffer = true;
    }
    lightOffsets[light->slot].angleY = angleY;
}

//Getters
float3 GetLightPosition(const EntityID& entity)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    float3 toReturn = { lightOffsets[light->slot].lightPositionOffset.x,
    lightOffsets[light->slot].lightPositionOffset.y,
    lightOffsets[light->slot].lightPositionOffset.z };
    return toReturn;
}
float3 GetLightColor(const EntityID& entity)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    float3 toReturn = { lightShaderBuffer.lights[light->slot].lightColor.x,
    lightShaderBuffer.lights[light->slot].lightColor.y,
    lightShaderBuffer.lights[light->slot].lightColor.z };
    return toReturn;
}
float3 GetLightDirection(const EntityID& entity)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    float3 toReturn = { lightOffsets[light->slot].lightDirectionOffset.x,
    lightOffsets[light->slot].lightDirectionOffset.y,
    lightOffsets[light->slot].lightDirectionOffset.z };
    return toReturn;
}
float GetLightRange(const EntityID& entity)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    return lightShaderBuffer.lights[light->slot].lightRange;
}
float GetLightFallofFactor(const EntityID& entity)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    return lightShaderBuffer.lights[light->slot].fallofFactor;
}
float GetLightAngle(const EntityID& entity)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    return lightShaderBuffer.lights[light->slot].lightCone;
}

//Setters
void SetLightPosition(const EntityID& entity, const float& x, const float& y, const float& z)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    lightOffsets[light->slot].lightPositionOffset.x = x;
    lightOffsets[light->slot].lightPositionOffset.y = y;
    lightOffsets[light->slot].lightPositionOffset.z = z;
    updateBuffer = true;
}
void SetLightColor(const EntityID& entity, const float& x, const float& y, const float& z)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    lightShaderBuffer.lights[light->slot].lightColor.x = x;
    lightShaderBuffer.lights[light->slot].lightColor.y = y;
    lightShaderBuffer.lights[light->slot].lightColor.z = z;
    updateBuffer = true;
}
void SetLightDirection(const EntityID& entity, const float& x, const float& y, const float& z)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    lightOffsets[light->slot].lightDirectionOffset.x = x;
    lightOffsets[light->slot].lightDirectionOffset.y = y;
    lightOffsets[light->slot].lightDirectionOffset.z = z;
    updateBuffer = true;
}
void SetLightRange(const EntityID& entity, const float& range)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    lightShaderBuffer.lights[light->slot].lightRange = range;
    updateBuffer = true;
}
void SetLightFallofFactor(const EntityID& entity, const float& fallof)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    lightShaderBuffer.lights[light->slot].lightRange = fallof;
    updateBuffer = true;
}
void SetLightAngle(const EntityID& entity, const float& angle)
{
    LightComponent* light = registry.GetComponent<LightComponent>(entity);
    assert(light != nullptr);
    lightShaderBuffer.lights[light->slot].lightCone = 3.1415f * angle / 180.0f;
    updateBuffer = true;
}