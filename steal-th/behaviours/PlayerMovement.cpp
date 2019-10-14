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
	if (Input::GetInstance().IsKeyDown(Input::INPUT_UP))
	{
		gameObject->position.z -= m_speed * deltaTime;
	}
	if (Input::GetInstance().IsKeyDown(Input::INPUT_DOWN))
	{
		gameObject->position.z += m_speed * deltaTime;
	}
	if (Input::GetInstance().IsKeyDown(Input::INPUT_LEFT))
	{
		gameObject->position.x -= m_speed * deltaTime;
	}
	if (Input::GetInstance().IsKeyDown(Input::INPUT_RIGHT))
	{
		gameObject->position.x += m_speed * deltaTime;
	}
}

void PlayerMovement::Draw(Shader& shader)
{
	// Do nothing
}

bool PlayerMovement::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}

