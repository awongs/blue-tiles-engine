#include <engine/GameObject.h>
#include "Rotate.h"


Rotate::Rotate(glm::vec3 velocity)
	: Behaviour(BehaviourType::Rotate)
	, m_velocity(velocity)
{
}

void Rotate::Update(float deltaTime)
{
	gameObject->rotation += m_velocity * deltaTime;
}

void Rotate::Draw(Shader& shader)
{
}

bool Rotate::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void Rotate::OnCollisionStay(GLuint other)
{
}
