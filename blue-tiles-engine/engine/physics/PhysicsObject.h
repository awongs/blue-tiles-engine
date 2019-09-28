#pragma once

#include <glad/glad.h>

#include <functional>
#include <memory>

#include "Collider.h"

class PhysicsObject
{
public:
	PhysicsObject(GLuint gameObjectId, Collider *collider, 
		std::function<void(const PhysicsObject &)> onCollision);

	~PhysicsObject();

	// The physics engine calls this when a collision is detected.
	void OnCollision(const PhysicsObject &other);

	// Get the game object's id.
	GLuint GetId() const;

private:
	// The id of the game object that this physics component belongs to.
	GLuint m_gameObjectId;

	// This physics component's collider.
	std::unique_ptr<Collider> m_collider{ nullptr };

	// Handle collisions specific to this component.
	// This allows each component to define its own way of handling collisions.
	std::function<void(const PhysicsObject &)> m_onCollision;
};