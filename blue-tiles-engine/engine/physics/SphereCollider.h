#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius);
	~SphereCollider();

private:
	float m_radius;
};