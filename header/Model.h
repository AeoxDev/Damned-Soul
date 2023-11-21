#pragma once
#include "D3D11Helper\IDX_Types.h"
#include "MemLib\PoolPointer.hpp"
#include "MemLib\ML_Vector.hpp"
#include "MemLib\ML_Map.hpp"
#include "Animation.hpp"


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
private:
	// Used internally only
	struct MaterialBufferStruct
	{
		float roughness = 0.5f;
		float exponent = 1.0f;

		MaterialBufferStruct(const float r, const float e)
		{
			this->roughness = r;
			this->exponent = e;
		}
	};

public:
	//PoolPointer<ModelBoneless> m_modelData;
	char m_name[128];
	PoolPointer<modelGenericData> m_data;
	ML_Map<char, ML_Vector<Animation>> m_animations;


	VB_IDX m_vertexBuffer = -1;
	IB_IDX m_indexBuffer = -1;
	CB_IDX m_materialBuffer = -1;
	SB_IDX m_animationVertexBuffer = -1;
	SRV_IDX m_animationVertexBufferSRV = -1;
	SB_IDX m_animationNormalBuffer = -1;
	SRV_IDX m_animationNormalBufferSRV = -1;
	uint16_t m_refCount = 0;
	
	
	~Model();

	// Load a .mdl file
	// No other file formats are supported!
	const MODEL_TYPE Load(const char* filename);

	bool SetMaterialActive() const;

	AnimationFrame GetAnimation(const ANIMATION_TYPE aType, const uint8_t aIdx, const float aTime);

	// Render all the model's submeshes one after another
	void RenderAllSubmeshes(const ANIMATION_TYPE aType = ANIMATION_IDLE, const uint8_t aIdx = 0, const float aTime = -1.f);

	// Render all tge model's submeshes one after another WITH blending
	void RenderAllSubmeshesWithBlending(const ANIMATION_TYPE aType = ANIMATION_IDLE, const ANIMATION_TYPE aType2 = ANIMATION_IDLE, const uint8_t aIdx = 0, const float aTime = -1.f);

	void Free();
};

// Load a model by filename, keeping a reference counter
// If the model was already loaded, increase the reference counter instead
// Returns a hash to the model
const uint64_t LoadModel(const char* filename);

// Release a model by hash, reducing the reference counter
// If the reference counter is reduced to 0, the model is completely removed from the system
const bool ReleaseModel(const uint64_t& hash);

// A macro that fetches the data in the loaded models pointer
#define LOADED_MODELS (*loadedModels)

// A pointer to a map of the loaded models
extern ML_Map<uint64_t, Model>* loadedModels;

