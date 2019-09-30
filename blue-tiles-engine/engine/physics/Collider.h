#pragma once

#include <glm/glm.hpp>

class Collider
{
public:
	// Define with powers of 2 for bitwise ops in narrow phase.
	enum Type
	{
		BOX = 1,
		SPHERE = 2,
	};

	Collider(glm::vec3 halfSizes);

	glm::vec3 GetHalfSizes() const;

	void SetPosition(glm::vec3 newPos);
	glm::vec3 GetPosition() const;

	virtual Collider::Type GetType() const;

private:
	// This collider's position in world coordinates.
	glm::vec3 m_position;

	// All colliders should be defined with a bounding box for broad phase 
	// collision-checking.
	// This specifies the half lengths of the box for each axis.
	glm::vec3 m_halfSizes;
};