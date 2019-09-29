#pragma once

#include <glad/glad.h>
#include "Behaviour.h"

class MeshRenderer : public Behaviour
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Update() override;
	void Draw() override;
	bool HandleMessage(std::string message) override;

private:
	// ID to the vertex buffer object
	GLuint m_vertexBufferObjectID;

	// ID to the indices buffer object
	GLuint m_indicesBufferObjectID;

	// ID to the vertex array object
	GLuint m_vertexArrayObjectID;

	// -- Testing Purposes --
	GLfloat m_verticesCube[24 * 3] = {
		// Front
		0.5, -0.5, 0.5,
		0.5,  0.5, 0.5,
		-0.5,  0.5, 0.5,
		-0.5, -0.5, 0.5,

		// Back
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,
		0.5,  0.5, -0.5,
		0.5, -0.5, -0.5,

		// Left
		-0.5, -0.5,  0.5,
		-0.5,  0.5,  0.5,
		-0.5,  0.5, -0.5,
		-0.5, -0.5, -0.5,

		// Right
		0.5, -0.5, -0.5,
		0.5,  0.5, -0.5,
		0.5,  0.5,  0.5,
		0.5, -0.5,  0.5,

		// Top
		0.5,  0.5,  0.5,
		0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,

		// Bottom
		0.5, -0.5, -0.5,
		0.5, -0.5,  0.5,
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5
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