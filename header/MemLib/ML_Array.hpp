#pragma once
#include "MemLib\MemLib.hpp"
#include <cinttypes>
#include <stdexcept>
//#include <xutility>

template<typename _T, unsigned int _Count>
class ML_Array
{
private:
	PoolPointer<_T> m_data;
	uint32_t m_size;
	uint16_t m_tSize;

public:
	_T* begin() const
	{
		return &(m_data[0]);
	};

	_T* end() const
	{
		return &(m_data[m_size]);
	};

	const uint32_t& size() const
	{
		return m_size;
	};

	const PoolPointer<_T>& data() const
	{
		return m_data;
	};

	// Construct the array
	ML_Array()
	{
		// Set the size
		m_size = _Count;
		// Set the element size
		m_tSize = sizeof(_T);
		// Allocate memory
		m_data = MemLib::palloc(m_size * m_tSize);
	};

	// Initialize the array, performs the exact same work as the constructor
	const ML_Array& Initialize()
	{
		// Set the size
		m_size = _Count;
		// Set the element size
		m_tSize = sizeof(_T);
		// Allocate memory
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_size * m_tSize);
	};

	ML_Array(const ML_Array& to_copy)
	{
		m_size = to_copy.m_size;
		// Set the individual item size
		m_tSize = to_copy.m_tSize;

		// Free the old memory
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_size * m_tSize);
		// Set items

		for (uint32_t i = 0; i < m_size; ++i)
			new(&m_data[i]) _T(to_copy.m_data[i]);
	}

	template<typename... Types>
	ML_Array(Types... args)
	{
		if (sizeof...(args) != _Count)
		{
			throw std::invalid_argument("Incorrect number of initial values for ML_Array!");
			std::terminate();
		}
		// Set capacity
		m_size = _Count;
		// Set the individual item size
		m_tSize = sizeof(_T);

		// Allocate to memory pool
		m_data = MemLib::palloc(m_size * m_tSize);

		// Set items
		uint32_t idx = 0;
		for (auto item : { args... })
		{
			if (typeid(item) != typeid(_T))
			{
				throw std::invalid_argument("Incorrect type for provided initial value for ML_Array!");
				std::terminate();
			}
			//std::memcpy(&(m_data[idx++]), &item, m_tSize);
			new(&m_data[idx++]) _T(item);
		}
	};

	void clear()
	{
		for (uint32_t i = 0; i < m_size; ++i)
			m_data[i].~_T();
	}

	ML_Array& operator=(const ML_Array& other)
	{
		// First clear
		clear();

		// Set size and element size
		m_size = other.m_size;
		m_tSize = other.m_tSize;

		// Free and allocate new
		MemLib::pfree(m_data);
		m_data = MemLib::palloc(m_size * m_tSize);

		// Copy
		for (uint32_t i = 0; i < m_size; ++i)
			new(&m_data[i]) _T(other.m_data[i]);
		//std::memcpy(m_data, other.m_data, m_size * m_tSize);

		return (*this);
	};

	~ML_Array()
	{
		clear();
		MemLib::pfree(m_data);
	};

	_T& operator[](const uint32_t& idx)
	{
		if (m_size <= idx)
		{
			throw std::out_of_range("Index provided for ML_Array is out of range!");
			std::terminate();
		}

		return m_data[idx];
	};

	operator _T* ()
	{
		return m_data;
	};

	_T& operator*()
	{
		return *m_data;
	};
};