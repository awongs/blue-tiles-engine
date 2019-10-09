#include <vector>

#include "GameEngine.h"
#include "graphics/Renderer.h"
#include "debugbt/DebugLog.h"
#include "GameObject.h"
#include "behaviours/MeshRenderer.h"
#include "Scene.h"
#include "physics/PhysicsObject.h"
#include "input/Input.h"
#include "graphics/TextRenderer.h"

GameEngine::GameEngine(SDL_Window* targetWindow)
	: m_window(targetWindow)
{
	DebugLog::Info("Engine initialization starting...");

	// create context
	SDL_GLContext targetContext = SDL_GL_CreateContext(m_window);
	if (targetContext == NULL)
	{
		DebugLog::Error("Failed to create context");
	}
	else
	{
		DebugLog::Info("Created context");
	}

	// load OpenGL functions glad SDL
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	GLint vpWidth, vpHeight;
	SDL_GL_GetDrawableSize(m_window, &vpWidth, &vpHeight);
	glViewport(0, 0, vpWidth, vpHeight);

	// text rendering (NOTE: must be created before any buffers!)
	textRenderer = new TextRenderer(800, 600);

	// 3d rendering
	renderer = new Renderer(&targetContext);

	m_lastFrameTime = SDL_GetTicks();

	// Instantiate the physics engine.
	m_physEngine = std::make_unique<PhysicsEngine>();

	DebugLog::Info("Engine initialization completed!");
}

GameEngine::~GameEngine()
{
	delete renderer;
}

void GameEngine::HandleInput(Input *input, SDL_Event windowEvent)
{
	// Update the inputs.
	if (input == nullptr) return;

	input->HandleInput(windowEvent);

	/*
	const std::unique_ptr<GameObject> &obj = m_currentScene->getWorldGameObjectByIndex(0);
	PhysicsObject *physObj = static_cast<PhysicsObject *>(obj->GetBehaviour(BehaviourType::PhysicsObject));

	// TODO: Input and collision testing. Remove this later.
	if (input->IsKeyDown(Input::INPUT_UP))
	{
		//DebugLog::Info("Up key pressed.");
		obj->position.y += 0.2f;
	}
	if (input->IsKeyDown(Input::INPUT_DOWN))
	{
		//DebugLog::Info("Down key pressed.");
		obj->position.y -= 0.2f;
	}
	if (input->IsKeyDown(Input::INPUT_LEFT))
	{
		//DebugLog::Info("Left key pressed.");
		obj->position.x -= 0.2f;
	}
	if (input->IsKeyDown(Input::INPUT_RIGHT))
	{
		//DebugLog::Info("Right key pressed.");
		obj->position.x += 0.2f;
	}
	if (input->IsMouseButtonDown(SDL_BUTTON_LEFT))
		DebugLog::Info("Left mouse button pressed.");
	if (input->IsMouseButtonDown(SDL_BUTTON_RIGHT))
		DebugLog::Info("Right mouse button pressed.");
	if (input->IsMouseButtonDown(SDL_BUTTON_MIDDLE))
		DebugLog::Info("Middle mouse button pressed.");

	physObj->GetCollider()->SetPosition(obj->position);
	*/
}

void GameEngine::Update()
{
	UpdateFPSCounter();

	m_currentScene->Update(m_deltaTime);

	m_physEngine->Update();
}

void GameEngine::Draw()
{
	renderer->Render(*m_currentScene);

	renderer->Display(*m_currentScene);

	textRenderer->RenderScreenText(*m_currentScene);

	// swap buffer
	SDL_GL_SwapWindow(m_window);
}

void GameEngine::SetScene(Scene* scene)
{
	m_currentScene = std::unique_ptr<Scene>(scene);
}

void GameEngine::UpdateFPSCounter()
{
	// delta time
	Uint32 deltaMS = SDL_GetTicks() - m_lastFrameTime;
	m_deltaTime = (float)deltaMS / 1000.0f;

	// update time tracker/counters
	m_lastFrameTime = SDL_GetTicks();
	m_frameRateUpdateCounter += deltaMS;
	m_frameCounter++;

	// update frame rate every couple millisec
	if (m_frameRateUpdateCounter > 1000)
	{
		// calculate frame rate
		m_frameRate = (int)((float)m_frameCounter * ((float)m_frameRateUpdateCounter / 1000.0f));
		m_frameRateUpdateCounter = 0;
		m_frameCounter = 0;

		// update window title with frame rate
		string newWindowTitle = "Blue Tiles Engine [FPS:" + std::to_string(m_frameRate) + "]";
		SDL_SetWindowTitle(m_window, newWindowTitle.c_str());
	}
}
