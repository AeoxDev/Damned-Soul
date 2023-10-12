#include "Light.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"
#include "D3D11Helper.h"

//struct LightingStruct
//{
//	DirectX::XMFLOAT4	ambientColor; // Kind of like background lighting
//	DirectX::XMFLOAT4	diffuseColor; // Color of the light
//	DirectX::XMFLOAT4	specularColor; // reflection, shiny 
//	float intensity; // focus of the light. Basically tigher/focused vs broader/stofter
//};


struct DirectionLightStruct //1 // also buffer?
{
    DirectX::XMFLOAT4 m_dirLightColor;
    DirectX::XMFLOAT4 m_dirLightDirection;
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

int16_t m_directionLightBufferIndex;
DirectionLightStruct DirLight;

int16_t m_pointLightBufferIndex;

PointLightStruct PointLight;

int16_t m_spotLightBufferIndex;

SpotLightStruct SpotLight;

void Light::SetColor(int type, const float x, const float y, const float z)
{
    if (type == 1)
    {
        DirLight.m_dirLightColor.x = x;
        DirLight.m_dirLightColor.y = y;
        DirLight.m_dirLightColor.z = z;
        UpdateConstantBuffer(Light::GetLightBufferIndex(1), &DirLight);
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
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light" << std::endl;
        std::cout << "2 = Point light" << std::endl;
        std::cout << "3 = Spot light" << std::endl;

    }
}

void Light::SetPosition(int type, const float x, const float y, const float z)
{
    if (type == 1)
    {
        std::cout << "Directional lights don´t hold a position" << std::endl;
    }
    else if(type == 2)
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
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light(Do not have a position)" << std::endl;
        std::cout << "2 = Point light" << std::endl;
        std::cout << "3 = Spot light" << std::endl; 
    }
}

void Light::SetDirection(int type, const float x, const float y, const float z)
{
    if (type == 1)
    {
        float length = sqrtf(x * x + y * y + z * z);
        DirLight.m_dirLightDirection.x = x/ length;
        DirLight.m_dirLightDirection.y = y/ length;
        DirLight.m_dirLightDirection.z = z/ length;
        UpdateConstantBuffer(Light::GetLightBufferIndex(1), &DirLight);
    }
    else if (type == 2)
    {
        std::cout << "Point lights don´t hold directions" << std::endl;

    }
    else if (type == 3)
    {
        float length = sqrtf(x * x + y * y + z * z);
        SpotLight.m_spotLightDirection.x = x/ length;
        SpotLight.m_spotLightDirection.y = y/ length;
        SpotLight.m_spotLightDirection.z = z/ length;
        UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);
    }
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light" << std::endl;
        std::cout << "2 = Point light(Do not have a direction)" << std::endl;
        std::cout << "3 = Spot light" << std::endl;
    }
}

void Light::SetRange(int type, const float range)
{
    if (type == 1)
    {
        std::cout << "Directional lights don´t hold range" << std::endl;
    }
    else if (type == 2)
    {
        PointLight.m_pointLightRange.x = range;
        UpdateConstantBuffer(Light::GetLightBufferIndex(2), &PointLight);
    }
    else if (type == 3)
    {
        SpotLight.m_spotLightRange.x = range;
        UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);
    }
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light(Do not have range)" << std::endl;
        std::cout << "2 = Point light" << std::endl;
        std::cout << "3 = Spot light" << std::endl;
    }
}

void Light::SetSpotLightCone(const float angle)
{
    SpotLight.m_spotLightCone.x = 3.1415f * angle / 180.0f;
    UpdateConstantBuffer(Light::GetLightBufferIndex(3), &SpotLight);
}

DirectX::XMVECTOR Light::GetColor(int type)
{
    if (type == 1)
    {
        return DirectX::XMLoadFloat4(&DirLight.m_dirLightColor); 
    }
    else if (type == 2)
    {
        return DirectX::XMLoadFloat4(&PointLight.m_pointLightColor);
    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight.m_spotLightColor);
    }
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light" << std::endl;
        std::cout << "2 = Point light" << std::endl;
        std::cout << "3 = Spot light" << std::endl;
    }
}

DirectX::XMVECTOR Light::GetPosition(int type)
{
    if (type == 1)
    {
        std::cout << "Directional lights don´t hold a position" << std::endl;
    }
    else if (type == 2)
    {
        return DirectX::XMLoadFloat4(&PointLight.m_pointLightPosition);

    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight.m_spotLightPosition);
    }
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light" << std::endl;
        std::cout << "2 = Point light(Do not have a direction)" << std::endl;
        std::cout << "3 = Spot light" << std::endl;
    }
}

DirectX::XMVECTOR Light::GetDirection(int type)
{
    if (type == 1)
    {
        return DirectX::XMLoadFloat4(&DirLight.m_dirLightDirection);
        
    }
    else if (type == 2)
    {
        std::cout << "Point lights don´t hold a direction" << std::endl;

    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight.m_spotLightDirection);
    }
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light" << std::endl;
        std::cout << "2 = Point light(Do not have a direction)" << std::endl;
        std::cout << "3 = Spot light" << std::endl;
    }
}

int16_t Light::GetLightBufferIndex(int type)
{
    if (type == 1)
    {
        return m_directionLightBufferIndex;

    }
    else if (type == 2)
    {
        return m_pointLightBufferIndex;

    }
    else if (type == 3)
    {
        return m_spotLightBufferIndex;
    }
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light" << std::endl;
        std::cout << "2 = Point light" << std::endl;
        std::cout << "3 = Spot light" << std::endl;
    }

}

void Light::CreateLight(int type) //inte klar --constantbuffer
{
    if (type == 1)
    {
        m_directionLightBufferIndex = CreateConstantBuffer(&(DirLight.m_dirLightColor), sizeof(DirectionLightStruct));
        SetColor(type, 1.0f, 1.0f, 1.0f);
        SetDirection(type, -1.0f, -1.0f, 1.0f);

        //DirLight = MemLib::palloc(sizeof(DirectionLightStruct))
      
        //Default done, update now

        //Prepare the buffer to creation
 
        
        UpdateConstantBuffer(m_directionLightBufferIndex, &(DirLight.m_dirLightColor));
    

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
    else
    {
        std::cout << "Did you use a correct type int?" << std::endl;
        std::cout << "1 = Directional light" << std::endl;
        std::cout << "2 = Point light" << std::endl;
        std::cout << "3 = Spot light" << std::endl;
    }
}

void Light::FreeLight()
{
    //MemLib::pfree(DirLight);
    //MemLib::pfree(PointLight);
    //MemLib::pfree(SpotLight);



}
