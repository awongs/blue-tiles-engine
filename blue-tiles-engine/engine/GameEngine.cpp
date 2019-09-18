#include "GameEngine.h"

GameEngine::GameEngine(SDL_Window* targetWindow)
{
	DebugLog::Info("Engine initialization starting...");

	// create context
	targetContext = SDL_GL_CreateContext(targetWindow);
	if (targetContext == NULL)
	{
		DebugLog::Error("Failed to create context");
	}
	else
	{
		DebugLog::Info("Created context");
	}

	// Load OpenGL functions glad SDL
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	GLint vpWidth, vpHeight;
	SDL_GL_GetDrawableSize(targetWindow, &vpWidth, &vpHeight);
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
}
