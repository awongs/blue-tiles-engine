#include "MeshRenderer.h"
#include "../debugbt/DebugLog.h"
#include "../../util/FileManager.h"
#include "../graphics/Texture.h"

MeshRenderer::MeshRenderer()
	: Behaviour(BehaviourType::MeshRenderer)
{
	// TODO : Identify game object ID here
	DebugLog::Info("Generating buffers for MeshRenderer");

	// Generate the buffers
	glGenBuffers(1, &m_vertexBufferObjectID);
	glGenBuffers(1, &m_indicesBufferObjectID);
	
	// Allocate data into the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube), m_verticesCube, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indicesCube), m_indicesCube, GL_STATIC_DRAW);

	// Generate and setup the vertex array object
	glGenVertexArrays(1, &m_vertexArrayObjectID);
	glBindVertexArray(m_vertexArrayObjectID);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// -- Testing purposes --
	m_texture = filemanager::LoadTexture("../Assets/crate.jpg");
}

MeshRenderer::~MeshRenderer()
{
	// Cleanup the buffers
	glDeleteBuffers(1, &m_vertexBufferObjectID);
	glDeleteBuffers(1, &m_indicesBufferObjectID);
	glDeleteVertexArrays(1, &m_vertexArrayObjectID);
}

void MeshRenderer::Update(float deltaTime)
{
}

void MeshRenderer::Draw()
{
	// Bind this mesh's buffers
	glBindVertexArray(m_vertexArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);

	// Bind the texture
	if (m_texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
	}

	// Draw the mesh
	glDrawElements(GL_TRIANGLES, sizeof(m_indicesCube), GL_UNSIGNED_INT, 0);
}

bool MeshRenderer::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}
