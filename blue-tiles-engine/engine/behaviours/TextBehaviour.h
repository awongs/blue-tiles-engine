#pragma once

#include <glm/vec3.hpp>
#include <string>
#include "Behaviour.h"

class TextRenderer;

class TextBehaviour : public Behaviour
{
public:

	TextBehaviour(GLuint gameObjectId, std::string text, float scale, glm::vec3 colour);

	// Called by text renderer
	void DrawTextWithRenderer(TextRenderer* textRenderer);

	// Overwritten functions.
	void Update(float deltaTime) override;
	void Draw() override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

private:

	// The string to display on screen.
	std::string m_text;

	// The scale of the text.
	float m_scale;

	// The colour of the text.
	glm::vec3 m_colour;
};