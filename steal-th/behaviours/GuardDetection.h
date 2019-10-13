#pragma once

#include <engine/behaviours/Behaviour.h>
#include <glm/glm.hpp>

class GuardDetection : public Behaviour {

public:
    // Constructor.
    GuardDetection();

    // Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

private:
    bool m_playerDetected;
};