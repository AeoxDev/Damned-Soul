#include "Model.h"
#include <iostream>
#include <fstream>
#include "MemLib\MemLib.hpp"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "STB_Helper.h"
#include "D3D11Helper.h"
#include "MemLib\ML_String.hpp"

#include "DeltaTime.h"

#define SANE_MODEL_BONELESS_NUMBER (1'234'567'890 | 0b0)
#define SANE_MODEL_BONES_NUMBER (1'234'567'890 | 0b1)

const MODEL_TYPE modelGenericData::ValidByteData() const
{
	switch (m_sanityCheckNumber)
	{
	case SANE_MODEL_BONELESS_NUMBER:
		return MODEL_BONELESS;
	case SANE_MODEL_BONES_NUMBER:
		return MODEL_WITH_BONES;
	default:
		return MODEL_INSANE;
	}
}

const SubMesh& modelGenericData::GetSubMesh(const size_t idx) const
{
	return ((SubMesh*)m_data)[idx];
}

#define SUBMESH_BYTE_SIZE (m_numSubMeshes * sizeof(SubMesh))
const Material& modelGenericData::GetMaterial(const size_t idx) const
{
	return ((Material*)(&(m_data[SUBMESH_BYTE_SIZE])))[idx];
}

#define MATERIAL_BYTE_SIZE (m_numMaterials * sizeof(Material))
const uint32_t* modelGenericData::GetIndices() const
{
	return (uint32_t*)((&m_data[SUBMESH_BYTE_SIZE + MATERIAL_BYTE_SIZE]));
}

#define INDEX_BYTE_SIZE (m_numIndices * sizeof(uint32_t))
const VertexBoneless* modelGenericData::GetBonelessVertices() const
{
	return (VertexBoneless*)(&(m_data[SUBMESH_BYTE_SIZE + MATERIAL_BYTE_SIZE + INDEX_BYTE_SIZE]));
}
const VertexBoned* modelGenericData::GetBonedVertices() const
{
	return (VertexBoned*)(&(m_data[SUBMESH_BYTE_SIZE + MATERIAL_BYTE_SIZE + INDEX_BYTE_SIZE]));
}

#define BONED_VERTEX_BYTE_SIZE (m_numVertices * sizeof(VertexBoned))
DirectX::XMMATRIX* modelGenericData::GetBoneMatrices()
{
	return (DirectX::XMMATRIX*)(&(m_data[SUBMESH_BYTE_SIZE + MATERIAL_BYTE_SIZE + INDEX_BYTE_SIZE + BONED_VERTEX_BYTE_SIZE]));
}


const MODEL_TYPE Model::Load(const char* filename)
{
	ML_String name = "Models\\Mdl\\";
	name.append(filename);

	std::ifstream reader;
	auto flags = std::ios::binary; // | std::ios::ate;
	reader.open(name.c_str(), flags);

	if (false == reader.is_open())
	{
		std::cerr << "Failed to open model for \"" << filename << "\"! Please verify file!" << std::endl;
		return MODEL_INSANE;
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
	m_data = MemLib::palloc(size);
	std::memcpy(&(*m_data), modelData, size);

	const MODEL_TYPE result = m_data->ValidByteData();
	if (MODEL_INSANE == result)
	{
		// pop the stack
		MemLib::spop();
		// Free the data
		MemLib::pfree(m_data);
		std::cerr << "Failed to load model \"" << filename << "\" correctly! Likely endian error!" << std::endl;
		return result;
	}

	// pop the stack
	MemLib::spop();

	if (MODEL_BONELESS == result)
		m_vertexBuffer = CreateVertexBuffer(m_data->GetBonelessVertices(), sizeof(VertexBoneless), m_data->m_numVertices, USAGE_IMMUTABLE);
	else
		m_vertexBuffer = CreateVertexBuffer(m_data->GetBonedVertices(), sizeof(VertexBoned), m_data->m_numVertices, USAGE_IMMUTABLE);
	m_indexBuffer = CreateIndexBuffer(m_data->GetIndices(), sizeof(uint32_t), m_data->m_numIndices);

	const modelGenericData& visCopy = *m_data;

	for (unsigned int i = 0; i < m_data->m_numMaterials; ++i)
	{
		// Same operation as GetMaterial(size_t), but not const
		Material& mat = ((Material*)(&(m_data->m_data[m_data->m_numSubMeshes * sizeof(SubMesh)])))[i];
		// Load colors
		mat.albedoIdx = LoadTexture(mat.albedo);
		// Load normal map
		mat.normalIdx = LoadTexture(mat.normal);
		// Load glow map
		mat.glowIdx = LoadTexture(mat.glow);
	}

	m_pixelShader = LoadPixelShader("TestPS.cso");
	if (MODEL_BONELESS == result)
		m_vertexShader = LoadVertexShader("TestVS.cso");
	else
		m_vertexShader = LoadVertexShader("TestSkelVS.cso");

	Animation animation;
	m_animations.push_back(animation);
	//m_animations[0].Load("Rocket_Anim_Wave.ani");
	m_animations[0].Load("PlayerPlaceholder_Anim_Stomp.ani");

	m_animationBuffer = CreateConstantBuffer(m_data->GetBoneMatrices(), m_data->m_numBones * sizeof(DirectX::XMMATRIX), 2);

	return result;
}

void Model::Free()
{
	MemLib::pfree(m_data);
}


bool Model::SetMaterialActive() const
{
	const Material& mat = (**m_data.m_pp).GetMaterial(0);
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
	if (false == SetConstantBuffer(m_animationBuffer, BIND_VERTEX))
		return false;
	return true;
}

void Model::SetPixelAndVertexShader() const
{
	SetPixelShader(m_pixelShader);
	SetVertexShader(m_vertexShader);
}


void Model::RenderAllSubmeshes()
{
	// Incorrect bone indices?
	// That would sort of explain some of the wonky stuff happening

	static float animationTime = 0.0f;
	animationTime += GetDeltaTime();
	if (animationTime > 1.0f)
		animationTime -= 2.0f;

	// Try to get the initial animation frame
	uint32_t size;
	//SetPixelAndVertexShader();
	UpdateConstantBuffer(m_animationBuffer, m_animations[0].GetFrame(animationTime, size));
	SetConstantBuffer(m_animationBuffer, BIND_VERTEX);

	for (unsigned int i = 0; i < m_data->m_numSubMeshes; ++i)
	{
		const SubMesh& currentMesh = m_data->GetSubMesh(i);
		const Material& currentMaterial = m_data->GetMaterial(currentMesh.m_material);

		// Set material and draw
		SetTexture(currentMaterial.albedoIdx, 0, BIND_PIXEL);
		d3d11Data->deviceContext->DrawIndexed(1 + currentMesh.m_end - currentMesh.m_start, currentMesh.m_start, 0);
	}
}
