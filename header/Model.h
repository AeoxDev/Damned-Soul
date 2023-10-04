#pragma once
#include <cinttypes>
#include "MemLib\PoolPointer.hpp"
#include "MemLib\ML_Vector.hpp"
#include "Animation.hpp"
#include <DirectXMath.h>

enum MODEL_TYPE
{
	MODEL_INSANE = 0,
	MODEL_BONELESS = 1, // Model loaded without bones
	MODEL_WITH_BONES = 2 // Model loaded with bones
};


struct VertexBoneless
{
	float m_position[4];
	float m_normal[4];
	float m_uv[2];
};

struct VertexBoned
{
	float m_position[4];
	float m_normal[4];
	float m_uv[2];
	uint32_t m_boneIdx[4];
	float m_boneWeight[4];
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

struct modelGenericData
{
	const uint32_t m_sanityCheckNumber;
	const uint32_t m_numSubMeshes;
	const uint32_t m_numMaterials;
	const uint32_t m_numIndices;
	const uint32_t m_numVertices;
	const uint32_t m_numBones;

	#pragma warning(suppress : 4200)
	const char m_data[];//Array is intentional, ignore warning

	const MODEL_TYPE ValidByteData() const;
	const SubMesh& GetSubMesh(const size_t idx) const;
	const Material& GetMaterial(const size_t idx) const;
	const uint32_t* GetIndices() const; // Swap the indices and vertices in the data structure!!!!!!!!
	const VertexBoneless* GetBonelessVertices() const;
	const VertexBoned* GetBonedVertices() const;
	DirectX::XMMATRIX* GetBoneMatrices();
};

struct Model
{
	//PoolPointer<ModelBoneless> m_modelData;
	PoolPointer<modelGenericData> m_data;
	ML_Vector<Animation> m_animations;


	uint32_t m_vertexBuffer = -1, m_indexBuffer = -1;
	uint16_t m_animationBuffer = -1;
	uint8_t m_pixelShader = -1, m_vertexShader = -1;

	// Load a .mdl file
	// No other file formats are supported!
	const MODEL_TYPE Load(const char* filename);

	bool SetMaterialActive() const;

	// Set the currently mode index and vertex buffers to this model
	bool SetVertexAndIndexBuffersActive() const;

	void SetPixelAndVertexShader() const;

	// Render all the model's submeshes one after another
	void RenderAllSubmeshes();

	void Free();
};