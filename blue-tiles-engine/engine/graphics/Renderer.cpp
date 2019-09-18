#include "Renderer.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...");


	DebugLog::Info("Renderer initialization completed!");
}

Renderer::~Renderer()
{
	// Cleanup context
	SDL_GL_DeleteContext(context);
}

void Renderer::Render()
{

	
	
	


}

void Renderer::Display()
{
}
