#pragma once
#include <stdint.h>
//#include <d3d11.h>

//struct LightingStruct
//{
//	DirectX::XMFLOAT4	ambientColor; // Kind of like background lighting
//	DirectX::XMFLOAT4	diffuseColor; // Color of the light
//	DirectX::XMFLOAT4	specularColor; // reflection, shiny 
//	float intensity; // focus of the light. Basically tigher/focused vs broader/stofter
//};
//

namespace Light
{
	void SetColor(int type, const float x, const float y, const float z);
	void SetPosition(int type, const float x, const float y, const float z);
	void SetDirection(int type, const float x, const float y, const float z);
	void SetRange(int type, const float range);
	void SetSpotLightCone(const float angle);
	
	/*DirectX::XMVECTOR GetColor(int type);
	DirectX::XMVECTOR GetPosition(int type);
	DirectX::XMVECTOR GetDirection(int type);*/
	
	int16_t GetLightBufferIndex(int type);
	int16_t GetLightBufferIndex();

	/// <summary>
	/// Initializes the light with default values
	/// Color(1,1,1) = white
	/// Direction(0,0,1) Only held by DirectionalLights and SpotLights
	/// Position(0,0,0)	 Only held by PointLights and SpotLights
	/// Range(10)		 Only held by PointLights and SpotLights
	/// ConeAngle(90)    Only held by SpotLights
	/// </summary>
	/// 
	
	void SetColorHue(const float& multiplicativeRed, const float& multiplicativeGreen, const float& multiplicativeBlue, const float& additiveRed, const float& additiveGreen, const float& additiveBlue);
	
	void CreateLight(int type);
	void SetupLight();
	void UpdateLight();
	void FreeLight();

	
}