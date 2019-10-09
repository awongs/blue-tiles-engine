#pragma once
#include <string>
#include <glad/glad.h>

// Available Behaviour Types
enum class BehaviourType { PhysicsObject, Collider, BoxCollider, SphereCollider, AudioSource, MeshRenderer, Light};

class GameObject;
class Shader;

class Behaviour
{
public:
	Behaviour(BehaviourType type);

	~Behaviour();

	// Update
	virtual void Update(float deltaTime) = 0;

	// Draw
	virtual void Draw(Shader& shader) = 0;

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string message) = 0;

	// Get this behaviour's type.
	BehaviourType GetType() const;

	GameObject* gameObject;

protected:

	// Behaviour's type
	BehaviourType m_type;
};