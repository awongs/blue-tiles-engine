#include "PhysicsObject.h"
#include "SphereCollider.h"

PhysicsObject::PhysicsObject(Collider *collider,
	std::function<void(const PhysicsObject &)> onCollision) :
	Behaviour(BehaviourType::PhysicsObject),
	m_collider(collider), m_onCollision(onCollision)
{

}

PhysicsObject::~PhysicsObject()
{

}

void PhysicsObject::Update(float deltaTime)
{

}

void PhysicsObject::Draw(Shader& shader)
{

}

bool PhysicsObject::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}

void PhysicsObject::OnCollision(const PhysicsObject &other)
{
	m_onCollision(other);
}

Collider *PhysicsObject::GetCollider() const
{
	if (m_collider == nullptr)
		return nullptr;

	return m_collider.get();
}