#pragma once

#include <engine/behaviours/Behaviour.h>
#include <glm/glm.hpp>

class LevelScene;

class GuardDetection : public Behaviour
{

public:
	// Constructor.
	GuardDetection(LevelScene *levelScene, GameObject *playerObj);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader &shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;

private:
	bool m_playerDetected;

	// Hold a raw pointer to the level scene.
	// We need this to get tile data.
	LevelScene *m_levelScene{ nullptr };

	// Hold a raw pointer to the player GameObject.
	GameObject *m_playerObj{ nullptr };
};