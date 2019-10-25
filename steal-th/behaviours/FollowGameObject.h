#pragma once

#include <glm/glm.hpp>
#include <engine/behaviours/Behaviour.h>

class FollowGameObject : public Behaviour
{
public:
	// Constructor.
	FollowGameObject(glm::vec3 offset, float lerpSpeed = 0.25f);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;
	void OnCollisionStay(GLuint other) override;

private:
	// Position offset from the follow target.
	glm::vec3 m_offset;

	// How quickly the camera follows the game object.
	float m_lerpSpeed;
};