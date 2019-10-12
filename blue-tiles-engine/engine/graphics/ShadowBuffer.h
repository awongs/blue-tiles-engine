#pragma once
#include <glad/glad.h>

class ShadowBuffer
{
public:
	// Constructor. Takes a width and height for the shadow texture.
	ShadowBuffer(int width, int height);

	// Destructor.
	virtual ~ShadowBuffer();

	// Accessor for buffer ID.
	GLuint GetBufferID() const;

	// Accessor for depth texture.
	GLuint GetDepthTexture() const;

	// Accessor for depth texture width.
	int GetWidth() const;

	// Accessor for depth texture height.
	int GetHeight() const;

private:
	// The ID of this shadow buffer.
	GLuint m_bufferID;

	// Depth texture for calculating shadows.
	GLuint m_depthTexture;

	// Width of the depth texture.
	int m_width;

	// Height of the depth texture.
	int m_height;
};