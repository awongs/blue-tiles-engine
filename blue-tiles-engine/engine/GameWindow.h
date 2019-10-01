#pragma once

class GameWindow
{
public:

	// Creates a window with the given width and height
	GameWindow(int width, int height);

	~GameWindow();

private:

	// Sets up SDL window stuff 
	void SetupSDLWindow();

	// Pointer to a sdl window
	SDL_Window* m_window;

	// window width
	int m_windowWidth;

	// window height
	int m_windowHeight;
};