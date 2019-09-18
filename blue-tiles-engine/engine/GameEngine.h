#pragma once

#include "debugbt/DebugLog.h"

/*
	The main game engine class
*/
class GameEngine
{

public:

	/*
		Game Engine constructor that ask for anything it needs for it to function properly
	*/
	GameEngine();

	/*
		Destroys and cleans up the engine.
	*/
	~GameEngine();

	/*
		Main game update loop
	*/
	void Update();

	/*
		Main game draw loop
	*/
	void Draw();

private:

};