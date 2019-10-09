#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	// Constructor.
	PointLight(glm::vec3 colour, float ambient = 1.0f, float diffuse = 1.0f, float specular = 1.0f, 
		float constant = 1.0f, float linear = 1.0f, float quadratic = 1.0f);

	// Overridden render.
	void Render(Shader& shader, int lightIndex) override;

private:
	// Constant attenuation factor.
	float m_constant;

	// Linear attenuation factor.
	float m_linear;

	// Quadratic attenuation factor.
	float m_quadratic;
};