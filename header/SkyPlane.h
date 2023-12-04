#pragma once
#include "EntityID.h"
#include "D3D11Helper\IDX_Types.h"
#include "MemLib\ML_Array.hpp"

#define AMOUNT_OF_PLANES 3

extern EntityID m_basePlane;


extern  RS_IDX m_skyPlaneRasterizer;
extern  DSS_IDX m_skyPlaneStencil;
extern  VS_IDX m_skyVS;
extern  PS_IDX m_skyPS;
extern	CB_IDX m_skyConst;
extern	BS_IDX m_skyBlend;


void InitializeSky();
void ReleaseSky();

