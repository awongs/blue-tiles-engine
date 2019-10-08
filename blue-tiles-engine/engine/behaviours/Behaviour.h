#pragma once
#include <string>
#include "..//GameObject.h"
#include <glad/glad.h>

// Available Behaviour Types
enum class BehaviourType { PhysicsObject, Collider, BoxCollider, SphereCollider, AudioSource, MeshRenderer };

class Behaviour
{
public:
	Behaviour(int gameObjectId, BehaviourType type);

	~Behaviour();

	// Update
	virtual void Update(float deltaTime) = 0;

	// Draw
	virtual void Draw() = 0;

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string message) = 0;

	// Get this behaviour's type.
	BehaviourType GetType() const;

	void SetId(int _id);

	//GameObject* gameObject;

protected:
	// The id of the game object that this component belongs to.
	int m_gameObjectId;

	// Behaviour's type
	BehaviourType m_type;
};