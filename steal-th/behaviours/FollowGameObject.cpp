#include <glm/gtx/compatibility.hpp>

#include <engine/graphics/Camera.h>
#include <engine/GameObject.h>

#include "FollowGameObject.h"

FollowGameObject::FollowGameObject(glm::vec3 offset, float lerpSpeed)
	: Behaviour(BehaviourType::FollowGameObject)
	, m_offset(offset)
	, m_lerpSpeed(lerpSpeed)
{
}

void FollowGameObject::Update(float deltaTime)
{
	// Calculate destination
	glm::vec3 destination = gameObject->position + m_offset;
	glm::vec3 lerpedDestination = glm::lerp(Camera::GetInstance().GetPosition(), destination, m_lerpSpeed);

	// Move to destination
	Camera::GetInstance().SetPosition(lerpedDestination);
}

void FollowGameObject::Draw(Shader& shader)
{
	// Do nothing
}

bool FollowGameObject::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}
