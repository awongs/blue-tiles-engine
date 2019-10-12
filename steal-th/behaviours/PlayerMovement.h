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

	glm::vec3 GetCurrentVelocity() const;

private:

	// Movement speed in OpengL units per second.
	float m_speed;

	// The player's velocity for this current frame.
	// This will be influenced by m_speed.
	glm::vec3 m_currentVelocity;
};