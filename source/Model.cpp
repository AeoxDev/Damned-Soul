#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MemLib\MemLib.hpp"
#include "D3D11Helper.h"
#include "GameRenderer.h"

const Material& ModelBonelss::GetMaterial(const size_t idx) const
{
	return ((Material*)m_data)[idx];
}

const VertexBoneless* ModelBonelss::GetVertices() const
{
	return (VertexBoneless*)(&m_data[m_numMaterials * sizeof(Material)]);
}

const uint32_t* ModelBonelss::GetIndices() const
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

	m_vertexBuffer = CreateVertexBuffer(m_bonelessModel->GetVertices(), sizeof(VertexBoneless), m_bonelessModel->m_numVertices);
	m_indexBuffer = CreateIndexBuffer(m_bonelessModel->GetIndices(), sizeof(uint32_t), m_bonelessModel->m_numIndices);
}

void Model::Free()
{
	if (false == m_bonelessModel.IsNullptr())
		MemLib::pfree(m_bonelessModel);
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