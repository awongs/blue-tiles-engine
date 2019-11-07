#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "../behaviours/MeshRenderer.h"
#include "../../util/MeshManager.h"
#include "Joint.h"

class Texture;
class Animator;

using meshmanager::AnimatedVertex;

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class AnimatedMesh : public Behaviour // TODO: Maybe try to inherit from MeshRenderer instead.
{
public:
	AnimatedMesh(std::string objPath, std::string skeletonPath, std::shared_ptr<Joint> rootJoint, int jointCount);

	// Loads and sets the current texture.
	void SetTexture(std::string texturePath);

	void SetupBuffers();

	// TODO: Load animation info somewhere.

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

	// TODO: Lazy copying
	std::shared_ptr<Joint> rootJoint;

	// Total number of joints.
	int jointCount;

	// Weak pointer to the animator behaviour.
	std::weak_ptr<Animator> animator;

private:
	bool parseJointHierarchy(std::string skeletonPath);

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