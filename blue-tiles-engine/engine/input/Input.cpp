#include "Input.h"

Input::Input()
{
	m_keyStates.fill(false);
	m_mouseStates.fill(false);
	m_keyMappings.resize(NUM_INPUTS);

	// Initialize key mappings.
	m_keyMappings[INPUT_CANCEL] = SDL_SCANCODE_ESCAPE;
	m_keyMappings[INPUT_UP] = SDL_SCANCODE_W;
	m_keyMappings[INPUT_DOWN] = SDL_SCANCODE_S;
	m_keyMappings[INPUT_LEFT] = SDL_SCANCODE_A;
	m_keyMappings[INPUT_RIGHT] = SDL_SCANCODE_D;
}

Input& Input::GetInstance()
{
	static Input input;
	return input;
}

void Input::HandleInput(const SDL_Event &inputEvent)
{
	switch (inputEvent.type)
	{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			SDL_Scancode key{ inputEvent.key.keysym.scancode };
			m_keyStates[key] = (inputEvent.type == SDL_KEYDOWN);
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			// Restrict the number of mouse buttons.
			int button{ inputEvent.button.button };
			if (button > NUM_MOUSEBUTTONS)
				break;

			m_mouseStates[button - 1] = (inputEvent.type == SDL_MOUSEBUTTONDOWN);
			break;
		}

		case SDL_MOUSEMOTION:
		{
			m_mousePos.x = inputEvent.motion.x;
			m_mousePos.y = inputEvent.motion.y;
			break;
		}
	}
}

bool Input::IsKeyDown(InputType key) const
{
	return m_keyStates[m_keyMappings[key]];
}

bool Input::IsMouseButtonDown(int button) const
{
	// Restrict the number of mouse buttons.
	button = glm::clamp(button, 1, NUM_MOUSEBUTTONS) - 1;
	return m_mouseStates[button];
}

glm::ivec2 Input::GetMousePos() const
{
	return m_mousePos;
}