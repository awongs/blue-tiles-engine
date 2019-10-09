#include "TextBehaviour.h"
#include "Behaviour.h"
#include "../GameObject.h"
#include "../graphics/TextRenderer.h"

TextBehaviour::TextBehaviour(GLuint gameObjectId, std::string text, float scale, glm::vec3 colour)
	: Behaviour(gameObjectId, BehaviourType::TextBehaviour)
	, m_text(text)
	, m_scale(scale)
	, m_colour(colour)
{
}

void TextBehaviour::DrawTextWithRenderer(TextRenderer* textRenderer)
{
	textRenderer->RenderText(m_text, gameObject->position, m_scale, m_colour);
}

void TextBehaviour::Update(float deltaTime)
{
}

void TextBehaviour::Draw()
{
}

bool TextBehaviour::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}
