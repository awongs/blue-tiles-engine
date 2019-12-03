#include "BlockStarrtAnimation.h"

#include <engine/GameObject.h>

BlockStartAnimation::BlockStartAnimation(float delay)
	: Behaviour(BehaviourType::BlockStartAnimation)
	, m_delay(delay)
{
}

void BlockStartAnimation::Update(float deltaTime)
{
	if (!m_startAnimation) return;

	if (deltaTime > 0.1) return;

	m_time += deltaTime;

	if (m_time < m_delay) return;
	
	float sinceDelay = m_time - m_delay;
	
	gameObject->position = animationPosition + ((basePosition - animationPosition) * sinceDelay * 1.5f);

	if (sinceDelay * 1.5f > 1)
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
		m_time = 0;

		basePosition = gameObject->position;
		animationPosition = glm::vec3(basePosition.x, basePosition.y + 50, basePosition.z);
		gameObject->position = animationPosition;
	}
	return false;
}

void BlockStartAnimation::OnCollisionStay(GLuint other)
{
}
