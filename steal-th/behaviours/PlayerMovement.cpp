#include <engine/input/Input.h>
#include <engine/GameObject.h>

#include "PlayerMovement.h"

PlayerMovement::PlayerMovement(float speed)
	: Behaviour(BehaviourType::PlayerMovement)
	, m_speed(speed)
{
}

void PlayerMovement::Update(float deltaTime)
{
	bool isKeyUp{ Input::GetInstance().IsKeyDown(Input::INPUT_UP) };
	bool isKeyDown{ Input::GetInstance().IsKeyDown(Input::INPUT_DOWN) };
	bool isKeyLeft{ Input::GetInstance().IsKeyDown(Input::INPUT_LEFT) };
	bool isKeyRight{ Input::GetInstance().IsKeyDown(Input::INPUT_RIGHT) };

	// Update current velocity based on input.
	if (isKeyUp)
	{
		m_currentVelocity.z = -m_speed * deltaTime;
	}
	if (isKeyDown)
	{
		m_currentVelocity.z = m_speed * deltaTime;
	}
	if (isKeyLeft)
	{
		m_currentVelocity.x = -m_speed * deltaTime;
	}
	if (isKeyRight)
	{
		m_currentVelocity.x = m_speed * deltaTime;
	}

	// Stop moving if no input pressed.
	if (!isKeyUp && !isKeyDown)
	{
		m_currentVelocity.z = 0.f;
	}
	if (!isKeyLeft && !isKeyRight)
	{
		m_currentVelocity.x = 0.f;
	}

	// Update position based on current velocity.
	gameObject->position += (m_currentVelocity);
}

void PlayerMovement::Draw(Shader& shader)
{
	// Do nothing
}

bool PlayerMovement::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}

void PlayerMovement::OnCollisionStay(GLuint other)
{
}

glm::vec3 PlayerMovement::GetCurrentVelocity() const
{
	return m_currentVelocity;
}

