#include "Model.h"
#include <fstream>
#include "MemLib\MemLib.hpp"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "STB_Helper.h"
#include "MemLib\ML_String.hpp"
#include "Hashing.h"
#include "D3D11Helper\D3D11Graphics.h"

#include "DeltaTime.h"

#include <filesystem>
#include <cstring>

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


Model::~Model()
{
	//Free();
}

const MODEL_TYPE Model::Load(const char* filename)
{
	std::strcpy(m_name, filename);
	ML_String name = "Models\\Mdl\\";
	name.append(filename);

	std::ifstream reader;
	auto flags = std::ios::binary; // | std::ios::ate;
	reader.open(name.c_str(), flags);

	assert(true == reader.is_open());

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
	assert(MODEL_INSANE != result);

	// pop the stack
	MemLib::spop();

	if (MODEL_BONELESS == result)
		m_vertexBuffer = CreateVertexBuffer(m_data->GetBonelessVertices(), sizeof(VertexBoneless), m_data->m_numVertices, USAGE_IMMUTABLE);
	else
	{
		// Load bone data
		m_vertexBuffer = CreateVertexBuffer(m_data->GetBonedVertices(), sizeof(VertexBoned), m_data->m_numVertices, USAGE_IMMUTABLE);

		// Construct filepath for the animations
		ML_String animationPath = "Models\\Ani\\";
		ML_String withoutMdl(filename);
		withoutMdl = withoutMdl.substr(0, withoutMdl.find_last_of("."));
		animationPath.append(withoutMdl);

		// Get the directory
		std::filesystem::directory_iterator animationDirectory(animationPath.c_str());

		ML_String can = "";
		for (const auto& entry : animationDirectory)
		{
			// Set the current animation name
			can = entry.path().string().c_str();
			can = can.substr(can.find_last_of("\\")+1, can.length());
			char animType = can[0];

			// Emplace if it doesn't exist
			if (false == m_animations.contains(animType))
			{
				m_animations.emplace(animType, ML_Vector<Animation>());
			}

			// Push the animation
			m_animations[animType].push_back(Animation());
			// Load the animation
			m_animations[animType][can[1] - '0'].Load(entry.path().string().c_str());
		}

		// Create the animation buffers
		m_animationVertexBuffer = CreateStructuredBuffer(m_data->GetBoneMatrices(), sizeof(DirectX::XMMATRIX), m_data->m_numBones, m_animationVertexBufferSRV);
		m_animationNormalBuffer = CreateStructuredBuffer(m_data->GetBoneMatrices(), sizeof(DirectX::XMMATRIX), m_data->m_numBones, m_animationNormalBufferSRV);
	}
		
	m_indexBuffer = CreateIndexBuffer(m_data->GetIndices(), sizeof(uint32_t), m_data->m_numIndices);

	const modelGenericData& visCopy = *m_data;

	for (unsigned int i = 0; i < m_data->m_numMaterials; ++i)
	{
		// Same operation as GetMaterial(size_t), but not const
		Material& mat = ((Material*)(&(m_data->m_data[m_data->m_numSubMeshes * sizeof(SubMesh)])))[i];
		// Load colors
		mat.albedoIdx = LoadTexture(mat.albedo);
		if (0 == std::strcmp(mat.albedo, mat.normal))
		{
			// Load normal map
			mat.normalIdx = LoadTexture("\\Default_Normal.png");
		}
		else
		{
			// Load normal map
			mat.normalIdx = LoadTexture(mat.normal);
		}
		// Load glow map
		mat.glowIdx = LoadTexture(mat.glow);
	}

	m_materialBuffer = CreateConstantBuffer(sizeof(MaterialBufferStruct));

	return result;
}

void Model::Free()
{
	if (m_animationVertexBuffer != -1)
	{
		DeleteD3D11Buffer(m_animationVertexBuffer);
		DeleteD3D11SRV(m_animationVertexBufferSRV);
		DeleteD3D11Buffer(m_animationNormalBuffer);
		DeleteD3D11SRV(m_animationNormalBufferSRV);
	}
	DeleteD3D11Buffer(m_vertexBuffer);
	DeleteD3D11Buffer(m_indexBuffer);
	DeleteD3D11Buffer(m_materialBuffer);
	m_animations.~ML_Map();
	MemLib::pfree(m_data);
}


bool Model::SetMaterialActive() const
{
	const Material& mat = (**m_data.m_pp).GetMaterial(0);
	if (SetTexture(mat.albedoIdx, BIND_PIXEL, 0)/* &&
		SetTexture(mat.normalIdx, 1, BIND_PIXEL) &&
		SetTexture(mat.glowIdx, 2, BIND_PIXEL)*/)
		return true;
	return false;
}

AnimationFrame Model::GetAnimation(const ANIMATION_TYPE aType, const uint8_t aIdx, const float aTime)
{
	if (m_animations.contains(aType) && aIdx < m_animations[aType].size())
	{
		AnimationFrame frame = m_animations[aType][aIdx].GetFrame(aTime);

		return m_animations[aType][aIdx].GetFrame(aTime);
	}

	// Handle this later on
	return m_animations[ANIMATION_IDLE][0].GetFrame(0);
}

void Model::RenderAllSubmeshes(const ANIMATION_TYPE aType, const uint8_t aIdx, const float aTime)
{

	// Try to get the initial animation frame
	DirectX::XMVECTOR tempScalar;
	DirectX::XMVECTOR tempRotation;
	DirectX::XMVECTOR tempTranslation;

	DirectX::XMMATRIX tempMatrix;

	m_data->m_numBones;

	if (m_animationVertexBuffer != -1)
	{
		AnimationFrame frame = GetAnimation(aType, aIdx, aTime);
		AnimationFrame frame2 = GetAnimation(ANIMATION_ATTACK, 0, aTime);
		// Push copy onto the stack
		AnimationFrame modified;
		modified.vertex = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));
		modified.normal = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));
		DirectX::XMMATRIX copy;

		for (int i = 0; i < m_data->m_numBones; i++)
		{
			if (i == 2) copy = frame.vertex[i];			//Center_Joint
			else if (i == 3) copy = frame.vertex[i];	//FootL_Joint
			else if (i == 4) copy = frame.vertex[i];	//FootR_Joint
			else if (i == 5) copy = frame.vertex[i];	//"freedom" joint
			else if (i == 9) copy = frame.vertex[i];	//Hip_Joint
			else if (i == 10) copy = frame.vertex[i];	//LegL_Joint
			else if (i == 11) copy = frame.vertex[i];	//LegR_Joint
			else if (i == 13) copy = frame.vertex[i];	//Root_Joint
			else if (i == 18) copy = frame.vertex[i];	//ThighL_Joint
			else if (i == 19) copy = frame.vertex[i];	//ThighR_Joint
			else if (i == 20) copy = frame.vertex[i];	//ToeL_Joint
			else if (i == 21) copy = frame.vertex[i];	//ToeR_Joint
			//Upper Body
			else if (i == 0) copy = frame2.vertex[i]; //ArmL_Joint
			else if (i == 1) copy = frame2.vertex[i]; //ArmR_Joint
			else if (i == 6) copy = frame2.vertex[i]; //HandL_Joint
			else if (i == 7) copy = frame2.vertex[i]; //HandR_Joint
			else if (i == 8) copy = frame2.vertex[i]; //Head_Joint
			else if (i == 12) copy = frame2.vertex[i];//Neck_Joint
			else if (i == 14) copy = frame2.vertex[i];//ShoulderL_Joint
			else if (i == 15) copy = frame2.vertex[i];//ShoulderR_Joint
			else if (i == 16) copy = frame2.vertex[i];//Spine1_Joint
			else if (i == 17) copy = frame2.vertex[i];//Spine2_Joint
			else if (i == 22) copy = frame2.vertex[i];//Top_Joint

			else
			{
				copy = frame.vertex[13];
			}
			

			DirectX::XMMatrixDecompose(&tempScalar, &tempRotation, &tempTranslation, DirectX::XMMatrixTranspose(copy));

			DirectX::XMMATRIX copyR = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorScale(tempRotation, 1.f));

			tempMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(
				DirectX::XMMatrixScalingFromVector(tempScalar), copyR
				/*DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorScale(tempRotation, 0.5f))*/),
				DirectX::XMMatrixTranslationFromVector(tempTranslation)));

			tempMatrix = DirectX::XMMatrixTranspose(tempMatrix);

			modified.vertex[i] = tempMatrix;
			modified.normal[i] = frame.normal[i];

		}
		



		UpdateStructuredBuffer(m_animationVertexBuffer, modified.vertex);
		SetShaderResourceView(m_animationVertexBufferSRV, BIND_VERTEX, 0);
		UpdateStructuredBuffer(m_animationNormalBuffer, modified.normal);
		SetShaderResourceView(m_animationNormalBufferSRV, BIND_VERTEX, 1);
		// Pop the copy from the stack
		MemLib::spop();
		MemLib::spop();
	}

	// Set as slot 0, for the time being
	SetConstantBuffer(m_materialBuffer, BIND_PIXEL, 0);

	for (unsigned int i = 0; i < m_data->m_numSubMeshes; ++i)
	{
		const SubMesh& currentMesh = m_data->GetSubMesh(i);
		const Material& currentMaterial = m_data->GetMaterial(currentMesh.m_material);

		// Create a buffer and give it values
		MaterialBufferStruct buffStruct(currentMaterial.roughness, currentMaterial.exponent);
		// Update the material buffer
		UpdateConstantBuffer(m_materialBuffer, &buffStruct);

		// Set material and draw
		SetTexture(currentMaterial.albedoIdx, BIND_PIXEL, 0);
		SetTexture(currentMaterial.normalIdx, BIND_PIXEL, 1);
		d3d11Data->deviceContext->DrawIndexed(1 + currentMesh.m_end - currentMesh.m_start, currentMesh.m_start, 0);
	}
}

void Model::RenderAllSubmeshesWithBlending(const ANIMATION_TYPE aType, const ANIMATION_TYPE aType2, const uint8_t aIdx, const float aTime)
{

	// Try to get the initial animation frame
	DirectX::XMVECTOR tempScalar;
	DirectX::XMVECTOR tempRotation;
	DirectX::XMVECTOR tempTranslation;

	DirectX::XMMATRIX tempMatrix;

	m_data->m_numBones;

	if (m_animationVertexBuffer != -1)
	{
		AnimationFrame frame = GetAnimation(aType, aIdx, aTime);
		AnimationFrame frame2 = GetAnimation(ANIMATION_ATTACK, 0, aTime);
		// Push copy onto the stack
		AnimationFrame modified;
		modified.vertex = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));
		modified.normal = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));
		DirectX::XMMATRIX copy;

		for (int i = 0; i < m_data->m_numBones; i++)
		{
			if (i == 2) copy = frame.vertex[i];			//Center_Joint
			else if (i == 3) copy = frame.vertex[i];	//FootL_Joint
			else if (i == 4) copy = frame.vertex[i];	//FootR_Joint
			else if (i == 5) copy = frame.vertex[i];	//"freedom" joint
			else if (i == 9) copy = frame.vertex[i];	//Hip_Joint
			else if (i == 10) copy = frame.vertex[i];	//LegL_Joint
			else if (i == 11) copy = frame.vertex[i];	//LegR_Joint
			else if (i == 13) copy = frame.vertex[i];	//Root_Joint
			else if (i == 18) copy = frame.vertex[i];	//ThighL_Joint
			else if (i == 19) copy = frame.vertex[i];	//ThighR_Joint
			else if (i == 20) copy = frame.vertex[i];	//ToeL_Joint
			else if (i == 21) copy = frame.vertex[i];	//ToeR_Joint
			//Upper Body
			else if (i == 0) copy = frame2.vertex[i]; //ArmL_Joint
			else if (i == 1) copy = frame2.vertex[i]; //ArmR_Joint
			else if (i == 6) copy = frame2.vertex[i]; //HandL_Joint
			else if (i == 7) copy = frame2.vertex[i]; //HandR_Joint
			else if (i == 8) copy = frame2.vertex[i]; //Head_Joint
			else if (i == 12) copy = frame2.vertex[i];//Neck_Joint
			else if (i == 14) copy = frame2.vertex[i];//ShoulderL_Joint
			else if (i == 15) copy = frame2.vertex[i];//ShoulderR_Joint
			else if (i == 16) copy = frame2.vertex[i];//Spine1_Joint
			else if (i == 17) copy = frame2.vertex[i];//Spine2_Joint
			else if (i == 22) copy = frame2.vertex[i];//Top_Joint

			else
			{
				copy = frame.vertex[13];
			}


			DirectX::XMMatrixDecompose(&tempScalar, &tempRotation, &tempTranslation, DirectX::XMMatrixTranspose(copy));

			DirectX::XMMATRIX copyR = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorScale(tempRotation, 1.f));

			tempMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(
				DirectX::XMMatrixScalingFromVector(tempScalar), copyR
			/*DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorScale(tempRotation, 0.5f))*/),
				DirectX::XMMatrixTranslationFromVector(tempTranslation)));

			tempMatrix = DirectX::XMMatrixTranspose(tempMatrix);

			modified.vertex[i] = tempMatrix;
			modified.normal[i] = frame.normal[i];

		}




		UpdateStructuredBuffer(m_animationVertexBuffer, modified.vertex);
		SetShaderResourceView(m_animationVertexBufferSRV, BIND_VERTEX, 0);
		UpdateStructuredBuffer(m_animationNormalBuffer, modified.normal);
		SetShaderResourceView(m_animationNormalBufferSRV, BIND_VERTEX, 1);
		// Pop the copy from the stack
		MemLib::spop();
		MemLib::spop();
	}

	// Set as slot 0, for the time being
	SetConstantBuffer(m_materialBuffer, BIND_PIXEL, 0);

	for (unsigned int i = 0; i < m_data->m_numSubMeshes; ++i)
	{
		const SubMesh& currentMesh = m_data->GetSubMesh(i);
		const Material& currentMaterial = m_data->GetMaterial(currentMesh.m_material);

		// Create a buffer and give it values
		MaterialBufferStruct buffStruct(currentMaterial.roughness, currentMaterial.exponent);
		// Update the material buffer
		UpdateConstantBuffer(m_materialBuffer, &buffStruct);

		// Set material and draw
		SetTexture(currentMaterial.albedoIdx, BIND_PIXEL, 0);
		SetTexture(currentMaterial.normalIdx, BIND_PIXEL, 1);
		d3d11Data->deviceContext->DrawIndexed(1 + currentMesh.m_end - currentMesh.m_start, currentMesh.m_start, 0);
	}
}

// The semiglobal map of loaded models
ML_Map<uint64_t, Model>* loadedModels = nullptr;

const uint64_t LoadModel(const char* filename)
{
	
	if (nullptr == loadedModels)
	{
		loadedModels = (ML_Map<uint64_t, Model>*)MemLib::spush(sizeof(ML_Map<uint64_t, Model>));
		new(loadedModels) ML_Map<uint64_t, Model>();
	}

	uint64_t hash = C_StringToHash(filename);

	if (loadedModels->contains(hash))
	{
		// Increment refcount and return hash
		++(LOADED_MODELS[hash].m_refCount);
		return hash;
	}

	loadedModels->emplace(hash, Model()); // Emplace if the first
	LOADED_MODELS[hash].Load(filename); // Load if the first
	LOADED_MODELS[hash].m_refCount = 1; // Set refcount to 1 if this is the first

	return hash;
}

const bool ReleaseModel(const uint64_t& hash)
{
	if (loadedModels->contains(hash))
	{
		// Reduce the refcount
		--(LOADED_MODELS[hash].m_refCount);
		// If the refcount is zero, erase the model
		if (0 == LOADED_MODELS[hash].m_refCount)
		{
			Model& modelRef = LOADED_MODELS[hash];
			modelRef.Free();
			LOADED_MODELS.erase(hash);
			return true;
		}
	}
	return false;
}
