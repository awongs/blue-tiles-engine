#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Behaviour.h"
#include "../../util/MeshManager.h"

class Texture;
using meshmanager::Vertex;

class MeshRenderer : public Behaviour
{
public:
	// Constructor.
	MeshRenderer(std::string objFilePath);
	
	// Sets up the vertex, index, and VAO buffers.
	void SetupBuffers();

	// Loads and sets the current texture.
	void SetTexture(std::string texturePath);

	// Destructor.
	virtual ~MeshRenderer();

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

private:
	// ID to the vertex buffer object
	GLuint m_vertexBufferObjectID;

	// ID to the indices buffer object
	GLuint m_indicesBufferObjectID;

	// ID to the vertex array object
	GLuint m_vertexArrayObjectID;
	
	// This mesh's texture.
	std::unique_ptr<Texture> m_texture;
	
	// This mesh's vertices.
	std::vector<Vertex> m_vertices;

	// This mesh's indices.
	std::vector<GLuint> m_indices;
};