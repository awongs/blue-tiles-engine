#pragma once

#include <glm/vec3.hpp>

#include <engine/behaviours/Behaviour.h>

class ElectricFloor : public Behaviour
{
public:

	ElectricFloor(glm::vec3 originalPos, bool isActive);

	void UpdateActiveState();

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

private:

	glm::vec3 m_originalPos;

	glm::vec3 m_disabledPos;

	bool m_isActive;

};