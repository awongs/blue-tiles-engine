#include <imgui/imgui.h>
#include "UIButtonBehaviour.h"

UIButtonBehaviour::UIButtonBehaviour(std::string text, std::function<void()> lambda)
	: Behaviour(BehaviourType::UIButtonBehaviour)
	, m_text(text)
	, m_lambda(lambda)
	, m_size(ImVec2(0, 0))
{

}

UIButtonBehaviour::UIButtonBehaviour(std::string text, std::function<void()> lambda, ImVec2 size)
	: Behaviour(BehaviourType::UIButtonBehaviour)
	, m_text(text)
	, m_lambda(lambda)
	, m_size(size)
{

}

UIButtonBehaviour::~UIButtonBehaviour()
{

}

std::string UIButtonBehaviour::GetText()
{
	return m_text;
}

void UIButtonBehaviour::SetText(std::string text)
{
	m_text = text;
}

void UIButtonBehaviour::Update(float deltaTime)
{

}

void UIButtonBehaviour::Draw(Shader& shader)
{
	if (ImGui::Button(m_text.c_str(), m_size))
		m_lambda();
}

bool UIButtonBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void UIButtonBehaviour::OnCollisionStay(GLuint other)
{

}