#pragma once

#include "Behaviour.h"

#include <functional>

class PhysicsEngine;
class Collider;

class PhysicsBehaviour : public Behaviour
{
public:
	PhysicsBehaviour(PhysicsEngine *physEngine,
		GLuint gameObjectId, Collider *collider,
		std::function<void(GLuint)> onCollision);

	PhysicsBehaviour(PhysicsEngine* physEngine, GLuint gameObjectId, Collider* collider);

	~PhysicsBehaviour();

	// Update
	virtual void Update(float deltaTime);

	// Draw
	virtual void Draw(Shader &shader);

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string& message);

	void OnCollisionStay(GLuint other) override;

	// Get its PhysicsObject's collider.
	Collider *GetCollider() const;

private:
	// Hold a raw pointer to the GameEngine's PhysicsEngine.
	// This behaviour will need to register a PhysicsObject for itself.
	PhysicsEngine *m_physEngine{ nullptr };

	// Hold a pointer to its corresponding PhysicsObject's collider.
	// This is needed to update the collider's position based on the
	// GameObject's current position.
	Collider *m_collider{ nullptr };

	// Handle collisions specific to this component.
	// This allows each component to define its own way of handling collisions.
	std::function<void(GLuint)> m_onCollision;
};