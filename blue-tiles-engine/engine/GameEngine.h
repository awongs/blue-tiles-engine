#pragma once

#include <memory>
#include <sdl2/SDL.h>

class Renderer;
class Scene;

/*
	The main game engine class
*/
class GameEngine
{

public:

	// Game Engine constructor that ask for anything it needs for it to function properly
	GameEngine(SDL_Window* targetWindow);

	// Destroys and cleans up the engine.
	~GameEngine();

	// Main game update loop
	void Update();

	// Main game draw loop
	void Draw();

	// Renderer thing
	Renderer* renderer;

private:
	// The current scene.
	std::unique_ptr<Scene> m_currentScene;

	// Updates the FPS counter
	void UpdateFPSCounter();

	// Pointer to a window
	SDL_Window* m_window;

	// Delta time between frames in seconds
	float m_deltaTime;

	// Milliseconds of the frame since the application was started
	Uint32 m_lastFrameTime;

	// Counter in milliseconds for updating the frame rate
	Uint32 m_frameRateUpdateCounter;

	// Counts the number of frames 
	int m_frameCounter;

	// Number of frames per second
	int m_frameRate;
};