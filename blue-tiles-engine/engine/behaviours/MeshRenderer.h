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
	MeshRenderer(GLuint gameObjectId, std::string objFilePath);
	
	void SetupBuffers();

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
};