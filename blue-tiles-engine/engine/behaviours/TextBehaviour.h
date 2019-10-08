#pragma once

#include <glm/vec3.hpp>
#include <string>
#include "Behaviour.h"

class TextRenderer;

class TextBehaviour : public Behaviour
{
public:

	TextBehaviour(GLuint gameObjectId, std::string text, float scale, glm::vec3 colour);

	void DrawTextWithRenderer(TextRenderer* textRenderer);

	// Overwritten functions.
	void Update(float deltaTime) override;
	void Draw() override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

private:

	std::string m_text;

	float m_scale;

	glm::vec3 m_colour;
};