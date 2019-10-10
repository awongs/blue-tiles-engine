#include "ShadowBuffer.h"
#include "../debugbt/DebugLog.h"

ShadowBuffer::ShadowBuffer(int width, int height)
{
	DebugLog::Info("Setting up shadow buffer \n");

	// Generate the frame buffer
	glGenFramebuffers(1, &m_bufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);

	// Generate the depth texture
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Bind the texture to the buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	// Check for errors
	if (glGetError() != GL_NO_ERROR || glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		DebugLog::Warn("Error in SetupBuffers()");
	}

	// Go back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowBuffer::~ShadowBuffer()
{
	DebugLog::Info("Cleaning up ShadowBuffer #" + std::to_string(m_bufferID));

	glDeleteFramebuffers(1, &m_bufferID);
	glDeleteTextures(1, &m_depthTexture);
}

GLuint ShadowBuffer::GetBufferID() const
{
	return m_bufferID;
}

GLuint ShadowBuffer::GetDepthTexture() const
{
	return m_depthTexture;
}


