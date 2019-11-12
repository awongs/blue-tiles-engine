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

	// Accessor for transparency.
	bool IsTransparent();

	// Setter for transparency.
	void SetTransparent(bool transparent);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

private:
	// ID to the vertex buffer object
	GLuint m_vertexBufferObjectID;

	// ID to the indices buffer object
	GLuint m_indicesBufferObjectID;

	// ID to the vertex array object
	GLuint m_vertexArrayObjectID;
	
	// This mesh's texture.
	std::shared_ptr<Texture> m_texture;
	
	// This mesh's vertices.
	std::vector<Vertex> m_vertices;

	// This mesh's indices.
	std::vector<GLuint> m_indices;

	// Flag for if this mesh is transprent.
	bool m_isTransparent;

	// Model matrix string constant to avoid unnecessary string creation.
	static const std::string MODEL_MATRIX;
};