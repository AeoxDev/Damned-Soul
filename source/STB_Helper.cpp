#include <string>
#include <assert.h>
#include "STB_Helper.h"
#include "MemLib\MemLib.hpp"
#include <vector>

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
	std::string in = filename ? filename : "";

	// Build list of candidate paths to try (cover common filename formats)
	std::vector<std::string> candidates;
	auto pushTexturePath = [&](const std::string& n) {
		if (n.empty()) return;
		// Ensure single separator between "Textures" and name
		if (n.front() == '/' || n.front() == '\\')
			candidates.push_back(std::string("VFX/Textures") + n);
		else
			candidates.push_back(std::string("VFX/Textures/") + n);
		};

	// 1) As provided relative to Textures (common)
	pushTexturePath(in);

	// 2) Provided raw (maybe already full or relative)
	if (!in.empty())
		candidates.push_back(in);

	// 3) Strip leading separators and try again under Textures
	std::string stripped = in;
	while (!stripped.empty() && (stripped.front() == '/' || stripped.front() == '\\'))
		stripped.erase(stripped.begin());
	if (!stripped.empty())
		pushTexturePath(stripped);

	// 4) Try adding .png if no extension
	auto hasExtension = [](const std::string& s) {
		auto p = s.find_last_of('.');
		if (p == std::string::npos) return false;
		auto slash = s.find_last_of("/\\");
		return (slash == std::string::npos) || (p > slash);
		};
	if (!hasExtension(in))
	{
		if (!in.empty()) candidates.push_back(in + ".png");
		if (!stripped.empty()) candidates.push_back(std::string("Textures/") + stripped + ".png");
	}

	// Deduplicate preserving order
	std::vector<std::string> uniq;
	for (auto& c : candidates)
	{
		if (std::find(uniq.begin(), uniq.end(), c) == uniq.end())
			uniq.push_back(c);
	}

	unsigned char* data = nullptr;
	for (auto& path : uniq)
	{
		data = stbi_load(path.c_str(), &m_width, &m_height, nullptr, CHANNELS);
		if (data)
		{
			// success: keep this path (optional: store it)
#ifdef _DEBUG
			printf("Image::load: loaded '%s' (w=%d h=%d)\n", path.c_str(), m_width, m_height);
#endif
			break;
		}
		else
		{
#ifdef _DEBUG
			const char* reason = stbi_failure_reason();
			printf("Image::load: failed to load '%s' -> %s\n", path.c_str(), reason ? reason : "unknown");
#endif
		}
	}

	m_channels = CHANNELS; // Should always be 4

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

	// Fallback checkerboard (existing behaviour)
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
/*

bool Image::load(const char* filename)
{
	// A price to pay for salvation...
	std::string filepath("VFX/Textures");
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
*/

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