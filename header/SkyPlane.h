#pragma once
#include "EntityID.h"


struct SkyPlane
{
	EntityID m_skyPlane;
	TX_IDX texture;

	SkyPlane();
	void Release();


};