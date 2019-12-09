#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include <sdl2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
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

	// Event handler for events like window management and input
	void HandleEvent(SDL_Event& _event);

	// Main game update loop
	void Update();

	// Main game draw loop
	void Draw();

	// Renderer thing
	Renderer* renderer;

	// Adds a scene to the cached scenes
	void AddScene(std::string sceneName, std::shared_ptr<Scene> scene, std::function<void()> lambda);

	// Setter for the current scene.
	void SetScene(std::string sceneName, bool reload = true);

	// Text Renderering thing
	TextRenderer* textRenderer;

	// Get the raw pointer to the physics engine, so that the game engine 
	// retains ownership of it.
	// Return nullptr if it somehow doesn't already exist.
	PhysicsEngine *GetPhysicsEngine() const;

private:
	// The game's physics engine.
	std::unique_ptr<PhysicsEngine> m_physEngine{ nullptr };

	// Cached scenes
	std::unordered_map<std::string, std::shared_ptr<Scene>> m_cachedScenes;
	std::unordered_map<std::string, std::function<void()>> m_cachedScenesLoading;
	
	// The current scene.
	std::shared_ptr<Scene> m_currentScene;

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

	// IO reference for ImGui
	ImGuiIO m_io;
};