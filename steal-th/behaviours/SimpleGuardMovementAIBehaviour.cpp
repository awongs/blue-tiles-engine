#include "SimpleGuardMovementAIBehaviour.h"
#include "SimpleGuardMovementAction.h"
#include "../../blue-tiles-engine/engine/GameObject.h"
#include <glm/glm.hpp>

SimpleGuardMovementAIBehaviour::SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed)
	: Behaviour(BehaviourType::SimpleGuardMovementBehaviour)
	, m_movementSpeed(movementSpeed)
	, m_rotationSpeed(rotationSpeed)
{
}

SimpleGuardMovementAIBehaviour::SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed, std::vector<SimpleGuardMovementAction> actionsToAdd)
	: Behaviour(BehaviourType::SimpleGuardMovementBehaviour)
	, m_movementSpeed(movementSpeed)
	, m_rotationSpeed(rotationSpeed)
{
	AddActions(actionsToAdd);
}

SimpleGuardMovementAIBehaviour::SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed, SimpleGuardMovementAction* actionsToAdd, size_t count)
	: Behaviour(BehaviourType::SimpleGuardMovementBehaviour)
	, m_movementSpeed(movementSpeed)
	, m_rotationSpeed(rotationSpeed)
{
	AddActions(actionsToAdd, count);
}

void SimpleGuardMovementAIBehaviour::AddAction(SimpleGuardMovementAction actionToAdd)
{
	m_actions.push_back(actionToAdd);
}

void SimpleGuardMovementAIBehaviour::AddActions(std::vector<SimpleGuardMovementAction> actionsToAdd)
{
	for (int i = 0; i < actionsToAdd.size; i++)
	{
		m_actions.push_back(actionsToAdd[i]);
	}
}

void SimpleGuardMovementAIBehaviour::AddActions(SimpleGuardMovementAction* actionsToAdd, size_t count)
{
	for (int i = 0; i < count; i++)
	{
		m_actions.push_back(actionsToAdd[i]);
	}
}

void SimpleGuardMovementAIBehaviour::Update(float deltaTime)
{
	if (m_isPaused) return;

	bool isDone = ProcessAction(deltaTime, m_actions[m_actionIndex]);

	if (isDone)
	{
		m_actionIndex++;

		if (m_actionIndex >= m_actions.size)
		{
			m_actionIndex = 0;
		}
	}
}

void SimpleGuardMovementAIBehaviour::Draw(Shader& shader)
{
}

bool SimpleGuardMovementAIBehaviour::HandleMessage(unsigned int senderID, std::string message)
{
	if (message == "Pause SGMAI") m_isPaused = true;

	else if (message == "Resume SGMAI") m_isPaused = false;

	return false;
}

bool SimpleGuardMovementAIBehaviour::ProcessAction(float deltaTime, SimpleGuardMovementAction& action)
{
	switch (action.actionType)
	{
	case SGMAType::Wait:

		m_actionTime += deltaTime;

		if (m_actionTime > action.waitDuration)
		{
			m_actionTime = 0;
			return true;
		}

		return false;

	case SGMAType::Move:

		return ProcessMoveAction(deltaTime, action);

	case SGMAType::MoveAndTurn:

		return ProcessMoveAction(deltaTime, action) && ProcessTurnAction(deltaTime, action);

	case SGMAType::Turn:

		return ProcessTurnAction(deltaTime, action);

	}

	return false;
}

bool SimpleGuardMovementAIBehaviour::ProcessMoveAction(float deltaTime, SimpleGuardMovementAction& action)
{
	// check if close enough yet

	glm::vec3 dest = glm::vec3(action.destinationX, 0.0f, action.destinationZ);

	float dist = glm::distance(gameObject->position, dest);

	if (dist < epsilon) return true;

	// move towards it based on position

	// movement vector	(normalize(dest - pos))
	glm::vec3 movement = glm::normalize(dest - gameObject->position) * (m_movementSpeed * deltaTime);

	glm::vec3 nextPos = gameObject->position + movement;

	// check for overshoot
	float nowAndThenDifference = glm::distance(gameObject->position, nextPos);

	// went past the destination
	if (nowAndThenDifference > dist)
	{
		dest.y = gameObject->position.y;
		gameObject->position = dest;
	}
	else
	{
		gameObject->position = nextPos;
	}

	return false;
}

bool SimpleGuardMovementAIBehaviour::ProcessTurnAction(float deltaTime, SimpleGuardMovementAction& action)
{
	return false;
}
