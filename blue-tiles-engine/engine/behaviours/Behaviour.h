#pragma once
#include <string>
#include <glad/glad.h>

// Available Behaviour Types
enum class BehaviourType
{ 
	Physics, 
	Collider, 
	BoxCollider, 
	SphereCollider,
	AudioSource,
	MeshRenderer, 
	TextBehaviour, 
	DirectionalLight, 
	PointLight,
	SpotLight, 
	PlayerMovement, 
	FollowGameObject,
	Inventory,
	GuardDetection,
	SimpleGuardMovementBehaviour,
	ObjectBehaviour,
	TileBehaviour
};

class GameObject;
class Shader;

class Behaviour
{
public:
	Behaviour(BehaviourType type);

	virtual ~Behaviour();

	// Update
	virtual void Update(float deltaTime) = 0;

	// Draw
	virtual void Draw(Shader& shader) = 0;

	virtual void OnCollisionStay(GLuint other) = 0;

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string& message) = 0;

	// Get this behaviour's type.
	BehaviourType GetType() const;

	GameObject* gameObject;

protected:

	// Behaviour's type
	BehaviourType m_type;
};