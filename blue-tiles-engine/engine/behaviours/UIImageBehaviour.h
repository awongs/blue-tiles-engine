#pragma once

#include <string>
#include <memory>
#include <imgui/imgui.h>
#include "Behaviour.h"
#include "../graphics/Texture.h"

class UIImageBehaviour : public Behaviour
{
public:
	// Default constructor
	UIImageBehaviour(std::string filePath);
	
	// Destructor
	virtual ~UIImageBehaviour();

	// Getter for window size
	ImVec2 GetSize();

	// Setter for window size
	void SetSize(ImVec2 size);

	// Overridden functions
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;
private:
	// Texture containing the image
	std::shared_ptr<Texture> m_texture;

	// Size of the image
	ImVec2 m_size;
};

