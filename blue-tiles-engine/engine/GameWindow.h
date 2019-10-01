#pragma once

struct SDL_Window;

class GameWindow
{
public:

	// Creates a window with the given width and height
	GameWindow(int width, int height);

	~GameWindow();

	// Sets up SDL window stuff 
	int SetupSDLWindow();

	SDL_Window* GetWindow();

private:

	// Pointer to a sdl window
	SDL_Window* m_window;

	// window width
	int m_windowWidth;

	// window height
	int m_windowHeight;
};