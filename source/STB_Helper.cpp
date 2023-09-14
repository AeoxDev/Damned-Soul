#include <iostream>
#include "STB_Helper.hpp"
#include "MemLib\MemLib.hpp"

//#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

#define CHANNELS 4

Image::Image(const char* filepath)
{
	this->load(filepath);
}

bool Image::load(const char* filepath)
{
	unsigned char* stbData = stbi_load(filepath, &this->width, &this->height, &this->channels, 0);

	if (nullptr == stbData)
	{
		std::cout << "Error loading " << filepath << std::endl;
		return false;
	}

	//// Expand to four channels if there aren't four already
	//if (CHANNELS != this->channels)
	//{
	//	const unsigned int withCorrect = this->width * this->height * CHANNELS;
	//	unsigned char* expanededTextureData = new unsigned char[withCorrect];
	//	for (int y = 0; y < this->height; y++)
	//	{
	//		for (int x = 0; x < this->width; x++)
	//		{
	//			int pixel = (x + (y * this->width)) * CHANNELS;
	//			expanededTextureData[pixel] = this->data[(x + (y * this->width)) * this->channels];
	//			if (this->channels == 1)
	//			{
	//				expanededTextureData[pixel + 1] = expanededTextureData[pixel];
	//				expanededTextureData[pixel + 2] = expanededTextureData[pixel];
	//			}
	//			else
	//			{
	//				expanededTextureData[pixel + 1] = this->data[1 + (x + (y * width)) * this->channels];
	//				expanededTextureData[pixel + 2] = this->data[2 + (x + (y * width)) * this->channels];
	//			}

	//			expanededTextureData[pixel + 3] = 255;
	//		}
	//	}
	//	// Delete the current data
	//	stbi_image_free(this->data);
	//	// Set the new data and channel count
	//	this->data = expanededTextureData;
	//	this->channels = CHANNELS;
	//}

	// Make a deep copy of the data
	unsigned int size = this->width * this->height * this->channels;
	this->data = MemLib::palloc(size);
	memcpy(&(this->data), stbData, size);

	// Free with STB
	stbi_image_free(stbData);

	return true;
}

Image::~Image()
{
	if (dontFree)
		return;
	MemLib::pfree(this->data);
}