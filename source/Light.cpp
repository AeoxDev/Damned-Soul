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
PoolPointer<PointLightStruct> PointLight;

int16_t m_spotLightBufferIndex;
PoolPointer<SpotLightStruct> SpotLight;


void Light::SetColor(int type, const float x, const float y, const float z)
{
    if (type == 1)
    {
        DirLight.m_dirLightColor.x = x;
        DirLight.m_dirLightColor.y = y;
        DirLight.m_dirLightColor.z = z;



    }
    else if (type == 2)
    {
        PointLight->m_pointLightColor.x = x;
        PointLight->m_pointLightColor.y = y;
        PointLight->m_pointLightColor.z = z;

        PointLight->m_pointLightColor = DirectX::XMFLOAT4(PointLight->m_pointLightColor.x, PointLight->m_pointLightColor.y, PointLight->m_pointLightColor.z, 1.f);

    }
    else if (type == 3)
    {
        SpotLight->m_spotLightColor.x = x;
        SpotLight->m_spotLightColor.y = y;
        SpotLight->m_spotLightColor.z = z;

        SpotLight->m_spotLightColor = DirectX::XMFLOAT4(SpotLight->m_spotLightColor.x, SpotLight->m_spotLightColor.y, SpotLight->m_spotLightColor.z, 1.f);
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
        PointLight->m_pointLightPosition.x = x;
        PointLight->m_pointLightPosition.y = y;
        PointLight->m_pointLightPosition.z = z;

        PointLight->m_pointLightPosition = DirectX::XMFLOAT4(PointLight->m_pointLightPosition.x, PointLight->m_pointLightPosition.y, PointLight->m_pointLightPosition.z, 1.f);

    }
    else if (type == 3)
    {
        SpotLight->m_spotLightPosition.x = x;
        SpotLight->m_spotLightPosition.y = y;
        SpotLight->m_spotLightPosition.z = z;

        SpotLight->m_spotLightPosition = DirectX::XMFLOAT4(SpotLight->m_spotLightPosition.x, SpotLight->m_spotLightPosition.y, SpotLight->m_spotLightPosition.z, 1.f);
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
        DirLight.m_dirLightDirection.x = x;
        DirLight.m_dirLightDirection.y = y;
        DirLight.m_dirLightDirection.z = z;

       /* DirLight->m_dirLightDirection = DirectX::XMFLOAT4(DirLight->m_dirLightDirection.x, DirLight->m_dirLightDirection.y, DirLight->m_dirLightDirection.z, 1.f);*/
    }
    else if (type == 2)
    {
        std::cout << "Point lights don´t hold directions" << std::endl;

    }
    else if (type == 3)
    {
        SpotLight->m_spotLightDirection.x = x;
        SpotLight->m_spotLightDirection.y = y;
        SpotLight->m_spotLightDirection.z = z;

        SpotLight->m_spotLightDirection = DirectX::XMFLOAT4(SpotLight->m_spotLightDirection.x, SpotLight->m_spotLightDirection.y, SpotLight->m_spotLightDirection.z, 1.f);
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
        PointLight->m_pointLightRange.x = range;

        PointLight->m_pointLightRange = DirectX::XMFLOAT4(PointLight->m_pointLightRange.x, 0, 0, 1.f);

    }
    else if (type == 3)
    {
        SpotLight->m_spotLightRange.x = range;
       
        SpotLight->m_spotLightRange = DirectX::XMFLOAT4(SpotLight->m_spotLightRange.x, 0, 0, 1.f);
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
    SpotLight->m_spotLightCone.x = angle;

    SpotLight->m_spotLightCone = DirectX::XMFLOAT4(SpotLight->m_spotLightCone.x, 0, 0, 1.f);
}

DirectX::XMVECTOR Light::GetColor(int type)
{
    if (type == 1)
    {
        return DirectX::XMLoadFloat4(&DirLight.m_dirLightColor); 
    }
    else if (type == 2)
    {
        return DirectX::XMLoadFloat4(&PointLight->m_pointLightColor);
    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight->m_spotLightColor);
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
        return DirectX::XMLoadFloat4(&PointLight->m_pointLightPosition);

    }
    else if (type == 3)
    {
        return DirectX::XMLoadFloat4(&SpotLight->m_spotLightPosition);
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
        return DirectX::XMLoadFloat4(&SpotLight->m_spotLightDirection);
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
        SetColor(type, 1.0f, 1.0f, 1.0f);
        SetDirection(type, -1.0f, -2.0f, 0.0f);

        //DirLight = MemLib::palloc(sizeof(DirectionLightStruct));

      /*  DirLight = MemLib::palloc(sizeof(DirectionLightStruct));*/ //behövs bara en
      
        //Default done, update now

        //Prepare the buffer to creation
 
        DirLight.m_dirLightColor = DirectX::XMFLOAT4(DirLight.m_dirLightColor.x, DirLight.m_dirLightColor.y, DirLight.m_dirLightColor.z, 1.0f);
        DirLight.m_dirLightDirection = DirectX::XMFLOAT4(DirLight.m_dirLightDirection.x, DirLight.m_dirLightDirection.y, DirLight.m_dirLightDirection.z, 1.0f);

        m_directionLightBufferIndex = CreateConstantBuffer(&(DirLight.m_dirLightColor), sizeof(DirectionLightStruct), 2);
        
        UpdateConstantBuffer(m_directionLightBufferIndex, &(DirLight.m_dirLightColor));


    }
    else if (type == 2)
    {
        SetColor(type, 1.0f, 1.0f, 1.0f);
        SetPosition(type, 0.0f, 0.0f, 0.0f);
        SetRange(type, 10.0f);
       
        PointLight = MemLib::palloc(sizeof(PointLightStruct));

        PointLight = MemLib::palloc(sizeof(PointLightStruct));

        //Default done, update now

        //Prepare the buffer to creation

        PointLight->m_pointLightColor = DirectX::XMFLOAT4(PointLight->m_pointLightColor.x, PointLight->m_pointLightColor.y, PointLight->m_pointLightColor.z, 1.0f);
        PointLight->m_pointLightPosition = DirectX::XMFLOAT4(PointLight->m_pointLightPosition.x, PointLight->m_pointLightPosition.y, PointLight->m_pointLightPosition.z, 1.0f);
        PointLight->m_pointLightRange = DirectX::XMFLOAT4(PointLight->m_pointLightRange.x, PointLight->m_pointLightRange.y, PointLight->m_pointLightRange.z, 1.0f);

        //PointLight->m_PointLightBufferIndex = CreateConstantBuffer(&(/*DirLight->m_dirLightColor*/), sizeof(PontLightStruct), /*1*/);

        //UpdateConstantBuffer(PointLight->m_pointLightBufferIndex, &(PointLight->m_pointLightColor));

        

    }
    else if (type == 3)
    {
        SetColor(type, 1.0f, 1.0f, 1.0f);
        SetPosition(type, 0.0f, 0.0f, 0.0f);
        SetDirection(type, 0.0f, 0.0f, 0.0f);
        SetRange(type, 10.0f);
        SetSpotLightCone(90.0f);

        SpotLight = MemLib::palloc(sizeof(SpotLightStruct));

        SpotLight = MemLib::palloc(sizeof(SpotLightStruct));

        //Default done, update now

        //Prepare the buffer to creation

        SpotLight->m_spotLightColor = DirectX::XMFLOAT4(SpotLight->m_spotLightColor.x, SpotLight->m_spotLightColor.y, SpotLight->m_spotLightColor.z, 1.0f);
        SpotLight->m_spotLightPosition = DirectX::XMFLOAT4(SpotLight->m_spotLightPosition.x, SpotLight->m_spotLightPosition.y, SpotLight->m_spotLightPosition.z, 1.0f);
        SpotLight->m_spotLightDirection = DirectX::XMFLOAT4(SpotLight->m_spotLightDirection.x, SpotLight->m_spotLightDirection.y, SpotLight->m_spotLightDirection.z, 1.0f);
        SpotLight->m_spotLightRange = DirectX::XMFLOAT4(SpotLight->m_spotLightRange.x, SpotLight->m_spotLightRange.y, SpotLight->m_spotLightRange.z, 1.0f);
        SpotLight->m_spotLightCone = DirectX::XMFLOAT4(SpotLight->m_spotLightCone.x, SpotLight->m_spotLightCone.y, SpotLight->m_spotLightCone.z, 1.0f);

        //SpotLight->m_SpotLightBufferIndex = CreateConstantBuffer(&(SpotLight->m_dirLightColor), sizeof(SpotLightStruct), /*1*/);

        //UpdateConstantBuffer(SpotLight->m_spotLightBufferIndex, &(SpotLight->m_spotLightColor));
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
    MemLib::pfree(PointLight);
    MemLib::pfree(SpotLight);



}
