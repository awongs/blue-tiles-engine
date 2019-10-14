#include "SimpleGuardMovementAIBehaviour.h"
#include "../../blue-tiles-engine/engine/GameObject.h"
#include <glm/glm.hpp>

float SimpleGuardMovementAIBehaviour::epsilon = 0.05f;

float SimpleGuardMovementAIBehaviour::epsilonSqr = epsilon * epsilon;

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
	for (int i = 0; i < actionsToAdd.size(); i++)
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

void SimpleGuardMovementAIBehaviour::AddWaitAction(float duration)
{
	AddAction(SimpleGuardMovementAction{
		SGMAType::Wait,	// type
		duration,		// duration
		0,				// x
		0,				// z
		false			// CW / CCW
	});
}

void SimpleGuardMovementAIBehaviour::AddMoveAction(float x, float z)
{
	AddAction(SimpleGuardMovementAction{
		SGMAType::Move,	// type
		0,				// duration
		x,				// x
		z,				// z
		false			// CW / CCW
		});
}

void SimpleGuardMovementAIBehaviour::AddMoveTileAction(int x, int z)
{
	AddAction(SimpleGuardMovementAction{
		SGMAType::Move,	// type
		0,				// duration
		(x * 9.0f) + 4.5f,		// x
		(z * 9.0f) + 4.5f,		// z
		false			// CW / CCW
		});
}

void SimpleGuardMovementAIBehaviour::AddTurnAction(bool rotateCCW)
{
	AddAction(SimpleGuardMovementAction{
		SGMAType::Turn,	// type
		0,				// duration
		0,				// x
		0,				// z
		rotateCCW		// CW / CCW
		});
}

void SimpleGuardMovementAIBehaviour::AddTurnCWAction()
{
	AddAction(SimpleGuardMovementAction{
		SGMAType::Turn,	// type
		0,				// duration
		0,				// x
		0,				// z
		false			// CW / CCW
		});
}

void SimpleGuardMovementAIBehaviour::AddTurnCCWAction()
{
	AddAction(SimpleGuardMovementAction{
		SGMAType::Turn,	// type
		0,				// duration
		0,				// x
		0,				// z
		true			// CW / CCW
		});
}

void SimpleGuardMovementAIBehaviour::Update(float deltaTime)
{
	if (m_isPaused) return;

	if (m_actions.size() == 0) return;

	if (deltaTime > 0.1f) return;

	bool isDone = ProcessAction(deltaTime, m_actions[m_actionIndex]);

	if (isDone)
	{
		m_actionIndex++;

		if (m_actionIndex >= m_actions.size())
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

		return ProcessTurnAction(deltaTime, action) && ProcessMoveAction(deltaTime, action);

	case SGMAType::Turn:
		
		return ProcessTurnAction(deltaTime, action);

	}

	return false;
}

bool SimpleGuardMovementAIBehaviour::ProcessMoveAction(float deltaTime, SimpleGuardMovementAction& action)
{
	// check if close enough yet

	glm::vec3 dest = glm::vec3(action.destinationX, gameObject->position.y, action.destinationZ);

	float dist = glm::distance(gameObject->position, dest);

	if (dist <= epsilon)
	{
		gameObject->position.x = dest.x;
		gameObject->position.y = dest.y;
		return true;
	}

	// move towards destination

	// check for overshoot
	float scaledSpeed = m_movementSpeed * deltaTime;

	// went past the destination
	if (scaledSpeed >= dist)
	{
		gameObject->position.x = dest.x;
		gameObject->position.y = dest.y;
		return true;
	}
	else
	{
		glm::vec3 direction = glm::normalize(dest - gameObject->position);
		gameObject->position = gameObject->position +  (direction * (m_movementSpeed * deltaTime));
	}

	return false;
}

bool SimpleGuardMovementAIBehaviour::ProcessTurnAction(float deltaTime, SimpleGuardMovementAction& action)
{
	// if new turn action, find the new target rotation
	if (!m_newRotationTarget)
	{
		if (action.rotateCCW) m_rotationTarget = gameObject->rotation.y + glm::radians(90.0f);
		else m_rotationTarget = gameObject->rotation.y - glm::radians(90.0f);

		m_newRotationTarget = true;
	}

	// difference between desired and current
	float diff = m_rotationTarget - gameObject->rotation.y;

	// abs diff
	diff = diff >= 0 ? diff : -diff;

	if (diff <= epsilon)
	{
		gameObject->rotation.y = m_rotationTarget;
		m_newRotationTarget = false;
		return true;
	}

	// rotate towards desired rotation

	float scaledRotation = m_rotationSpeed * deltaTime;

	// check for overshoot
	if (scaledRotation >= diff)
	{
		gameObject->rotation.y = m_rotationTarget;
		m_newRotationTarget = false;
		return true;
	}
	else
	{
		// rotate ccw (?)
		if (m_rotationTarget > gameObject->rotation.y) gameObject->rotation.y += scaledRotation;
		// rotate cw (?)
		else gameObject->rotation.y -= scaledRotation;
	}

	return false;
}
