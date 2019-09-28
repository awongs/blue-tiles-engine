#pragma once

#include <glm/glm.hpp>

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(glm::vec2 halfSizes);
	~BoxCollider();

private:
	glm::vec2 m_halfSizes;
};