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

	void AddWaitAction(float duration);

	void AddMoveAction(float x, float z);

	void AddMoveTileAction(int x, int z);

	void AddTurnAction(bool rotateCCW);

	void AddTurnCWAction();

	void AddTurnCCWAction();

	// Overwritten functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

protected:

	static float epsilon;

	static float epsilonSqr;

	bool ProcessAction(float deltaTime, SimpleGuardMovementAction& action);

	bool ProcessMoveAction(float deltaTime, SimpleGuardMovementAction& action);

	bool ProcessTurnAction(float deltaTime, SimpleGuardMovementAction& action);

	float m_movementSpeed;

	float m_rotationSpeed;

	float m_actionTime = 0;

	bool m_isPaused = false;

	float m_rotationTarget;

	bool m_newRotationTarget;

	size_t m_actionIndex = 0;

	std::vector<SimpleGuardMovementAction> m_actions;
};

enum SGMAType { Wait, Move, Turn, MoveAndTurn };

struct SimpleGuardMovementAction
{
public:

	//
	// -- Parameters used by all actions --
	//

	// The type of action
	SGMAType actionType;

	//
	// -- Parameters used by Wait actions --
	//

	// The duration of the action.
	float waitDuration;

	//
	// -- Parameters used by Move actions --
	//

	// Destination to move to. (X component)
	float destinationX;

	// Destination to move to. (Z component)
	float destinationZ;

	//
	// -- Parameters used by Turn actions -- 
	//

	// Rotate CW or CCW.
	bool rotateCCW;
};
