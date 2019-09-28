#include "PhysicsObject.h"
#include "../debugbt/DebugLog.h"

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
	DebugLog::Info("Collision detected between (" + 
		std::to_string(m_gameObjectId) +  " " + 
		std::to_string(other.GetId()) + ").");

	m_onCollision(other);
}

GLuint PhysicsObject::GetId() const
{
	return m_gameObjectId;
}
