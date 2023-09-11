#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MemLib\MemLib.hpp"
#include "D3D11Helper.h"
#include "GameRenderer.h"

bool Model::Load(const char* filepathWithoutEnding)
{
	std::string name = filepathWithoutEnding;
	name.append(".mdl");

	std::ifstream reader;
	auto flags = std::ios::binary; // | std::ios::ate;
	reader.open(name, flags);

	if (false == reader.is_open())
	{
		std::cerr << "Failed to open model for \"" << filepathWithoutEnding << "\"! Please verify file!" << std::endl;
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

	name = filepathWithoutEnding;
	name.append(".idx");
	reader.open(name, flags);

	if (false == reader.is_open())
	{
		std::cerr << "Failed to open index data for for \"" << filepathWithoutEnding << "\"! Please verify file!" << std::endl;
		return false;
	}

	// Allocate temporarily onto the stack
	reader.seekg(0, std::ios::end);
	size = static_cast<unsigned int>(reader.tellg());
	char* indexData = (char*)MemLib::spush(size);
	reader.seekg(0, std::ios::beg);

	// Read byte data onto stack
	reader.read(indexData, size);
	reader.close();

	// Copy data to the boneless model
	m_indices = MemLib::palloc(size);
	std::memcpy(&(*m_indices), indexData, size);

	// pop the stack
	MemLib::spop();

	m_vertexBuffer = CreateVertexBuffer(m_bonelessModel->m_vertices, sizeof(VertexBoneless), m_bonelessModel->m_numVertices);
	m_indexBuffer = CreateIndexBuffer(m_indices->m_indices, sizeof(uint32_t), m_indices->m_numIndices);
}

void Model::Free()
{
	if (false == m_bonelessModel.IsNullptr())
		MemLib::pfree(m_bonelessModel);
	if (false == m_indices.IsNullptr())
		MemLib::pfree(m_indices);
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