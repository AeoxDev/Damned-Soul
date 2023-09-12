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
	#define MAT_TEX_NAME_LEN 40
	char albedo[MAT_TEX_NAME_LEN];
	char normal[MAT_TEX_NAME_LEN];
	char glow[MAT_TEX_NAME_LEN];
	float roughness;
	float exponent;
};

struct ModelBonelss
{
	uint32_t m_numMaterials;
	uint32_t m_numVertices;
	uint32_t m_numIndices;
	char m_data[];

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
	PoolPointer<ModelBonelss> m_bonelessModel;
	uint32_t m_vertexBuffer = -1, m_indexBuffer = -1;

	// Load a .mdl file
	// No other file formats are supported!
	bool Load(const char* filename);

	// Set the currently active index and vertex buffers to this model
	bool SetVertexAndIndexBuffersActive() const;

	void Free();
};