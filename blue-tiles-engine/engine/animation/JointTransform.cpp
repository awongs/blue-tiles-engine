#include <glm/gtx/compatibility.hpp>

#include "JointTransform.h"

JointTransform::JointTransform(glm::vec3& position, glm::quat& rotation)
	: position(position)
	, rotation(rotation)
{
}

JointTransform::JointTransform(glm::mat4& transformationMatrix)
{
	position.x = transformationMatrix[3][0];
	position.y = transformationMatrix[3][1];
	position.z = transformationMatrix[3][2];
	rotation = glm::quat_cast(glm::mat3(transformationMatrix));
}

glm::mat4 JointTransform::GetLocalTransform() const
{
	glm::mat4 localTransform = glm::mat4(1);

	// Apply translation, rotation, and return.
	localTransform = glm::translate(localTransform, position);
	return localTransform * glm::mat4_cast(rotation);
}

JointTransform JointTransform::Interpolate(JointTransform& firstFrame, JointTransform& secondFrame, float progression)
{
	// Linear for position, spherical for rotation.
	glm::vec3 position = glm::lerp(firstFrame.position, secondFrame.position, progression);
	glm::quat rotation = glm::slerp(firstFrame.rotation, secondFrame.rotation, progression);
	return JointTransform(position, rotation);
}
