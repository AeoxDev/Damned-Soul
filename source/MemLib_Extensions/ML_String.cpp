#pragma once
#include "MemLib\ML_String.hpp"
#include "MemLib\MemLib.hpp"
#include <charconv>
#include <stdexcept>
#include <stdio.h>

char* ML_String::begin() const
{
	return &(m_data[0]);
}

char* ML_String::end() const
{
	return &(m_data[m_len]);
}

uint32_t ML_String::find_first_of(const char* other)
{
	uint8_t oLen = (uint8_t)std::strlen(other);

	// Find the first that maches a single char of the other string
	for (uint32_t i = 0; i < m_len; ++i)
	{
		for (uint8_t u = 0; u < oLen; ++u)
		{
			if (m_data[i] == other[u])
				return i;
		}
	}
	// Return max otherwise
	return UINT32_MAX;
}

uint32_t ML_String::find_last_of(const char* other)
{
	uint8_t oLen = (uint8_t)std::strlen(other);

	// Find the last that maches a single char of the other string
	for (uint32_t i = m_len; 0 <= i; --i)
	{
		for (uint8_t u = 0; u < oLen; ++u)
		{
			if (m_data[i] == other[u])
				return i;
		}
	}
	// Return max otherwise
	return UINT32_MAX;
}

ML_String ML_String::substr(uint32_t beg, uint32_t len)
{
	ML_String other;
	beg = beg < m_len ? beg : m_len;
	len = ((beg + len) < m_len) ? len : m_len - beg;

	if (other.m_capacity < len + 1)
		other.reserve(len + 1);
	other.m_len = len;

	std::memcpy(other.m_data, &(m_data[beg]), len);
	other.m_data[len + 1] = '\0';

	return other;
}

const uint32_t& ML_String::length() const
{
	return m_len;
}

const PoolPointer<char>& ML_String::data() const
{
	return m_data;
}

// Reserve a new capacity for the vector
uint32_t ML_String::reserve(const uint32_t& capacity)
{
	if (capacity < m_capacity)
	{
		throw std::invalid_argument("Capacity too small! ML_String.reserve() cannot be called to reduce the capacity of the string!");
		std::terminate();
	}

	// Provide a temporary copy of the data
	char* temp = (char*)MemLib::spush(m_capacity);

	// Free the old pool pointer and allocate a new one, if need be
	std::memcpy(temp, &(*m_data), m_capacity);
	MemLib::pfree(m_data);

	m_data = MemLib::palloc(capacity);

	// Copy the data over to the new location and pop the temp from the stack
	std::memcpy(&(*m_data), temp, m_capacity);
	MemLib::spop();

	// Inform the new capacity
	return m_capacity = capacity;
};

// Clear the vector
void ML_String::clear()
{
	m_len = 0;
	m_data[0] = '\0';
}

// Return the internal string. Should not be kept between frames due to risk of defragmentation discoupling the data
const char* ML_String::c_str() const
{
	return m_data;
}

// Push an item into the back of the vector, returns the index of that item
const ML_String& ML_String::append(const ML_String& other)
{
	//validate_this();

	uint32_t newLen = m_len + other.m_len;
	// if the capacity of the vector is less than the size of the vector, reserve a larger chunk of memory
	while (m_capacity <= newLen)
		reserve(2 * m_capacity + ((newLen == 0) * 32));

	// Set data at location
	std::memcpy(&m_data[m_len], &(*other), other.m_len);
	m_data[newLen] = '\0';
	//ML_ZeroMemory(&m_data[newLen], m_capacity - newLen);

	// Set new length and return the object
	m_len = newLen;
	return (*this);
};

// Push an item into the back of the vector, returns the index of that item
const ML_String& ML_String::append(const char* other)
{
	//validate_this();

	uint32_t otherLen = (uint32_t)(std::strlen(other) + 1);
	uint32_t newLen = m_len + otherLen - 1;
	// if the capacity of the vector is less than the size of the vector, reserve a larger chunk of memory
	while (m_capacity <= newLen)
		reserve(m_capacity * 2 + ((newLen == 0) * 32));

	// Set data at location
	std::memcpy(&m_data[m_len], other, otherLen);
	m_data[newLen] = '\0';
	//ML_ZeroMemory(&m_data[newLen], m_capacity - newLen);

	// Set new length and return the object
	m_len = newLen;
	return (*this);
};

// Append an integer type number
const ML_String& ML_String::append(const long int& number)
{
	//// Should be enough to cover every concievable number
	//char* temp = (char*)MemLib::spush(64);
	//std::to_chars(temp, temp + 64, number);
	//append(temp);
	//// Pop
	//MemLib::spop();

	char temp[32];
	sprintf(temp, "%ld", number);
	append(temp);

	return (*this);
}

// Append a float type number
const ML_String& ML_String::append(const double& number)
{
	//// Should be enough to cover every concievable number
	//char* temp = (char*)MemLib::spush(64);
	//std::to_chars(temp, temp + 64, number);
	//append(temp);
	//// Pop
	//MemLib::spop();

	char temp[32];
	sprintf(temp, "%lf", number);
	append(temp);

	return (*this);
}

const char& ML_String::operator*() const
{
	return (*m_data);
};

char& ML_String::operator[](const uint32_t& idx)
{
	if (idx < 0 || m_len <= idx)
	{
		throw std::out_of_range("Index provided for ML_Vector is out of range!");
		std::terminate();
	}

	return m_data[idx];
};

ML_String& ML_String::operator=(const ML_String& other)
{
	// Update length
	m_len = other.m_len;
	m_capacity = other.m_capacity;

	MemLib::pfree(m_data);
	m_data = MemLib::palloc(m_capacity);

	// Deep copy data
	std::memcpy(m_data, other.m_data, m_len);
	m_data[m_len] = '\0';
	//ML_ZeroMemory(&(m_data[m_len]), m_capacity - m_len);

	return *this;
}

// Initiate an ML_Vector<T> with a number of T objects, can be called as such to emulate normal C++ style coding
// ML_Vector<T>() = { args };
ML_String::ML_String()
{
	// Set capacity
	m_capacity = 32;
	m_len = 0;
	m_data = MemLib::palloc(m_capacity);
};

ML_String::ML_String(const char* c_str)
{
	// Set capacity
	m_capacity = 32;
	m_len = 0;
	m_data = MemLib::palloc(m_capacity);

	append(c_str);
}

ML_String::ML_String(const ML_String& ml_str)
{
	// Set capacity
	m_capacity = 32;
	m_len = 0;
	m_data = MemLib::palloc(m_capacity);

	append(ml_str);
}

// Comparison with other ML_String
bool ML_String::operator==(const ML_String& other) const
{
	return (m_len == other.m_len) && (0 == std::strcmp(m_data, other.m_data));
}

// Comparison with c string
bool ML_String::operator==(const char*& other) const
{
	return (0 == std::strcmp(m_data, other));
}

ML_String::~ML_String()
{
	MemLib::pfree(m_data);
}