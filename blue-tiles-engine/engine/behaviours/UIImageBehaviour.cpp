#include <imgui/imgui.h>
#include <sstream>
#include "UIImageBehaviour.h"
#include "../../util/FileManager.h"
#include "..//debugbt/DebugLog.h"

UIImageBehaviour::UIImageBehaviour(std::string filePath)
	: Behaviour(BehaviourType::UIImageBehaviour)
{
	m_texture = filemanager::LoadTexture(filePath);
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
	std::ostringstream oss;
	oss << m_texture->GetTextureID();
	DebugLog::Info(oss.str());
	ImGui::Image((void*)(intptr_t)m_texture->GetTextureID(), m_size);
}

bool UIImageBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void UIImageBehaviour::OnCollisionStay(GLuint other)
{

}