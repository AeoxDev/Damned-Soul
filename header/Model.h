#pragma once
#include <cinttypes>
#include "MemLib\PoolPointer.hpp"
#include "MemLib\ML_Vector.hpp"
struct Model;

//extern ML_Vector<Model> models;


struct VertexBoneless
{
	float m_position[4];
	float m_normal[4];
	float m_uv[2];
};

struct SubMesh
{
	uint16_t m_material = 0;
	uint32_t m_start = 0;
	uint32_t m_end = 0;
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
	const uint32_t m_sanityCheckNumber;
	const uint32_t m_numSubMeshes;
	const uint32_t m_numMaterials;
	const uint32_t m_numVertices;
	const uint32_t m_numIndices;
	const char m_data[];//Array is intentional, ignore warning

	const bool ValidByteData() const;
	const SubMesh& GetSubMesh(const size_t idx) const;
	const Material& GetMaterial(const size_t idx) const;
	const VertexBoneless* GetVertices() const;
	const uint32_t* GetIndices() const;
};

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

	// Render all the model's submeshes one after another
	void RenderAllSubmeshes();

	void Free();
};