#include <imgui/imgui.h>
#include "UITextBehaviour.h"

UITextBehaviour::UITextBehaviour(std::string text)
	: Behaviour(BehaviourType::UITextBehaviour)
	, m_text(text)
{

}

UITextBehaviour::~UITextBehaviour()
{

}

std::string UITextBehaviour::GetText()
{
	return m_text;
}

void UITextBehaviour::SetText(std::string text)
{
	m_text = text;
}

void UITextBehaviour::Update(float deltaTime)
{

}

void UITextBehaviour::Draw(Shader& shader)
{
	ImGui::Text(m_text.c_str());
}

bool UITextBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void UITextBehaviour::OnCollisionStay(GLuint other)
{

}