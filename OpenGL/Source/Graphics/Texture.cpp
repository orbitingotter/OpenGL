#include "pch.h"
#include "Texture.h"


#include <GL/glew.h>
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	: mID(0), mFilePath(filePath), mImageBuffer(nullptr), mWidth(0), mHeight(0), mChannels(0)
{
	stbi_set_flip_vertically_on_load(1);
	mImageBuffer = stbi_load(filePath.c_str(), &mWidth, &mHeight, &mChannels, 4);

	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImageBuffer);

	// unbind to later bind specific slot
	Unbind();

	if (mImageBuffer)
	{
		stbi_image_free(mImageBuffer);
	}
	else
	{
		std::cout << "ERROR : Texture ( " << filePath << " ) not loaded - " << stbi_failure_reason() << std::endl;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
