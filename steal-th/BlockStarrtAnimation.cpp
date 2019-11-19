#include "BlockStarrtAnimation.h"

#include <engine/GameObject.h>

BlockStartAnimation::BlockStartAnimation()
	: Behaviour(BehaviourType::BlockStartAnimation)
{
}

void BlockStartAnimation::Update(float deltaTime)
{
	if (!m_startAnimation) return;

	if (deltaTime > 0.1) return;

	m_time += deltaTime;

	if (m_time < m_delay) return;
	
	float sinceDelay = m_time - m_delay;
	
	gameObject->position = animationPosition + ((basePosition - animationPosition) * sinceDelay * 0.5f);

	if (sinceDelay > 2)
	{
		m_startAnimation = false;
		gameObject->position = basePosition;
	}
}

void BlockStartAnimation::Draw(Shader& shader)
{
}

bool BlockStartAnimation::HandleMessage(unsigned int senderID, std::string& message)
{
	
	if (message == "StartBlockAnimation")
	{

		m_startAnimation = true;

		m_delay = (glm::length(glm::vec2(gameObject->position.x / 9, gameObject->position.y / 9))) * 0.1;
		m_time = 0;

		basePosition = gameObject->position;
		animationPosition = glm::vec3(basePosition.x, basePosition.y + 100, basePosition.z);
		gameObject->position = animationPosition;
	}
	return false;
}

void BlockStartAnimation::OnCollisionStay(GLuint other)
{
}
