#pragma once

#include <memory>
#include <sdl2/SDL.h>

#include "physics/PhysicsEngine.h"

class TextRenderer;
class Renderer;
class Scene;
class Input;

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

	// Handle user input.
	void HandleInput(Input *input, SDL_Event windowEvent);

	// Main game update loop
	void Update();

	// Main game draw loop
	void Draw();

	// Renderer thing
	Renderer* renderer;

	// Setter for the current scene.
	void SetScene(Scene* scene);

	// Text Renderering thing
	TextRenderer* textRenderer;

	// Get the raw pointer to the physics engine, so that the game engine 
	// retains ownership of it.
	// Return nullptr if it somehow doesn't already exist.
	PhysicsEngine *GetPhysicsEngine() const;

private:
	// The game's physics engine.
	std::unique_ptr<PhysicsEngine> m_physEngine{ nullptr };

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