#include "PhysicsBehaviour.h"
#include "../physics/PhysicsEngine.h"
#include "../physics/PhysicsObject.h"
#include "../GameObject.h"

PhysicsBehaviour::PhysicsBehaviour(PhysicsEngine *physEngine,
	GLuint gameObjectId, Collider *collider,
	std::function<void(GLuint)> onCollision) :
	Behaviour(BehaviourType::Physics), m_physEngine(physEngine),
	m_collider(collider), m_onCollision(onCollision)
{
	// Register a PhysicsObject to the PhysicsEngine.
	if (m_physEngine != nullptr)
	{
		// Get a new, empty PhysicsObject.
		PhysicsObject *physObj{ m_physEngine->AddPhysicsObject() };

		// Set the collider for the PhysicsObject.
		physObj->SetCollider(collider);
		physObj->SetGameObjectId(gameObjectId);
	}
}

PhysicsBehaviour::PhysicsBehaviour(PhysicsEngine* physEngine, GLuint gameObjectId, Collider* collider)
	: Behaviour(BehaviourType::Physics)
	, m_physEngine(physEngine)
	, m_collider(collider)
{
	m_onCollision = [this](GLuint other) {

		gameObject->OnCollisionStay(other);

	};

	// Register a PhysicsObject to the PhysicsEngine.
	if (m_physEngine != nullptr)
	{
		// Get a new, empty PhysicsObject.
		PhysicsObject* physObj{ m_physEngine->AddPhysicsObject() };

		// Set the collider for the PhysicsObject.
		physObj->SetCollider(collider);
		physObj->SetGameObjectId(gameObjectId);
	}
}

PhysicsBehaviour::~PhysicsBehaviour()
{
	// Unregister its corresponding PhysicsObject from the PhysicsEngine.
	if (m_physEngine != nullptr && gameObject != nullptr)
		m_physEngine->RemovePhysicsObject(gameObject->id);
}

void PhysicsBehaviour::Update(float deltaTime)
{
	if (m_collider != nullptr)
		m_collider->SetPosition(gameObject->position);
}

void PhysicsBehaviour::Draw(Shader &shader)
{
}

bool PhysicsBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	if (message == PhysicsEngine::COLLISION_MESSAGE_STR)
	{
		m_onCollision(senderID);
		return true;
	}

	return false;
}

void PhysicsBehaviour::OnCollisionStay(GLuint other)
{
}

void PhysicsBehaviour::OnAttach(GameObject& gameObject)
{
	// Set position of collider when attached to prevent the
	// issue of all colliders being in the same position when the scene is first updated.
	if (m_collider != nullptr)
	{
		m_collider->SetPosition(gameObject.position);
	}
}

Collider *PhysicsBehaviour::GetCollider() const
{
	return m_collider;
}
