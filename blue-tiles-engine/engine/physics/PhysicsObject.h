#pragma once

#include <glad/glad.h>

#include <memory>

#include "Collider.h"

class PhysicsObject
{
public:
	PhysicsObject();

	~PhysicsObject();
	
	// Get the physics object's collider.
	Collider *GetCollider() const;
	void SetCollider(Collider *collider);
	GLuint GetGameObjectId() const;
	void SetGameObjectId(GLuint id);

private:
	// This physics component's collider.
	std::unique_ptr<Collider> m_collider{ nullptr };

	// Hold the id of the GameObject that corresponds to this PhysicsObject.
	GLuint m_gameObjectId;
};