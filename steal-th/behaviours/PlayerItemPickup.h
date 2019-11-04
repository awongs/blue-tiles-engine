#pragma once

#include <engine/behaviours/Behaviour.h>

class PlayerItemPickup : public Behaviour
{
public:

	PlayerItemPickup();

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;
private:

};