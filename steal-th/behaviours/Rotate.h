#pragma once
#include <engine/behaviours/Behaviour.h>
#include <glm/glm.hpp>

class Rotate : public Behaviour
{
public:
	Rotate(glm::vec3 velocity);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

private:
	// The rotation speed and axis of rotation combined.
	glm::vec3 m_velocity;
};