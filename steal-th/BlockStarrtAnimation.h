#pragma once
#include <glm/vec3.hpp>
#include <engine/behaviours/Behaviour.h>

class BlockStartAnimation : Behaviour
{
public:


	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

private:

	bool m_startAnimation = false;

	float m_delay;

	float m_time;

	glm::vec3 basePosition;

	glm::vec3 animationPosition;
};