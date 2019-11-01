#pragma once

#include <vector>
#include "../../blue-tiles-engine/engine/behaviours/Behaviour.h"

struct SimpleGuardMovementAction;

class SimpleGuardMovementAIBehaviour : public Behaviour
{
public:

	// Base constructor for a guard that takes in it's movement speed and rotation.
	// Action list will be empty.
	SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed);

	// Constructor for a guard that takes in it's movement speed, rotation and a list of actions to add.
	SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed, std::vector<SimpleGuardMovementAction> actionsToAdd);

	// Constructor for a guard that takes in it's movement speed, rotation and an array of actions to add.
	SimpleGuardMovementAIBehaviour(float movementSpeed, float rotationSpeed, SimpleGuardMovementAction* actionsToAdd, size_t count);

	// Adds an action to the list of actions.
	void AddAction(SimpleGuardMovementAction actionToAdd);

	// Adds all of the actions from the list of actions to add.
	void AddActions(std::vector<SimpleGuardMovementAction> actionsToAdd);

	// Adds all of the actions from the array of actions to add.
	void AddActions(SimpleGuardMovementAction* actionsToAdd, size_t count);

	// Add a wait action.
	void AddWaitAction(float duration);

	// Add a move action to a certain x and z position.
	void AddMoveAction(float x, float z);

	// Add a move action to a certain tile (9 units).
	void AddMoveTileAction(int x, int z);

	// Add a turn action.
	// True = Turn counter clock wise 90.
	// False = Turn clock wise 90.
	void AddTurnAction(bool rotateCCW);

	// Add a clock wise turn action.
	void AddTurnCWAction();

	// Add a counter clock wise turn action.
	void AddTurnCCWAction();

	// Overwritten functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

protected:

	// Epsilon for comparing two floats. (The close enough threshold)
	static float epsilon;

	// Processes an action.
	bool ProcessAction(float deltaTime, SimpleGuardMovementAction& action);

	// Processes the given action as a move action.
	bool ProcessMoveAction(float deltaTime, SimpleGuardMovementAction& action);

	// Processes the given action as a turn action.
	bool ProcessTurnAction(float deltaTime, SimpleGuardMovementAction& action);

	// Max movement speed.
	float m_movementSpeed;

	// Max rotation speed.
	float m_rotationSpeed;

	// Tracks time for actions that require it in seconds.
	float m_actionTime = 0;

	// Is the action processing paused?
	bool m_isPaused = false;

	// The transform rotation y target.
	float m_rotationTarget;

	// If there is currently a new rotation y target in m_rotationTarget.
	bool m_newRotationTarget;

	// Index of the current action.
	size_t m_actionIndex = 0;

	// Vector/List of actions to run for this guard's patrol.
	std::vector<SimpleGuardMovementAction> m_actions;
};

// Enum types of actions.
enum SGMAType { Wait, Move, Turn, MoveAndTurn };

// Used for storing data about any action.
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
