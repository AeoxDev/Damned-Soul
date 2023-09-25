#include <iostream>
#include <string>
#include "STB_Helper.h"
#include "MemLib\MemLib.hpp"

#ifndef  _DEBUG
#define STB_IMAGE_IMPLEMENTATION
#endif // ! _DEBUG
#include "STB_Image\stb_image.h"

#define CHANNELS 4

Image::Image(const char* filepath)
{
	this->load(filepath);
}

bool Image::load(const char* filename)
{
	// A price to pay for salvation...
	std::string filepath("Textures");
	filepath.append(filename);

	unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, nullptr, CHANNELS);
	m_channels = CHANNELS; // Should always be 4
	
	if (nullptr == data)
	{
		std::cout << "Error loading " << filepath << std::endl;
		return false;
	}

	// Make a deep copy of the data
	unsigned int size = this->m_width * this->m_height * this->m_channels;
	this->m_data = MemLib::palloc(size);
	memcpy(&(*(this->m_data)), &(data[0]), size);

	// Free with STB
	stbi_image_free((void*)data);

	return true;
}

void Image::Release()
{
	MemLib::pfree(m_data);
}

Image::~Image()
{
	if (dontFree)
		return;
	MemLib::pfree(m_data);
}