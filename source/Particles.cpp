#include "Particles.h"
#include "D3D11Helper.h"
#include "D3D11Graphics.h"
#include "MemLib/MemLib.hpp"

PoolPointer<Particle> m_particles;

int8_t m_computeShaders[8]; 
int8_t m_ACBuffer[2];		

void InitializeParticles()
{
	//NOTE TODO: DONT USE HARDCODED VALUES
	m_particles = MemLib::palloc(sizeof(Particle)*100);

	m_ACBuffer[0] = CreateUnorderedAcessView(NULL, sizeof(Particle), RESOURCE_BUFFER, 0);
	m_ACBuffer[1] = CreateUnorderedAcessView(NULL, sizeof(Particle), RESOURCE_BUFFER, 0);
}
