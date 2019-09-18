#include "GameEngine.h"

GameEngine::GameEngine(SDL_Window* targetWindow)
{
	DebugLog::Info("Engine initialization starting...");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GLContext targetContext = SDL_GL_CreateContext(targetWindow);
	DebugLog::Info("Created context");

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
