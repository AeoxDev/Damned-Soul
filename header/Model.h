#pragma once
#include <cinttypes>
#include "MemLib\PoolPointer.hpp"

struct VertexBoneless
{
	float m_position[4];
	float m_normal[4];
	float m_uv[2];
};

struct ModelBonelss
{
	uint32_t m_numVertices;
	VertexBoneless m_vertices[];
};

struct ModelIndices
{
	uint32_t m_numIndices;
	uint32_t m_indices[];
};

struct Model
{
	PoolPointer<ModelBonelss> m_bonelessModel;
	PoolPointer<ModelIndices> m_indices;
	uint32_t m_vertexBuffer = -1, m_indexBuffer = -1;

	// Load a set of .mdl and .idx files (other file types possibly to be added in the future
	// The file ending is NOT included in the filepath.
	bool Load(const char* filepathWithoutEnding);

	// Set the currently active index and vertex buffers to this model
	bool SetVertexAndIndexBuffersActive() const;

	void Free();
};