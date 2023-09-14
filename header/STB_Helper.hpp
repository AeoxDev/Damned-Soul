#pragma once
#include "MemLib\PoolPointer.hpp"

struct Image
{
	int height, width, channels;
	PoolPointer<char> data;
	Image() = default;
	Image(const char* filepath);
	bool load(const char* filepath);
	bool dontFree = false;
	~Image();
};