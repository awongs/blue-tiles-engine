#pragma once

#include <string>
#include <memory>
#include "Behaviour.h"
#include "../graphics/Texture.h"

class UITextBehaviour : public Behaviour
{
public:
	// Default constructor
	UITextBehaviour(std::string text);
	
	// Destructor
	virtual ~UITextBehaviour();

	// Getter for display text
	std::string GetText();

	// Setter for display text
	void SetText(std::string text);

	// Overridden functions
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;
private:
	// The string to display
	std::string m_text;
};

