#include "ElectricSwitch.h"

#include <engine/MessageSystem.h>
#include <engine/GameObject.h>
#include <engine/Scene.h>
#include <engine/sound/SoundManager.h>
#include <engine/sound/Sound.h>

ElectricSwitch::ElectricSwitch(glm::vec3 originalPos)
	: Behaviour(BehaviourType::ElectricSwitch)
	, m_originalPos(originalPos)
	, m_startAnimation(false)
	, m_stillTouching(false)
	, m_touching(false)
	, m_time(0)
{
	m_activationPos = glm::vec3(m_originalPos.x, m_originalPos.y + 5, m_originalPos.z);
}

void ElectricSwitch::ActivateSwitch()
{
	m_startAnimation = true;
	m_time = 0;
	gameObject->position = m_originalPos;

	MessageSystem::BroadcastMessage(gameObject->id, BehaviourType::ElectricFloor, "ToggleElectricFloor");

	(SoundManager::getInstance().getSound("alarm"))->play();
}

void ElectricSwitch::Update(float deltaTime)
{
	if (m_touching && !m_stillTouching)
	{
		m_stillTouching = true;
	}
	else if (!m_touching && m_stillTouching)
	{
		m_stillTouching = false;
	}

	// consume touch
	m_touching = false;

	if (m_startAnimation)
	{
		m_time += deltaTime;

		gameObject->position = m_originalPos + ((m_activationPos - m_originalPos) * m_time * 2.f);

		if (m_time > 0.5f)
		{
			gameObject->position = m_activationPos;
			m_startAnimation = false;
		}
	}
	else
	{
		gameObject->position = gameObject->position + ((m_originalPos - gameObject->position) * deltaTime * 2.f);
	}
}

void ElectricSwitch::Draw(Shader& shader)
{
}

bool ElectricSwitch::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void ElectricSwitch::OnCollisionStay(GLuint other)
{
	GameObject* otherObj{ gameObject->currentScene->GetWorldGameObjectById(other) };

	if (otherObj->name != "player") return;

	m_touching = true;

	if (m_stillTouching) return;

	ActivateSwitch();
}
