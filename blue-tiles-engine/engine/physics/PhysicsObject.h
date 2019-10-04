#pragma once

#include <glad/glad.h>

#include <functional>
#include <memory>

#include "../behaviours/Behaviour.h"
#include "Collider.h"

class PhysicsObject : public Behaviour
{
public:
	PhysicsObject(GLuint gameObjectId, Collider *collider, 
		std::function<void(const PhysicsObject &)> onCollision);

	~PhysicsObject();

	// Update
	virtual void Update(float deltaTime);

	// Draw
	virtual void Draw();

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string message);

	// The physics engine calls this when a collision is detected.
	void OnCollision(const PhysicsObject &other);

	// Get the game object's id.
	GLuint GetId() const;
	
	// Get the physics object's collider.
	Collider *GetCollider() const;

private:
	// This physics component's collider.
	std::unique_ptr<Collider> m_collider{ nullptr };

	// Handle collisions specific to this component.
	// This allows each component to define its own way of handling collisions.
	std::function<void(const PhysicsObject &)> m_onCollision;
};