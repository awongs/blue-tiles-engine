#include "GameEngine.h"

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

	DebugLog::Info("Engine initialization completed!");
}

GameEngine::~GameEngine()
{
	delete renderer;
}

void GameEngine::Update()
{
}

void GameEngine::Draw()
{
	renderer->Render();

	renderer->Display();

	// swap buffer
	SDL_GL_SwapWindow(m_window);
}
