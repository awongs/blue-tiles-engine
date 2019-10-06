#include "GeometryBuffer.h"
#include "../debugbt/DebugLog.h"

GeometryBuffer::GeometryBuffer(int screenWidth, int screenHeight)
{
	DebugLog::Info("Setting up geometry buffer \n");

	// Create the gBuffer
	glGenFramebuffers(1, &m_bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);

	// Position texture
	glGenTextures(1, &m_positionTexture);
	glBindTexture(GL_TEXTURE_2D, m_positionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positionTexture, 0);

	// Normal texture
	glGenTextures(1, &m_normalTexture);
	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalTexture, 0);

	// Color texture
	glGenTextures(1, &m_colourTexture);
	glBindTexture(GL_TEXTURE_2D, m_colourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colourTexture, 0);

	// Set color attachments for fragment shader
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Attach depth buffer
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	// Check for errors
	if (glGetError() != GL_NO_ERROR || glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		DebugLog::Warn("Error in SetupBuffers()");
	}

	// Go back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GeometryBuffer::~GeometryBuffer()
{
	DebugLog::Info("Cleaning up GeometryBuffer #" + std::to_string(m_bufferID));

	glDeleteFramebuffers(1, &m_bufferID);
	glDeleteRenderbuffers(1, &m_depthBuffer);
	glDeleteTextures(1, &m_positionTexture);
	glDeleteTextures(1, &m_normalTexture);
}

GLuint GeometryBuffer::GetBufferID() const
{
	return m_bufferID;
}

GLuint GeometryBuffer::GetPositionTexture() const
{
	return m_positionTexture;
}

GLuint GeometryBuffer::GetNormalTexture() const
{
	return m_normalTexture;
}

GLuint GeometryBuffer::GetColourTexture() const
{
	return m_colourTexture;
}
