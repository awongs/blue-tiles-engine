#pragma once
#include <glad\glad.h>
#include <string>

class Texture
{
public:
	// Constructor, takes image properties as parameters.
	Texture(void* imageData, int width, int height, int numberOfChannels);

	// Destructor.
	virtual ~Texture();

	// Accessor for texture ID.
	GLuint GetTextureID() const;

	// Accessor for image width.
	int GetWidth() const;

	// Accessor for image height.
	int GetHeight() const;

	// Accessor for number of channels.
	int GetNumberOfChannels() const;
	
private:
	// The texture ID generated by OpenGL.
	GLuint textureID;

	// The width of the texture.
	int width;

	// The height of the texture.
	int height;

	// The number of channels in the image.
	// e.g. RGB has 3 while RBGA has 4.
	int numberOfChannels;
};