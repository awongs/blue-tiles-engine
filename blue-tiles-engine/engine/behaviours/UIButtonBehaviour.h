#pragma once

#include <string>
#include <memory>
#include <functional>
#include "Behaviour.h"
#include "../graphics/Texture.h"

class UIButtonBehaviour : public Behaviour
{
public:
	// Default constructor
	UIButtonBehaviour(std::string text, std::function<void()> lambda);

	// Default constructor
	UIButtonBehaviour(std::string text, std::function<void()> lambda, ImVec2 size);
	
	// Destructor
	virtual ~UIButtonBehaviour();

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

	// The size of the button
	ImVec2 m_size;

	std::function<void()> m_lambda;
};

