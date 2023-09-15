#pragma once
#include <cinttypes>
#include "MemLib\PoolPointer.hpp"

struct VertexBoneless
{
	float m_position[4];
	float m_normal[4];
	uint32_t m_material;
	float m_uv[2];
};

struct Material
{
	#define MAT_TEX_NAME_LEN 64
	char albedo[MAT_TEX_NAME_LEN];
	int16_t albedoIdx = -1;
	char normal[MAT_TEX_NAME_LEN];
	int16_t normalIdx = -1;
	char glow[MAT_TEX_NAME_LEN];
	int16_t glowIdx = -1;
	float roughness = 0.5f;
	float exponent = 1.0f;
};

struct ModelBoneless
{
	uint32_t m_numMaterials;
	uint32_t m_numVertices;
	uint32_t m_numIndices;
	char m_data[];//Array is intentional, ignore warning

	const Material& GetMaterial(const size_t idx) const;
	const VertexBoneless* GetVertices() const;
	const uint32_t* GetIndices() const;
};

//struct ModelIndices
//{
//	uint32_t m_numIndices;
//	uint32_t m_indices[];
//};

struct Model
{
	PoolPointer<ModelBoneless> m_bonelessModel;
	uint32_t m_vertexBuffer = -1, m_indexBuffer = -1;

	// Load a .mdl file
	// No other file formats are supported!
	bool Load(const char* filename);

	bool SetMaterialActive() const;

	// Set the currently active index and vertex buffers to this model
	bool SetVertexAndIndexBuffersActive() const;

	void Free();
};