#pragma once
#include "MemLib\MemLib.hpp"
#include <cinttypes>
#include <stdexcept>
//#include <xutility>

template<typename T, unsigned int count>
class ML_Array
{
private:
	PoolPointer<T> m_data;
	uint32_t m_size;
	uint16_t m_tSize;

public:
	T* begin() const
	{
		return &(m_data[0]);
	};

	T* end() const
	{
		return &(m_data[m_size]);
	};

	const uint32_t& size() const
	{
		return m_size;
	};

	const PoolPointer<T>& data() const
	{
		return m_data;
	};

	// Construct the array
	ML_Array()
	{
		// Set the size
		m_size = count;
		// Set the element size
		m_tSize = sizeof(T);
		// Allocate memory
		m_data = MemLib::palloc(m_size * m_tSize);
	};

	// Initialize the array, performs the exact same work as the constructor
	const ML_Array& Initialize()
	{
		// Set the size
		m_size = count;
		// Set the element size
		m_tSize = sizeof(T);
		// Allocate memory
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_size * m_tSize);
	};

	template<typename... Types>
	ML_Array(Types... args)
	{
		if (sizeof...(args) != count)
		{
			throw std::invalid_argument("Incorrect number of initial values for ML_Array!");
			std::terminate();
		}
		// Set capacity
		m_size = count;
		// Set the individual item size
		m_tSize = sizeof(T);

		// Allocate to memory pool
		m_data = MemLib::palloc(m_size * m_tSize);

		// Set items
		uint32_t idx = 0;
		for (auto item : { args... })
		{
			//if (typeid(item) != typeid(T))
			//{
			//	throw std::invalid_argument("Incorrect type for provided initial value for ML_Array!");
			//	std::terminate();
			//}
			std::memcpy(&(m_data[idx++]), &item, m_tSize);
		}
	};

	ML_Array& operator=(const ML_Array& other)
	{
		// Set size and element size
		m_size = other.m_size;
		m_tSize = other.m_tSize;

		// Free and allocate new
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_size * m_tSize);

		// Copy
		std::memcpy(m_data, other.m_data, m_size * m_tSize);

		return (*this);
	};

	~ML_Array()
	{
		MemLib::pfree(m_data);
	};

	T& operator[](const uint32_t& idx)
	{
		if (m_size <= idx)
		{
			throw std::out_of_range("Index provided for ML_Array is out of range!");
			std::terminate();
		}

		return m_data[idx];
	};

	operator T* ()
	{
		return m_data;
	};

	T& operator*()
	{
		return *m_data;
	};
};