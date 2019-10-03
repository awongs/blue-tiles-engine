#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Behaviour.h"
#include "../../util/MeshManager.h"

class Texture;

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
	
	// This mesh's texture.
	std::shared_ptr<Texture> m_texture;

	std::vector<meshmanager::Vertex> m_vertices;
	std::vector<GLuint> m_indices;

	// -- Testing Purposes --
	GLfloat m_verticesCube[24 * 5] = {
		// Front
		0.5, -0.5, 0.5,    1, 0,
		0.5,  0.5, 0.5,    1, 1,
		-0.5,  0.5, 0.5,   0, 1,
		-0.5, -0.5, 0.5,   0, 0,

		// Back
		-0.5, -0.5, -0.5,  1, 0,
		-0.5,  0.5, -0.5,  1, 1,
		0.5,  0.5, -0.5,   0, 1,
		0.5, -0.5, -0.5,   0, 0,

		// Left
		-0.5, -0.5,  0.5,  1, 0,
		-0.5,  0.5,  0.5,  1, 1,
		-0.5,  0.5, -0.5,  0, 1,
		-0.5, -0.5, -0.5,  0, 0,

		// Right
		0.5, -0.5, -0.5,   1, 0,
		0.5,  0.5, -0.5,   1, 1,
		0.5,  0.5,  0.5,   0, 1,
		0.5, -0.5,  0.5,   0, 0,

		// Top
		0.5,  0.5,  0.5,   1, 0,
		0.5,  0.5, -0.5,   1, 1,
		-0.5,  0.5, -0.5,  0, 1,
		-0.5,  0.5,  0.5,  0, 0,

		// Bottom
		0.5, -0.5, -0.5,   1, 0,
		0.5, -0.5,  0.5,   1, 1,
		-0.5, -0.5,  0.5,  0, 1,
		-0.5, -0.5, -0.5,  0, 0
	};

	unsigned int m_indicesCube[12 * 3] = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Back
		4, 5, 6,
		6, 7, 4,

		// Left
		8, 9, 10,
		10, 11, 8,

		// Right
		12, 13, 14,
		14, 15, 12,

		// Top
		16, 17, 18,
		18, 19, 16,

		// Bottom
		20, 21, 22,
		22, 23, 20
	};
};