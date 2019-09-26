#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
	: Behaviour(BehaviourType::MeshRenderer)
{
	// Generate the buffers
	glGenBuffers(1, &m_vertexBufferObjectID);
	glGenBuffers(1, &m_indicesBufferObjectID);
}

MeshRenderer::~MeshRenderer()
{
	// Cleanup the buffers
	glDeleteBuffers(1, &m_vertexBufferObjectID);
	glDeleteBuffers(1, &m_indicesBufferObjectID);
}

void MeshRenderer::Update()
{
}

void MeshRenderer::Draw()
{
	// Bind this mesh's buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);

	// Draw the mesh  TODO:: Input indices count here
	glDrawElements(GL_TRIANGLES, sizeof(0), GL_UNSIGNED_INT, 0);
}

bool MeshRenderer::HandleMessage(std::string message)
{
	return false;
}
