#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Behaviour.h"

class MeshRenderer : public Behaviour
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Update(float deltaTime) override;
	void Draw() override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

private:
	// ID to the vertex buffer object
	GLuint m_vertexBufferObjectID;

	// ID to the indices buffer object
	GLuint m_indicesBufferObjectID;

	// ID to the vertex array object
	GLuint m_vertexArrayObjectID;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	std::vector<GLuint> m_indices;
};