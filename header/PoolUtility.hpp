#pragma once
#include "MemLib/MemLib.hpp"
#include <cstring>

template<typename T1, typename T2>
struct PoolMap
{
private:
	size_t m_size;
	size_t m_capacity;
	PoolPointer<T1> m_keys;
	PoolPointer<T2> m_values;

public:
	void Reserve(const size_t& capacity)
	{
		auto tempKey = MemLib::palloc(sizeof(T1) * capacity);
		auto tempVal = MemLib::palloc(sizeof(T2) * capacity);

		if (false == m_keys.IsNullptr())
		{
			std::memcpy(&*tempKey, &*m_keys, sizeof(T1) * m_size);
			std::memcpy(&*tempVal, &*m_values, sizeof(T2) * m_size);
		}

		//for (unsigned int i = 0; i < m_size; ++i)
		//{
		//	tempKey[i] = m_keys[i];
		//	tempVal[i] = m_values[i];
		//}

		MemLib::pfree(m_keys);
		MemLib::pfree(m_values);
		m_keys = tempKey;
		m_values = tempVal;

		m_capacity = capacity;

	}

	//PoolMap()
	//{
	//	m_size = 0;
	//	Reserve(4);
	//}

	PoolMap(const size_t& startCapacity = 4)
	{
		m_size = 0;
		Reserve(startCapacity);
	}

	void emplace(T1& key, T2& value)
	{
		if (m_size == m_capacity)
			Reserve((0 == m_size) ? 2 : (m_size * 2));

		for (unsigned int i = 0; i < m_size; ++i)
		{
			if (key == m_keys[i])
				return;//m_keys[999999] = m_keys[999998]; // Really scuffed way of forcing an "error" message for duplicate keys
		}

		std::memcpy((void*)&m_keys[m_size], (void*)&key, sizeof(key));
		std::memcpy((void*)&m_values[m_size], (void*)&value, sizeof(value));


		++m_size;
	}

	T2& operator[](const T1& key)
	{
		for (unsigned int i = 0; i < m_size; ++i)
		{
			if (key == m_keys[i])
				return m_values[i];
		}
		return m_values[999999999]; // This is an error message stand in
	}

	const size_t Size() const
	{
		return m_size;
	}

	const T1* GetKeys() const
	{
		return *(m_keys.m_pp);
	}

	void Clear()
	{
		m_size = 0;
		m_capacity = 0;
	}
};


template<typename T>
struct PoolVector
{
private:
	size_t m_size;
	size_t m_capacity;
	PoolPointer<T> m_values;

public:
	PoolVector(const size_t& startCapacity = 4)
	{
		m_size = 0;
		Reserve(startCapacity);
	}

	void Reserve(const size_t& capacity)
	{
		auto tempVal = MemLib::palloc(sizeof(T) * capacity);

		MemLib::pfree(m_values);
		m_values = tempVal;

		m_capacity = capacity;
	}

	void emplace_back(T& value)
	{
		if (m_size == m_capacity)
			Reserve((0 == m_size) ? 2 : (m_size * 2));

		std::memcpy((void*)&m_values[m_size], (void*)&value, sizeof(value));

		++m_size;
	}

	T& operator[](const unsigned int key)
	{
		return m_values[key];
	}

	const size_t Size() const
	{
		return m_size;
	}

	void Clear()
	{
		m_size = 0;
		m_capacity = 0;
	}
};