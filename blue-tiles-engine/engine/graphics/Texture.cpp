#include <stb/stb_image.h>

#include "Texture.h"
#include "../debugbt/DebugLog.h"

Texture::Texture(unsigned char* imageData, int width, int height, int numberOfChannels)
	: width(width)
	, height(height)
	, numberOfChannels(numberOfChannels)
{
	// TODO:: Implement texture caching

	// Generate the texture
	glGenTextures(1, &textureID);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imageData != nullptr)
	{
		// Load image data onto the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(imageData);
	}
	else
	{
		DebugLog::Error("Failed to load texture, image data was null");
	}
}

Texture::~Texture()
{
	DebugLog::Info("Texture #" + std::to_string(textureID) + " destructor called.");
	glDeleteTextures(1, &textureID);
}

GLuint Texture::GetTextureID() const
{
	return textureID;
}

int Texture::GetWidth() const
{
	return width;;
}

int Texture::GetHeight() const
{
	return height;
}

int Texture::GetNumberOfChannels() const
{
	return numberOfChannels;
}
