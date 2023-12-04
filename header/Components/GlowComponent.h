#pragma once
#include "EntityID.h"

// float r, float g,float b
struct GlowComponent
{
	float m_r;
	float m_g;
	float m_b;

	GlowComponent(float r, float g, float b)
	{
		m_r = r;
		m_g = g;
		m_b = b;
	}
}; 