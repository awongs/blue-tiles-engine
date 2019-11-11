#pragma once

#include <imgui/imgui.h>
#include "Behaviour.h"

class UIMenuBehaviour : public Behaviour
{
public:
	// Default constructor
	UIMenuBehaviour(std::string title);
	// Constructor with position
	UIMenuBehaviour(std::string title, ImVec2 position);
	// Constructor with position and size
	UIMenuBehaviour(std::string title, ImVec2 position, ImVec2 size);
	// Constructor with position, size, and imgui window flags
	UIMenuBehaviour(std::string title, ImVec2 position, ImVec2 size, int flags);
	
	// Destructor
	virtual ~UIMenuBehaviour();

	// Getter for window position
	ImVec2 GetPosition();

	// Getter for window size
	ImVec2 GetSize();

	// Getter for imgui window flags
	int GetWindowFlags();

	// Setter for window position
	void SetPosition(ImVec2 position);

	// Setter for window size
	void SetSize(ImVec2 size);

	// Setter for imgui window flags
	void SetWindowFlags(int flags);

	// Begin ImGui drawing
	void BeginDraw();

	// End ImGui drawing
	void EndDraw();

	// Overridden functions
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;
private:
	// Title of the menu
	std::string m_title;

	// Position of the menu
	ImVec2 m_position;
	// Size of the menu
	ImVec2 m_size;
	// ImGuiWindowFlags
	int m_windowFlags;
};

