#include "Shatter.h"
#include "D3D11Helper/D3D11Helper.h"

CB_IDX Shatter::cBuffer;
GS_IDX Shatter::gShader;

void Shatter::Initialize()
{
	ShatterComponent tempData = ShatterComponent(1.f);
	Shatter::cBuffer = CreateConstantBuffer(&tempData, sizeof(ShatterComponent));
	Shatter::gShader = LoadGeometryShader("ShatterGS.cso");
}

void Shatter::SetResources(ShatterComponent* comp)
{
	UpdateConstantBuffer(Shatter::cBuffer, comp);
	SetConstantBuffer(Shatter::cBuffer, SHADER_TO_BIND_RESOURCE::BIND_GEOMETRY, 2);
	SetGeometryShader(Shatter::gShader);
}

void Shatter::UnsetResources()
{
	UnsetConstantBuffer(SHADER_TO_BIND_RESOURCE::BIND_GEOMETRY, 2);
	UnsetGeometryShader();
}
