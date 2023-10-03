#pragma once

#include "D3D11Graphics.h"
#include <DirectXMath.h>
#include <bitset>

#include "Relics.h"

//Components:
/*
Graphics (d3d11 stuff, transform too)
Status (hp, movespeed, attack speed, damage)
Player (souls collected)
Enemy (possibly some bool checking to see if they're in an attack animation)
Weapon (move attack speed and damage variables here? depends how we want to do things)
StaticHazard (damage)
*/

//TEEEEMP
//struct ConstantBuffer
//{
//	ID3D11Buffer* m_buffer = nullptr;
//	size_t m_bufferSize = 0;
//
//	ConstantBuffer() = default;
//
//	//You're welcome herman ehe
//	void Init(ID3D11Device* device, void* data, size_t byteSize)
//	{
//		m_bufferSize = byteSize;
//
//		D3D11_BUFFER_DESC desc = {};
//		desc.ByteWidth = byteSize;
//		desc.Usage = D3D11_USAGE_DYNAMIC;
//		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//		desc.MiscFlags = 0;
//		desc.StructureByteStride = 0;
//
//		D3D11_SUBRESOURCE_DATA srd = {};
//		srd.pSysMem = data;
//		srd.SysMemPitch = 0;
//		srd.SysMemSlicePitch = 0;
//
//		//Do something like HRESULT or assert here?
//		device->CreateBuffer(&desc, &srd, &m_buffer);
//	}
//	void Update(ID3D11DeviceContext* context, void* data)
//	{
//		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
//		HRESULT hr = context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//
//		if (FAILED(hr))
//		{
//			MessageBox(NULL, L"Failed to map resource!", L"Error", MB_OK);
//			return;
//		}
//		memcpy(mappedResource.pData, data, m_bufferSize);
//
//		context->Unmap(m_buffer, 0);
//	}
//};
//
////Temporary graphics component gibberish that needs to work with however the rest of graphics stuff is set up so this is all super temp
//struct GraphicsComponent
//{
//	//Defaults
//	DirectX::XMFLOAT3 m_scale = {1.0f, 1.0f, 1.0f};
//	DirectX::XMFLOAT3 m_rotate = {0.0f, 0.0f, 0.0f};
//	DirectX::XMFLOAT3 m_translate = {0.0f, 0.0f, 0.0f};
//
//	//Container/descriptor for the transform
//	struct WorldTransform
//	{
//		DirectX::XMFLOAT4X4 scale;
//		DirectX::XMFLOAT4X4 rotate;
//		DirectX::XMFLOAT4X4 translate;
//	};
//	WorldTransform m_transform;
//
//	//Ababa
//	ConstantBuffer m_constantBuffer;
//
//	//ConstructoRR (ehe do Initialize()-function instead maybe)
//	GraphicsComponent(ID3D11Device* device, float posX, float posY, float posZ)
//	{
//		m_translate = { posX, posY, posZ };
//		
//		//Store all the stuff into the m_transform
//		DirectX::XMStoreFloat4x4(&m_transform.scale, DirectX::XMMatrixTranspose(
//			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)
//		));
//		DirectX::XMStoreFloat4x4(&m_transform.rotate, DirectX::XMMatrixTranspose(
//			DirectX::XMMatrixRotationX(m_rotate.x) * DirectX::XMMatrixRotationY(m_rotate.y) * DirectX::XMMatrixRotationZ(m_rotate.z)
//		));
//		DirectX::XMStoreFloat4x4(&m_transform.translate, DirectX::XMMatrixTranspose(
//			DirectX::XMMatrixTranslation(m_translate.x, m_translate.y, m_translate.z)
//		));
//
//		//Init constant buffer using the transform
//		m_constantBuffer.Init(device, &m_transform, sizeof(m_transform));
//
//		//Submesh stuff? Idk I have no idea how the rest of graphics stuff is handled here
//	}
//
//	//Guhhh..?
//	void EditTranslation(float x, float y, float z)
//	{
//		m_translate.x += x;
//		m_translate.y += y;
//		m_translate.z += z;
//
//		DirectX::XMStoreFloat4x4(&m_transform.translate, DirectX::XMMatrixTranspose(
//			DirectX::XMMatrixTranslation(m_translate.x, m_translate.y, m_translate.z)
//		));
//	}
//};

//Temporary component for player
#define MAX_RELICS 8
struct TempPlayerComponent
{
#define MAX_LENGTH 16

	char name[MAX_LENGTH] = "Default Name";
	int hp = 100;

	std::bitset<MAX_RELICS> relicBitset; //I have become bitset enjoyer

	float damage = 20.0f;
	float defense = 0.0f; //percentage
	float speed = 0.25f;

	TempPlayerComponent(const char name_in[MAX_LENGTH], const int hp) :hp(hp)
	{
		std::memcpy(name, name_in, MAX_LENGTH);
	}

	struct RelicArray
	{
		RelicArray(size_t relicSize)
		{
			this->relicSize = relicSize;
			this->data = new char[relicSize];
		}

		inline void* get()
		{
			return data;
		}

		~RelicArray()
		{
			delete[] data;
		}

		char* data = nullptr;

	private:
		size_t relicSize = 0;
	};

	//Any time GetId() is called on a new type of relic, relicCount gets incremented, which results in the ID number for every component type being unique
	int relicCount = 0;
	template <typename T>
	int GetId()
	{
		static int id = relicCount++;
		return id;
	}

	template<typename T>
	void AddRelic()
	{
		//Ehe
		int id = GetId<T>();

		//Create relic and cast to RelicArray data
		T* relicPointer = new (relics->get()) T();

		//Set the component bitset of the entity at "id" to match the component we're passing in
		relicBitset.set(id);
	}

	template<typename T>
	T* GetRelic()
	{
		int id = GetId<T>(); //Since GetId is being called on a type T that it's been called on before, the id won't be incremented. It's brilliant honestly

		if (!relicBitset.test(id)) //Test to see if we have the relic before we try returning it
			return nullptr;

		//Get relic by casting RelicArray data back to the relic struct (but pointer)
		T* relicPointer = (T*)(relics->get());

		return relicPointer;
	}

	template<typename T>
	void RemoveRelic()
	{
		int id = GetId<T>();
		relicBitset.reset(id);
	}

private:
	RelicArray* relics = new RelicArray(sizeof(DamageRelic)); //Since all relics are the same size, this is fine
};










//DANGER ZONE
/*
struct StatusComponent
{
	int hp = 50;

	StatusComponent() = default;
	StatusComponent(int n) : hp(n) {}
};

struct EnemyComponent
{
#define MAX_LENGTH 16
	char name[MAX_LENGTH] = "Default Name";

	EnemyComponent() = default;
	EnemyComponent(const char s[MAX_LENGTH])
	{
		std::memcpy(name, s, MAX_LENGTH);
	}
};


struct GraphicsComponent
{
	//posX, posY, texture..
};

struct PositionComponent
{
	bool is3D;//false: position relative in screen, 0.0 to 1.0nb  
	float posX, posY, posZ;
	float lastPosX;
	float lastPosZ;
	float mass;
};

/// <summary>
/// This component is only for moving objects (e.g. walls do not need a physics component)
/// </summary>
*/