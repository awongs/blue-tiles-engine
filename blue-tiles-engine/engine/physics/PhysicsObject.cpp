#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(GLuint gameObjectId, Collider *collider,
	std::function<void(const PhysicsObject &)> onCollision) :
	m_gameObjectId(gameObjectId), m_onCollision(onCollision)
{
	m_collider = std::unique_ptr<Collider>(collider);
}

PhysicsObject::~PhysicsObject()
{

}

void PhysicsObject::OnCollision(const PhysicsObject &other)
{
	m_onCollision(other);
}

GLuint PhysicsObject::GetId() const
{
	return m_gameObjectId;
}

Collider *PhysicsObject::GetCollider() const
{
	return m_collider.get();
}