#include <glm/gtx/compatibility.hpp>
#include <iostream>
#include "JointTransform.h"

JointTransform::JointTransform(glm::vec3& position, glm::quat& rotation)
	: position(position)
	, rotation(rotation)
{
}

JointTransform::JointTransform(glm::mat4& transformationMatrix)
{
	// Extract position vector from the last row.
	// Note: GLM matrices seem to have operator[] implemented as [row][column] despite being column major.
	position.x = transformationMatrix[3][0];
	position.y = transformationMatrix[3][1];
	position.z = transformationMatrix[3][2];

	// Convert the transformation matrix into a 3x3 matrix, then into a quaternion.
	rotation = glm::quat_cast(glm::mat3(transformationMatrix));
}

glm::mat4 JointTransform::GetLocalTransform() const
{
	// Apply translation, rotation, and return.
	return glm::translate(glm::mat4(1), position) * glm::mat4_cast(rotation);
}

JointTransform JointTransform::Interpolate(JointTransform& firstFrame, JointTransform& secondFrame, float progression)
{
	// Linear for position, spherical for rotation.
	glm::vec3 position = glm::lerp(firstFrame.position, secondFrame.position, progression);
	glm::quat rotation = glm::slerp(firstFrame.rotation, secondFrame.rotation, progression);
	return JointTransform(position, rotation);
}
