#include "TextBehaviour.h"
#include "../GameObject.h"
#include "../graphics/TextRenderer.h"

TextBehaviour::TextBehaviour(std::string text, float scale, glm::vec3 colour)
	: Behaviour(BehaviourType::TextBehaviour)
	, m_text(text)
	, m_scale(scale)
	, m_colour(colour)
{
}

TextBehaviour::TextBehaviour(std::string text, float scale, float r, float g, float b)
	: Behaviour(BehaviourType::TextBehaviour)
	, m_text(text)
	, m_scale(scale)
{
	m_colour = glm::vec3(r, g, b);
}

void TextBehaviour::DrawTextWithRenderer(TextRenderer* textRenderer)
{
	textRenderer->RenderText(m_text, gameObject->position, m_scale, m_colour);
}

void TextBehaviour::Update(float deltaTime)
{
}

void TextBehaviour::Draw(Shader& shader)
{
}

bool TextBehaviour::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}

void TextBehaviour::OnCollisionStay(GLuint other)
{
}
