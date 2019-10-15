#pragma once
#include <glad/glad.h>

class GeometryBuffer
{
public:
	// Constructor.
	GeometryBuffer(int screenWidth, int screenHeight);
	
	// Destructor.
	virtual ~GeometryBuffer();

	// Accessor for buffer ID.
	GLuint GetBufferID() const;

	// Accessor for position texture.
	GLuint GetPositionTexture() const;

	// Accessor for normal texture.
	GLuint GetNormalTexture() const;

	// Accessor for colour texture.
	GLuint GetColourTexture() const;

private:
	// The ID of this geometry buffer.
	GLuint m_bufferID;

	// Depth buffer ID.
	GLuint m_depthBuffer;

	// Position texture in geometry buffer.
	GLuint m_positionTexture;

	// Normal texture in geometry buffer.
	GLuint m_normalTexture;

	// Color texture in geometry buffer.
	GLuint m_colourTexture;
};