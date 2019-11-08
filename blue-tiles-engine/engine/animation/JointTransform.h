#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class JointTransform
{
public:
	// Constructor that takes a position vector and a rotation quaternion.
	JointTransform(glm::vec3& position, glm::quat& rotation);

	// Constructor that takes a transformation matrix.
	JointTransform(glm::mat4& transformationMatrix);

	// Returns the local transform of this joint.
	glm::mat4 GetLocalTransform() const;

	// Calculates and returns an interpolated joint transform between two frames.
	static JointTransform Interpolate(JointTransform& firstFrame, JointTransform& secondFrame, float progression);

	// Position relative to the parent bone.
	glm::vec3 position;

	// Rotation relative to the parent bone.
	glm::quat rotation;
};