#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius) :
	Collider(glm::vec3(radius)), m_radius(radius)
{

}

SphereCollider::~SphereCollider()
{

}
