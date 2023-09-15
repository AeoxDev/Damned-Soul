#pragma once
#include "MemLib\PoolPointer.hpp"

struct Image
{
	int32_t m_height = 0, m_width = 0, m_channels = 0;
	uint64_t m_hash = 0;
	PoolPointer<char> m_data;
	Image() = default;
	Image(const char* filepath);
	bool load(const char* filepath);
	bool dontFree = false;
	void Release();
	~Image();
};