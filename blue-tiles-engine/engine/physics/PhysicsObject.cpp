#include "PhysicsObject.h"
#include "SphereCollider.h"

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::~PhysicsObject()
{

}

Collider *PhysicsObject::GetCollider() const
{
	if (m_collider == nullptr)
		return nullptr;

	return m_collider.get();
}

void PhysicsObject::SetCollider(Collider *collider)
{
	m_collider = std::unique_ptr<Collider>(collider);
}

GLuint PhysicsObject::GetGameObjectId() const
{
	return m_gameObjectId;
}

void PhysicsObject::SetGameObjectId(GLuint id)
{
	m_gameObjectId = id;
}
