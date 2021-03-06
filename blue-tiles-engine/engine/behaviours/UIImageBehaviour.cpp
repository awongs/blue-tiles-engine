#include <imgui/imgui.h>
#include "UIImageBehaviour.h"
#include "../..//util/FileManager.h"

UIImageBehaviour::UIImageBehaviour(std::string filePath)
	: Behaviour(BehaviourType::UIImageBehaviour)
	, m_size(100, 100)
{
	m_texture = FileManager::LoadTexture(filePath);
}

UIImageBehaviour::UIImageBehaviour(std::string filePath, ImVec2 size)
	: Behaviour(BehaviourType::UIImageBehaviour)
	, m_size(size)
{
	m_texture = FileManager::LoadTexture(filePath);
}

UIImageBehaviour::~UIImageBehaviour()
{

}

ImVec2 UIImageBehaviour::GetSize()
{
	return m_size;
}

void UIImageBehaviour::SetSize(ImVec2 size)
{
	m_size = size;
}

void UIImageBehaviour::Update(float deltaTime)
{

}

void UIImageBehaviour::Draw(Shader& shader)
{
	ImGui::Image((void*)(intptr_t)m_texture->GetTextureID(), m_size);
}

bool UIImageBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void UIImageBehaviour::OnCollisionStay(GLuint other)
{

}