#include "ElectricFloor.h"
#include <engine/GameObject.h>

ElectricFloor::ElectricFloor(glm::vec3 originalPos, bool isActive)
	: Behaviour(BehaviourType::ElectricFloor)
	, m_originalPos(originalPos)
	, m_isActive(isActive)
{
	m_disabledPos = glm::vec3(m_originalPos.x - 1000, m_originalPos.y, m_originalPos.z - 1000);
}

void ElectricFloor::UpdateActiveState()
{
	if (m_isActive) gameObject->position = m_originalPos;
	else gameObject->position = m_disabledPos;
}

void ElectricFloor::Update(float deltaTime)
{
}

void ElectricFloor::Draw(Shader& shader)
{
}

bool ElectricFloor::HandleMessage(unsigned int senderID, std::string& message)
{
	if (message == "ToggleElectricFloor")
	{
		m_isActive = !m_isActive;

		UpdateActiveState();
	}

	return false;
}

void ElectricFloor::OnCollisionStay(GLuint other)
{
}
