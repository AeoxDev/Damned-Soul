#pragma once
#include "EntityID.h"

struct GlowComponent
{
	float m_r;
	float m_g;
	float m_b;
	//float m_intensity; Maybe?

	GlowComponent(float r, float g, float b)
	{
		m_r = r;
		m_g = g;
		m_b = b;
	}
};