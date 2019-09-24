#pragma once

#include <sdl2/SDL.h>
#include <glm/glm.hpp>

#include <array>
#include <vector>

class Input
{
public:
	enum InputType
	{
		INPUT_CANCEL,
		INPUT_UP,
		INPUT_DOWN,
		INPUT_LEFT,
		INPUT_RIGHT,
		NUM_INPUTS
	};

	Input();

	// Poll input events and update their states.
	void HandleInput(const SDL_Event &inputEvent);

	// Check if a key is being held down.
	bool IsKeyDown(InputType key) const;

	// Check if a mouse button is being held down.
	// Ex: SDL_BUTTON_LEFT.
	bool IsMouseButtonDown(int button) const;

	// Get the mouse's current position.
	glm::ivec2 GetMousePos() const;

private:
	// Hold input states for each key.
	const static int NUM_SCANCODES{ 322 };
	std::array<bool, NUM_SCANCODES> m_keyStates;

	// Hold key mappings for all input types.
	std::vector<SDL_Keycode> m_keyMappings;

	// Hold input states for each mouse button.
	const static int NUM_MOUSEBUTTONS{ 3 };
	std::array<bool, NUM_MOUSEBUTTONS> m_mouseStates;

	// Hold the mouse's current position.
	glm::ivec2 m_mousePos{ 0 };
};