#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "../behaviours/MeshRenderer.h"
#include "Joint.h"

class Texture;
class Animator;

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class AnimatedMesh : public Behaviour // TODO: Maybe try to inherit from MeshRenderer instead.
{
	struct AnimatedVertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::ivec3 jointIds;
		glm::vec3 jointWeights;
	};

public:
	AnimatedMesh(const std::string objFilePath);

	// Loads and sets the current texture.
	void SetTexture(std::string texturePath);

	// TODO: Load animation info somewhere.

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

	// TODO: Lazy copying
	Joint rootJoint;

	// Total number of joints.
	int jointCount;

	// Weak pointer to the animator behaviour.
	std::weak_ptr<Animator> animator;

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
	std::vector<AnimatedVertex> m_vertices;

	// This mesh's indices.
	std::vector<GLuint> m_indices;
};