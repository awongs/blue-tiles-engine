#pragma once

#include <sdl2\SDL.h>
#include "../debugbt/DebugLog.h"

/*
	This renders the game engine.
*/
class Renderer
{
public:

	Renderer(SDL_GLContext* targetContext);

	~Renderer();

	void Render();

	void Display();

private:

	SDL_GLContext* context;

};