#pragma once
#include "EntityID.h"
#include "D3D11Helper\IDX_Types.h"
#include "MemLib\ML_Array.hpp"

#define AMOUNT_OF_PLANES 3

extern EntityID planes[AMOUNT_OF_PLANES];

extern EntityID m_basePlane;
extern EntityID m_rocksPlane;
extern EntityID m_islandPlane;

extern  RS_IDX m_skyPlaneRasterizer;
extern  DSS_IDX m_skyPlaneStencil;
extern  VS_IDX m_skyVS;
extern  PS_IDX m_skyPS;
extern  PS_IDX m_skyPSForeground;
extern	CB_IDX m_skyConst;
extern	BS_IDX m_skyBlend;
extern SRV_IDX m_skyBackBufferSRV;
extern SRV_IDX m_skyBackBufferSRVCopy;


void InitializeSky();
void ReleaseSky();
void SwitchCamera();
void UpdateTransform(int layerNumber);

