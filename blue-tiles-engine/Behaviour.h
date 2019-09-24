#pragma once
#include <string>

enum class BehaviourType { PhysicsObject, Collider, BoxCollider, SphereCollider, AudioSource };

class Behaviour
{
public:
	Behaviour(BehaviourType _type);

	~Behaviour();

	BehaviourType type;

	// Update
	virtual void Update();

	// Draw
	virtual void Draw();

	// Handles messages
	virtual bool HandleMessage(std::string message);
};