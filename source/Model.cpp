#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MemLib\MemLib.hpp"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "STB_Helper.h"

const Material& ModelBoneless::GetMaterial(const size_t idx) const
{
	return ((Material*)m_data)[idx];
}

const VertexBoneless* ModelBoneless::GetVertices() const
{
	return (VertexBoneless*)(&m_data[m_numMaterials * sizeof(Material)]);
}

const uint32_t* ModelBoneless::GetIndices() const
{
	return (uint32_t*)(&m_data[m_numMaterials * sizeof(Material) + m_numVertices * sizeof(VertexBoneless)]);
}


bool Model::Load(const char* filename)
{
	std::string name = "Models\\Mdl\\";
	name.append(filename);

	std::ifstream reader;
	auto flags = std::ios::binary; // | std::ios::ate;
	reader.open(name, flags);

	if (false == reader.is_open())
	{
		std::cerr << "Failed to open model for \"" << filename << "\"! Please verify file!" << std::endl;
		return false;
	}

	// Allocate temporarily onto the stack
	reader.seekg(0, std::ios::end);
	unsigned int size = static_cast<unsigned int>(reader.tellg());
	char* modelData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(modelData, size);
	reader.close();

	// Copy data to the boneless model
	m_bonelessModel = MemLib::palloc(size);
	std::memcpy(&(*m_bonelessModel), modelData, size);

	// pop the stack
	MemLib::spop();

	//float fun[600][2];
	//for (unsigned int i = 0; i < m_bonelessModel->m_numVertices; ++i)
	//{
	//	fun[i][0] = m_bonelessModel->GetVertices()[i].m_uv[0];
	//	fun[i][1] = m_bonelessModel->GetVertices()[i].m_uv[1];
	//}

	m_vertexBuffer = CreateVertexBuffer(m_bonelessModel->GetVertices(), sizeof(VertexBoneless), m_bonelessModel->m_numVertices);
	m_indexBuffer = CreateIndexBuffer(m_bonelessModel->GetIndices(), sizeof(uint32_t), m_bonelessModel->m_numIndices);

	for (unsigned int i = 0; i < m_bonelessModel->m_numMaterials; ++i)
	{
		// Same operation as GetMaterial(size_t), but not const
		Material& mat = ((Material*)m_bonelessModel->m_data)[i];
		// Load colors
		mat.albedoIdx = LoadTexture(mat.albedo);
		// Load normal map
		mat.normalIdx = LoadTexture(mat.normal);
		// Load glow map
		mat.glowIdx = LoadTexture(mat.glow);
	}

	return true;
}

void Model::Free()
{
	if (false == m_bonelessModel.IsNullptr())
		MemLib::pfree(m_bonelessModel);
}


bool Model::SetMaterialActive() const
{
	const Material& mat = (**m_bonelessModel.m_pp).GetMaterial(0);
	if (SetTexture(mat.albedoIdx, 0, BIND_PIXEL)/* &&
		SetTexture(mat.normalIdx, 1, BIND_PIXEL) &&
		SetTexture(mat.glowIdx, 2, BIND_PIXEL)*/)
		return true;
	return false;
}

// Set the currently active index and vertex buffers to this model
bool Model::SetVertexAndIndexBuffersActive() const
{
	if (false == SetVertexBuffer(m_vertexBuffer))
		return false;
	if (false == SetIndexBuffer(m_indexBuffer))
		return false;
	return true;
}