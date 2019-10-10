#pragma once
#include <glad/glad.h>

class ShadowBuffer
{
public:
	// Constructor.
	ShadowBuffer(int screenWidth, int screenHeight);

	// Destructor.
	virtual ~ShadowBuffer();

	// Accessor for buffer ID.
	GLuint GetBufferID() const;

	// Accessor for depth texture.
	GLuint GetDepthTexture() const;

private:
	// The ID of this shadow buffer.
	GLuint m_bufferID;

	// Depth texture for calculating shadows.
	GLuint m_depthTexture;
};