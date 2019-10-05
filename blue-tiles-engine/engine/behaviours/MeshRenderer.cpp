#include "MeshRenderer.h"
#include "../debugbt/DebugLog.h"
#include "../../util/FileManager.h"
#include "../graphics/Texture.h"

MeshRenderer::MeshRenderer(GLuint gameObjectId)
	: Behaviour(gameObjectId, BehaviourType::MeshRenderer)
{
	meshmanager::LoadObj("../Assets/models/golden_goose.obj", m_vertices, m_indices);
	// TODO : Identify game object ID here
	DebugLog::Info("Generating buffers for MeshRenderer");

	// Generate the buffers
	glGenBuffers(1, &m_vertexBufferObjectID);
	glGenBuffers(1, &m_indicesBufferObjectID);
	
	// Allocate data into the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(meshmanager::Vertex), &m_vertices[0], GL_STATIC_DRAW); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// Generate and setup the vertex array object
	glGenVertexArrays(1, &m_vertexArrayObjectID);
	glBindVertexArray(m_vertexArrayObjectID);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(meshmanager::Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(meshmanager::Vertex), (GLvoid*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	// Normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(meshmanager::Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);

	// Unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// -- Testing purposes --
	m_texture = filemanager::LoadTexture("../Assets/textures/crate.jpg");
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
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

bool MeshRenderer::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}
