#pragma once

#include <sdl2/SDL.h>

#include "debugbt/DebugLog.h"
#include "graphics/Renderer.h"

/*
	The main game engine class
*/
class GameEngine
{

public:

	/*
		Game Engine constructor that ask for anything it needs for it to function properly
	*/
	GameEngine(SDL_Window* targetWindow);

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

	Renderer* renderer;

private:

	SDL_GLContext targetContext;

	SDL_Window* window;

};