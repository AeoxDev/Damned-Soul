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

	GlowComponent(float rgb[3])
	{
		m_r = rgb[0];
		m_g = rgb[1];
		m_b = rgb[2];
	}
}; 