#pragma once

#include <vector>
#include "../../blue-tiles-engine/engine/behaviours/Behaviour.h"

struct SimpleGuardMovementAction;

class SimpleGuardMovementAIBehaviour : public Behaviour
{
public:

	SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed);

	SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed, std::vector<SimpleGuardMovementAction> actionsToAdd);

	SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed, SimpleGuardMovementAction* actionsToAdd, size_t count);

	void AddAction(SimpleGuardMovementAction actionToAdd);

	void AddActions(std::vector<SimpleGuardMovementAction> actionsToAdd);

	void AddActions(SimpleGuardMovementAction* actionsToAdd, size_t count);

	// Overwritten functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

protected:

	static float closeFloat;

	bool ProcessAction(float deltaTime, SimpleGuardMovementAction& action);

	bool ProcessMoveAction(SimpleGuardMovementAction& action);

	bool ProcessTurnAction(SimpleGuardMovementAction& action);

	float m_movementSpeed;

	float m_rotationSpeed;

	float m_actionTime = 0;

	bool m_isPaused = false;

	size_t m_actionIndex = 0;

	std::vector<SimpleGuardMovementAction> m_actions;
};

float SimpleGuardMovementAIBehaviour::closeFloat = 0.05f;