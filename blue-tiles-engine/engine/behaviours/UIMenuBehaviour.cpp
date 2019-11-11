#include "UIMenuBehaviour.h"
#include <imgui/imgui.h>

UIMenuBehaviour::UIMenuBehaviour(std::string title)
	: Behaviour(BehaviourType::UIMenuBehaviour)
	, m_title(title)
	, m_position(ImVec2(0, 0))
	, m_size(ImVec2(100, 100))
	, m_windowFlags(0)
{

}

UIMenuBehaviour::UIMenuBehaviour(std::string title, ImVec2 position)
	: Behaviour(BehaviourType::UIMenuBehaviour)
	, m_title(title)
	, m_position(position)
	, m_size(ImVec2(100, 100))
	, m_windowFlags(0)
{

}

UIMenuBehaviour::UIMenuBehaviour(std::string title, ImVec2 position, ImVec2 size)
	: Behaviour(BehaviourType::UIMenuBehaviour)
	, m_title(title)
	, m_position(position)
	, m_size(size)
	, m_windowFlags(0)
{

}

UIMenuBehaviour::UIMenuBehaviour(std::string title, ImVec2 position, ImVec2 size, int flags)
	: Behaviour(BehaviourType::UIMenuBehaviour)
	, m_title(title)
	, m_position(position)
	, m_size(size)
	, m_windowFlags(flags)
{

}

UIMenuBehaviour::~UIMenuBehaviour()
{

}

ImVec2 UIMenuBehaviour::GetPosition()
{
	return m_position;
}

ImVec2 UIMenuBehaviour::GetSize()
{
	return m_size;
}

int UIMenuBehaviour::GetWindowFlags()
{
	return m_windowFlags;
}

void UIMenuBehaviour::SetPosition(ImVec2 position)
{
	m_position = position;
}

void UIMenuBehaviour::SetSize(ImVec2 size)
{
	m_size = size;
}

void UIMenuBehaviour::SetWindowFlags(int flags)
{
	m_windowFlags = flags;
}

void UIMenuBehaviour::BeginDraw()
{
	ImGui::SetNextWindowPos(m_position);
	ImGui::SetNextWindowSize(m_size);
	ImGui::Begin(m_title.c_str(), NULL, m_windowFlags);
}

void UIMenuBehaviour::EndDraw()
{
	ImGui::End();
}

void UIMenuBehaviour::Update(float deltaTime)
{

}

void UIMenuBehaviour::Draw(Shader& shader)
{

}

bool UIMenuBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void UIMenuBehaviour::OnCollisionStay(GLuint other)
{

}