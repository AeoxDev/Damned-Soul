#pragma once
#include "MemLib\PoolPointer.hpp"
#include <cinttypes>


struct ML_String
{
private:
	// Pool pointer to internal data
	PoolPointer<char> m_data;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_len;
	// Due to our memory usage restriction, a size larger than 2^30 would be guaranteed to exceed memory limits
	uint32_t m_capacity;

public:
	char* begin() const;

	char* end() const;

	uint32_t find_first_of(const char* other);

	uint32_t find_last_of(const char* other);

	ML_String substr(uint32_t beg, uint32_t len);

	const uint32_t& length() const;

	const PoolPointer<char>& data() const;

	// Reserve a new capacity for the vector
	uint32_t reserve(const uint32_t& capacity);

	// Clear the vector
	void clear();

	// Return the internal string. Should not be kept between frames due to risk of defragmentation discoupling the data
	const char* c_str() const;

	// Push an item into the back of the vector, returns the index of that item
	const ML_String& append(const ML_String& other);

	// Push an item into the back of the vector, returns the index of that item
	const ML_String& append(const char* other);

	// Append an integer type number
	const ML_String& append(const long int& number);

	// Append a float type number
	const ML_String& append(const double& number);

	const char& operator*() const;

	char& operator[](const uint32_t& idx);

	ML_String& operator=(const ML_String& other);

	// Initiate an ML_Vector<T> with a number of T objects, can be called as such to emulate normal C++ style coding
	// ML_Vector<T>() = { args };
	ML_String();

	ML_String(const char* c_str);

	ML_String(const ML_String& ml_str);

	// Comparison with other ML_String
	bool operator==(const ML_String& other) const;

	// Comparison with c string
	bool operator==(const char*& other) const;

	~ML_String();
};