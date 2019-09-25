#include "GameEngine.h"
#include "graphics/Renderer.h"
#include "debugbt/DebugLog.h"

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

	renderer = new Renderer(&targetContext);

	m_lastFrameTime = SDL_GetTicks();

	DebugLog::Info("Engine initialization completed!");
}

GameEngine::~GameEngine()
{
	delete renderer;
}

void GameEngine::Update()
{
	UpdateFPSCounter();
}

void GameEngine::Draw()
{
	renderer->Render();

	renderer->Display();

	// swap buffer
	SDL_GL_SwapWindow(m_window);
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