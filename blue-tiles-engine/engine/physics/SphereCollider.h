#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(float radius);
	~SphereCollider();

	float GetRadius() const;

	virtual Collider::Type GetType() const;

private:
	float m_radius;
};