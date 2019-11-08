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

	// Sets up the vertex, index, and VAO buffers.
	void SetupBuffers();

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

	// Shared pointer to the root joint in the skeleton hierarchy.
	std::shared_ptr<Joint> rootJoint;

	// Total number of joints.
	int jointCount;

private:
	// Constructs the joint hierarhcy from a .dae file.
	bool parseJointHierarchy(std::string skeletonPath);

	// Iterates through the skeleton to find all joint transforms.
	std::vector<glm::mat4> getJointTransforms();

	// Adds a joint's animated transform matrix to the list of transform matrices.
	// Recursively calls each joint in the skeleton hierarchy.
	void addJointsToArray(Joint& headJoint, std::vector<glm::mat4>& jointMatrices);

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