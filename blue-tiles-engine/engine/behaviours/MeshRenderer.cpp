#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
	: Behaviour(BehaviourType::MeshRenderer)
{
	// Generate the buffers
	glGenVertexArrays(1, &m_vertexAttributeObjectID);
	

	glGenBuffers(1, &m_vertexBufferObjectID);
	glGenBuffers(1, &m_indicesBufferObjectID);
	
	glBindVertexArray(m_vertexAttributeObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube), m_verticesCube, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indicesCube), m_indicesCube, GL_STATIC_DRAW);

	// -- Testing --
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
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
	glDrawElements(GL_TRIANGLES, sizeof(m_indicesCube), GL_UNSIGNED_INT, 0);
}

bool MeshRenderer::HandleMessage(std::string message)
{
	return false;
}
