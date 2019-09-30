#pragma once
#include <string>

// Available Behaviour Types
enum class BehaviourType { PhysicsObject, Collider, BoxCollider, SphereCollider, AudioSource, MeshRenderer };

class Behaviour
{
public:
	Behaviour(BehaviourType _type);

	~Behaviour();


	// Behaviour's type
	BehaviourType type;

	// Update
	virtual void Update(float deltaTime);

	// Draw
	virtual void Draw();

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string message);
};