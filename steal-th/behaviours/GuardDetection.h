#pragma once

#include <engine/behaviours/Behaviour.h>
#include <engine/behaviours/SpotLight.h>
#include <glm/glm.hpp>
#include <engine/opencl/OpenCLManager.h>

class LevelScene;

class GuardDetection : public Behaviour
{

public:
	// Constructor.
	GuardDetection(LevelScene *levelScene, GameObject *playerObj, 
		float maxViewDist, int tileViewRadius);

	// Destructor.
	~GuardDetection();

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader &shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;
	
private:
	// Output the end points of each detection ray into
	// a single vector.
	void GetDetectionRayEndPoints(std::vector<float> &endpointsX, 
		std::vector<float>& endpointsZ) const;

	// Try to detect the player by using the line algorithm with
	// a start point and the point at the maximum view distance 
	// from the guard, in world coordinates.
	// Returns true if player was detected, otherwise returns false.
	bool TryDetectPlayer(glm::vec2 startPoint, glm::vec2 maxDistancePoint);

	bool m_isPlayerDetected{ false };

	// Defines how far and wide a guard can detect players.
	float m_maxViewDist{ 0 };
	int m_tileViewRadius{ 0 };

	// Hold a raw pointer to the level scene.
	// We need this to get tile data.
	LevelScene *m_levelScene{ nullptr };

	// Hold a raw pointer to the player GameObject.
	GameObject *m_playerObj{ nullptr };

	// A spot light for the guard's detection cone.
	std::weak_ptr<SpotLight> m_detectionCone;

	// Store the number of detection rays for this guard.
	// This is a fixed number, calculated from the guard's view radius.
	int m_numDetectionRays{ 0 };

	// Store OpenCL-related values.
	std::unique_ptr<OpenCLManager> m_openCLManager;
	bool* m_outputBuffer;
	bool m_isCollidingPlayer{ false };
};