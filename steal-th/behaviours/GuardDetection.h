#pragma once

#include <engine/behaviours/Behaviour.h>
#include <glm/glm.hpp>

class LevelScene;

class GuardDetection : public Behaviour
{

public:
	// Constructor.
	GuardDetection(LevelScene *levelScene, GameObject *playerObj, 
		float maxViewDist, int tileViewRadius);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader &shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

private:
	// Try to detect the player by using the line algorithm with
	// a start point and the point at the maximum view distance 
	// from the guard, in world coordinates.
	void tryDetectPlayer(glm::vec2 startPoint, glm::vec2 maxDistancePoint);

	bool m_playerDetected;

	// Defines how far and wide a guard can detect players.
	float m_maxViewDist{ 0 };
	int m_tileViewRadius{ 0 };

	// Hold a raw pointer to the level scene.
	// We need this to get tile data.
	LevelScene *m_levelScene{ nullptr };

	// Hold a raw pointer to the player GameObject.
	GameObject *m_playerObj{ nullptr };
};