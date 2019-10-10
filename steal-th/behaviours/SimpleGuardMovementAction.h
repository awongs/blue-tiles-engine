#pragma once

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

	// The desired y rotation value.
	float rotationY;

};