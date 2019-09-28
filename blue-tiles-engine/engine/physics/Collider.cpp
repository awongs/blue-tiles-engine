#include "Collider.h"

Collider::Collider(glm::vec3 halfSizes) :
	m_halfSizes(halfSizes)
{

}

glm::vec3 Collider::GetHalfSizes() const
{
	return m_halfSizes;
}

void Collider::SetPosition(glm::vec3 newPos)
{
	m_position = newPos;
}

glm::vec3 Collider::GetPosition() const
{
	return m_position;
}
