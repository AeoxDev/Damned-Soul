#include <string>
#include <assert.h>
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
	
	//assert(nullptr != data);
	if (data)
	{
		// Make a deep copy of the data
		unsigned int size = this->m_width * this->m_height * this->m_channels;
		this->m_data = MemLib::palloc(size);
		memcpy(&(*(this->m_data)), &(data[0]), size);

		// Free with STB
		stbi_image_free((void*)data);

		return true;
	}
#define DEFAULT_COLOR_CELL_DIMENSION (8)
#define WIDTH_HEIGHT_DEFAULT_MISSING (4 * DEFAULT_COLOR_CELL_DIMENSION)
#define CHANNELS_DEFAULT_MISSING (4)
	m_height = m_width = WIDTH_HEIGHT_DEFAULT_MISSING;
	m_channels = CHANNELS_DEFAULT_MISSING;
	m_data = MemLib::palloc(WIDTH_HEIGHT_DEFAULT_MISSING * WIDTH_HEIGHT_DEFAULT_MISSING * CHANNELS_DEFAULT_MISSING);
	struct M_IMAGE_PIXEL
	{
		uint8_t c[CHANNELS_DEFAULT_MISSING] = { 255 };
	};
	for (uint8_t y = 0; y < WIDTH_HEIGHT_DEFAULT_MISSING; ++y)
	{
		for (uint8_t x = 0; x < WIDTH_HEIGHT_DEFAULT_MISSING; ++x)
		{
			M_IMAGE_PIXEL& current = ((M_IMAGE_PIXEL*)&(*this->m_data))[(y * WIDTH_HEIGHT_DEFAULT_MISSING + x)];
			uint8_t even = ((x / DEFAULT_COLOR_CELL_DIMENSION) + (y / DEFAULT_COLOR_CELL_DIMENSION)) % 2;
			current.c[0] = 250 * even;
			current.c[1] = 3 * even;
			current.c[2] = 216 * even;
			current.c[3] = 255;
		}
	}
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