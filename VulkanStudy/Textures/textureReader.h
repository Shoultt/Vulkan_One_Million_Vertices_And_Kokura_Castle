#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

class textureReader
{
public:
	int tWidth, tHeight, tChannels;
	stbi_uc* texture;
	
	void load(const char* filename)
	{
		texture = stbi_load(filename, &tWidth, &tHeight, &tChannels, STBI_rgb_alpha);
	}

	int getTextureWidth()
	{
		return tWidth;
	}

	int getTextureHeight()
	{
		return tHeight;
	}

	void* getPicture()
	{
		return texture;
	}

	uint32_t getTextureSize()
	{
		return getTextureWidth() * getTextureHeight() * 4;
	}

	void destroy()
	{
		stbi_image_free(texture);
	}
};