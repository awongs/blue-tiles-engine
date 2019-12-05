#pragma once

#include <glm/vec3.hpp>

#include <engine/behaviours/Behaviour.h>

class ElectricSwitch : public Behaviour
{
public:

	ElectricSwitch(glm::vec3 originalPos);

	void ActivateSwitch();

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

private:

	glm::vec3 m_originalPos;

	glm::vec3 m_activationPos;

	bool m_startAnimation;

	float m_time;

	bool m_touching;

	bool m_stillTouching;

};