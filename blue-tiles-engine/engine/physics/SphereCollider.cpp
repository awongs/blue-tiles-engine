#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius) :
	Collider(glm::vec3(radius)), m_radius(radius)
{

}

SphereCollider::~SphereCollider()
{

}

float SphereCollider::GetRadius() const
{
	return m_radius;
}

Collider::Type SphereCollider::GetType() const
{
	return Type::SPHERE;
}