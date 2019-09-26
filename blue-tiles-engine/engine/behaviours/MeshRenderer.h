#pragma once

#include <glad/glad.h>
#include "Behaviour.h"

class MeshRenderer : Behaviour
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

	GLfloat m_verticesCube[8 * 3] = {
		// top side
		-0.5f, 0.5f, -0.5f, // v1
		0.5f, 0.5f, -0.5f, // v2
		-0.5f, 0.5f, 0.5f, // v3
		0.5f, 0.5f, 0.5f, // v4
		// bottom side
		-0.5f, -0.5f, -0.5f, // v5
		0.5f, -0.5f, -0.5f, // v6
		-0.5f, -0.5f, 0.5f, // v7
		0.5f, -0.5f, 0.5f, // v8
	};

	unsigned int m_indicesCube[12 * 3] = {  // note that we start from 0!
		0, 1, 2, // +y face (top)
		1, 3, 2,
		1, 3, 6, // +z face (front)
		3, 6, 7,
		6, 7, 4, // -y face (bottom)
		4, 7, 5,
		4, 5, 0, // -z face (back)
		0, 5, 1,
		0, 2, 4, // -x face (left)
		2, 6, 4,
		3, 1, 7, // +x face (right)
		1, 5, 7
	};

};