#pragma once
#include <engine/behaviours/Behaviour.h>

class PlayerMovement : public Behaviour
{
public:
	// Constructor.
	PlayerMovement(float speed);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

private:

	// Movement speed in OpengL units per second.
	float m_speed;
};