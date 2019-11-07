#include "AnimatedMesh.h"
#include "../../util/FileManager.h"

#include "../debugbt/DebugLog.h"
#include "../GameObject.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"

AnimatedMesh::AnimatedMesh(std::string objPath, std::string skeletonPath, std::shared_ptr<Joint> rootJoint, int jointCount)
	: Behaviour(BehaviourType::AnimatedMesh)
	, rootJoint(rootJoint)
	, jointCount(jointCount)
{
	meshmanager::LoadObjWithSkeleton(objPath, skeletonPath, m_vertices, m_indices);

	SetupBuffers();
}

void AnimatedMesh::SetTexture(std::string texturePath)
{
	m_texture = filemanager::LoadTexture(texturePath);
}

void AnimatedMesh::SetupBuffers()
{
	if (m_vertices.empty() || m_indices.empty())
	{
		DebugLog::Warn("AnimatedMesh created with no vertices or indices");
		return;
	}

	// Generate the buffers
	glGenBuffers(1, &m_vertexBufferObjectID);
	glGenBuffers(1, &m_indicesBufferObjectID);

	// Allocate data into the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(AnimatedVertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// Generate and setup the vertex array object
	glGenVertexArrays(1, &m_vertexArrayObjectID);
	glBindVertexArray(m_vertexArrayObjectID);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	// Normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);

	// Joint Ids
	glVertexAttribIPointer(3, 3, GL_INT, sizeof(AnimatedVertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);

	// Joint weights
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::ivec3)));
	glEnableVertexAttribArray(3);

	if (glGetError() != GL_NO_ERROR)
	{
		DebugLog::Warn("Error in setting up AnimatedMesh");
	}

	// Unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AnimatedMesh::Update(float deltaTime)
{
}

void AnimatedMesh::Draw(Shader& shader)
{
	// Bind this mesh's buffers
	glBindVertexArray(m_vertexArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);

	// Update transform matrix
	gameObject->UpdateTransformMatrix();

	// Set model matrix in shader
	shader.SetUniformMatrix4fv("model", gameObject->GetTransformMatrix());

	// Bind the texture
	if (m_texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
	}

	// Draw the mesh
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

bool AnimatedMesh::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void AnimatedMesh::OnCollisionStay(GLuint other)
{
}
