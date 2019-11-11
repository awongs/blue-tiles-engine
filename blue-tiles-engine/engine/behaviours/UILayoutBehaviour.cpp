#include "UILayoutBehaviour.h"
#include <imgui/imgui.h>

UILayoutBehaviour::UILayoutBehaviour(bool vertical)
	: Behaviour(BehaviourType::UILayoutBehaviour)
	, m_isVertical(vertical)
{

}

UILayoutBehaviour::~UILayoutBehaviour()
{

}

bool UILayoutBehaviour::IsVertical()
{
	return m_isVertical;
}

void UILayoutBehaviour::SetVertical(bool vertical)
{
	m_isVertical = vertical;
}

void UILayoutBehaviour::Update(float deltaTime)
{

}

void UILayoutBehaviour::Draw(Shader& shader)
{
	ImGui::SameLine();
}

bool UILayoutBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void UILayoutBehaviour::OnCollisionStay(GLuint other)
{

}