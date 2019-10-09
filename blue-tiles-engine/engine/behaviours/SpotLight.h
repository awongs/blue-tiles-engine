#pragma once
#include <glm/glm.hpp>
#include "Light.h"

class SpotLight : public Light
{
public:
	// Constructor.
	SpotLight(glm::vec3 colour, glm::vec3 direction, float innerRadius, float outerRadius, 
		float ambient = 1.0f, float diffuse = 1.0f, float specular = 1.0f,
		float constant = 1.0f, float linear = 1.0f, float quadratic = 1.0f);

	// Overridden render.
	void Render(Shader& shader, int lightIndex = -1) override;

private:

	// The direction that the light is facing towards.
	glm::vec3 m_direction;

	// Radius of the inner ring.
	float m_innerRadius;

	// Radius of the outer ring.
	float m_outerRadius;

	// Constant attenuation factor.
	float m_constant;

	// Linear attenuation factor.
	float m_linear;

	// Quadratic attenuation factor.
	float m_quadratic;
};