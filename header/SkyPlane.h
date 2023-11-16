#pragma once
#include "EntityID.h"
#include "D3D11Helper\IDX_Types.h"

extern  EntityID m_skyPlane;
extern  RS_IDX m_skyPlaneRasterizer;
extern  DSV_IDX m_skyPlaneDepth;
extern  VS_IDX m_skyVS;
extern  PS_IDX m_skyPS;
extern	CB_IDX m_skyConst;

void InitializeSky();
void ReleaseSky();
void SwitchCamera();
void UpdateTransform();

