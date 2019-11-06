#include <glm/gtx/compatibility.hpp>

#include "JointTransform.h"

JointTransform::JointTransform(glm::vec3& position, glm::quat& rotation)
	: position(position)
	, rotation(rotation)
{
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
