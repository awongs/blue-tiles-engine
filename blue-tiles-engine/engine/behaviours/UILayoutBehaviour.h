#pragma once

#include <imgui/imgui.h>
#include "Behaviour.h"

class UILayoutBehaviour : public Behaviour
{
public:
	// Default constructor
	UILayoutBehaviour(bool vertical);
	
	// Destructor
	virtual ~UILayoutBehaviour();
	
	// Returns whether this layout is vertical or horizontal
	bool IsVertical();

	void SetVertical(bool vertical);

	// Overridden functions
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;
private:
	// If the contents of this layout are vertical
	bool m_isVertical;
};

