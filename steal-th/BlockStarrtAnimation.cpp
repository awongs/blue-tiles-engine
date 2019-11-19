#include "BlockStarrtAnimation.h"

#include <engine/GameObject.h>

void BlockStartAnimation::Update(float deltaTime)
{
	if (!m_startAnimation) return;

	m_time += deltaTime;

	if (m_time < m_delay) return;
	
	float sinceDelay = m_time - m_delay;
	
	gameObject->position = animationPosition + ((basePosition - animationPosition) * sinceDelay);

	if (sinceDelay > 1)
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

		basePosition = gameObject->position;
		animationPosition = glm::vec3(basePosition.x, basePosition.y + 10, basePosition.z);
	}
	return false;
}

void BlockStartAnimation::OnCollisionStay(GLuint other)
{
}
