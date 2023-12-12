#include "Model.h"
#include <fstream>
#include "MemLib\MemLib.hpp"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "STB_Helper.h"
#include "MemLib\ML_String.hpp"
#include "Hashing.h"
#include "D3D11Helper\D3D11Graphics.h"
#include "Skynet\TempBossBehaviour.h"

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


void Model::scaleQuarternion(DirectX::XMVECTOR& vector, float scale)
{
	DirectX::XMFLOAT4 rotQuat;

	DirectX::XMStoreFloat4(&rotQuat, vector);

	rotQuat.x *= scale;
	rotQuat.y *= scale;
	rotQuat.z *= scale;
	rotQuat.w *= scale;

	vector = DirectX::XMLoadFloat4(&rotQuat);
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

void Model::RenderAllSubmeshes(EntityID& entity, const ANIMATION_TYPE aType, const uint8_t aIdx, const float aTime, bool isOutline)
{

	// Try to get the initial animation frame
	m_data->m_numBones;

	if (m_animationVertexBuffer != -1)
	{
		AnimationFrame frame = GetAnimation(aType, aIdx, aTime);
		// Push copy onto the stack
		AnimationFrame modified;
		modified.vertex = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));
		modified.normal = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));

		for (int i = 0; i < m_data->m_numBones; i++)
		{
			modified.vertex[i] = frame.vertex[i];
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

	if (m_data->m_numSubMeshes == 7 && m_data->m_numBones == 25) // this is splitboss, we need to do special things. Trust the process, don't touch this code...bastard
	{
		TempBossBehaviour* tempBossComponent = registry.GetComponent<TempBossBehaviour>(entity);
		for (unsigned int i = 0; i < m_data->m_numSubMeshes; ++i)
		{
			if (i != 0 && i != 1)
			{
				if (tempBossComponent->parts[i - 2] == false && false == isOutline)
				{
					continue;
				}
			}
			const SubMesh& currentMesh = m_data->GetSubMesh(i);
			const Material& currentMaterial = m_data->GetMaterial(currentMesh.m_material);

			// Create a buffer and give it values
			MaterialBufferStruct buffStruct(currentMaterial.roughness, currentMaterial.exponent);
			// Update the material buffer
			UpdateConstantBuffer(m_materialBuffer, &buffStruct);

			// Set material and draw
			SetTexture(currentMaterial.albedoIdx, BIND_PIXEL, 0);
			SetTexture(currentMaterial.normalIdx, BIND_PIXEL, 1);
			SetTexture(currentMaterial.glowIdx, BIND_PIXEL, 2);
			d3d11Data->deviceContext->DrawIndexed(1 + currentMesh.m_end - currentMesh.m_start, currentMesh.m_start, 0);
			int iJustWantToSeeWhatHappens = 0; // debug variable?
		}
	}
	else
	{
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
			SetTexture(currentMaterial.glowIdx, BIND_PIXEL, 2);
			d3d11Data->deviceContext->DrawIndexed(1 + currentMesh.m_end - currentMesh.m_start, currentMesh.m_start, 0);

			int iJustWantToSeeWhatHappens = 0; // debug variable?
		}
	}
}

void Model::RenderAllSubmeshesWithBlending(const ANIMATION_TYPE aType, const uint8_t aIdx, const float aTime , const ANIMATION_TYPE aType2, const uint8_t aIdx2, const float aTime2)
{

	// Try to get the initial animation frame
	DirectX::XMVECTOR tempScalar;
	DirectX::XMVECTOR tempRotation;
	DirectX::XMVECTOR tempTranslation;

	DirectX::XMMATRIX tempVertMatrix;
	DirectX::XMMATRIX tempNormMatrix;

	m_data->m_numBones;

	if (m_animationVertexBuffer != -1)
	{
		AnimationFrame frame = GetAnimation(aType, aIdx, aTime);
		AnimationFrame frame2 = GetAnimation(aType2, aIdx2, aTime2);
		// Push copy onto the stack
		AnimationFrame modified;
		modified.vertex = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));
		modified.normal = (DirectX::XMMATRIX*)MemLib::spush(m_data->m_numBones * sizeof(DirectX::XMMATRIX));
		DirectX::XMMATRIX copyVert;
		DirectX::XMMATRIX copyNorm;
		float animScale = 1.0f;

		for (int i = 0; i < m_data->m_numBones; i++)
		{
			// Lower Body
			
			//if (i == 0) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i];}	//0  Control_Joint
			if (i == 15) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }		//15 FootL_Joint
			else if (i == 16) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//16 FootR_Joint
			//else if (i == 20) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//20 Hip_Joint
			else if (i == 21) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//21 KneeL_Joint
			else if (i == 22) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//22 KneeR_Joint
			//else if (i == 24){ copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//24 Root_Joint
			else if (i == 29) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//29 ThighL_Joint
			else if (i == 30) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//30 ThighR_Joint
			else if (i == 37) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//37 TiptoeL_Joint
			else if (i == 38) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//38 TiptoeR_Joint
			else if (i == 39) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//39 ToeL_Joint
			else if (i == 40) { copyVert = frame.vertex[i]; copyNorm = frame.normal[i]; }	//40 ToeR_Joint
			
			else if (i == 20)
			{
				copyVert = (frame.vertex[i] * 0.2f) + (frame2.vertex[i] * 0.8f);
				copyNorm = (frame.vertex[i] * 0.2f) + (frame2.vertex[i] * 0.8f);
			}
			else if (i == 27)
			{
				copyVert = (frame.vertex[i] * 0.5f) + (frame2.vertex[i] * 0.5f);
				copyNorm = (frame.vertex[i] * 0.5f) + (frame2.vertex[i] * 0.5f);
			}
			else if (i == 28)
			{
				copyVert = (frame.vertex[i] * 0.2f) + (frame2.vertex[i] * 0.8f);
				copyNorm = (frame.vertex[i] * 0.2f) + (frame2.vertex[i] * 0.8f);
			}
			//Upper Body
			else
			{
				copyVert = frame2.vertex[i];
				copyNorm = frame2.normal[i];
			}
			
			 

			// Decompose VERTEX matrix to scale rotations and translations without scaling the entire transform matrix.
			DirectX::XMMatrixDecompose(&tempScalar, &tempRotation, &tempTranslation, DirectX::XMMatrixTranspose(copyVert));
			
			scaleQuarternion(tempRotation, animScale);

			//Rebuild decomposed transformation matrix
			tempVertMatrix = DirectX::XMMatrixIdentity(); //IdentityMatrix (T-Pose)
			tempVertMatrix *= DirectX::XMMatrixRotationQuaternion(tempRotation); //Self * Rotation
			tempVertMatrix *= DirectX::XMMatrixScalingFromVector(tempScalar); //Self * Scale
			tempVertMatrix *= DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(tempTranslation, animScale)); //Self * Translation

			tempVertMatrix = DirectX::XMMatrixTranspose(tempVertMatrix);


			// Decompose NORMAL matrix to scale rotations and translations without scaling the entire transform matrix.
			DirectX::XMMatrixDecompose(&tempScalar, &tempRotation, &tempTranslation, DirectX::XMMatrixTranspose(copyNorm));

			scaleQuarternion(tempRotation, animScale);

			//Rebuild decomposed transformation matrix
			
			tempNormMatrix = DirectX::XMMatrixIdentity(); //IdentityMatrix (T-Pose)

			tempNormMatrix *= DirectX::XMMatrixRotationQuaternion(tempRotation); //Self * Rotation
			tempNormMatrix *= DirectX::XMMatrixScalingFromVector(tempScalar); //Self * Scale
			tempNormMatrix *= DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorScale(tempTranslation, animScale)); //Self * Translation

			tempNormMatrix = DirectX::XMMatrixTranspose(tempNormMatrix);

			modified.vertex[i] = tempVertMatrix;
			modified.normal[i] = tempNormMatrix;

		}


		UpdateStructuredBuffer(m_animationVertexBuffer, modified.vertex);
		SetShaderResourceView(m_animationVertexBufferSRV, BIND_VERTEX, 0);
		UpdateStructuredBuffer(m_animationNormalBuffer, modified.normal);
		SetShaderResourceView(m_animationNormalBufferSRV, BIND_VERTEX, 1);

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
			SetTexture(currentMaterial.glowIdx, BIND_PIXEL, 2);
			d3d11Data->deviceContext->DrawIndexed(1 + currentMesh.m_end - currentMesh.m_start, currentMesh.m_start, 0);

			int iJustWantToSeeWhatHappens = 0; // debug variable?
		}
		// Pop the copy from the stack
		MemLib::spop();
		MemLib::spop();
	}
}

void Model::RenderMeshes()
{
	for (unsigned int i = 0; i < m_data->m_numSubMeshes; ++i)
	{
		const SubMesh& currentMesh = m_data->GetSubMesh(i);

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
