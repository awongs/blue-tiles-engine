#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 colour, float ambient = 1.0f, float diffuse = 1.0f, float specular = 1.0f, 
		float constant = 1.0f, float linear = 1.0f, float quadratic = 1.0f);

	// Overwritten draw function
	virtual void Draw(Shader& shader) override;

private:
	// Constant attenuation factor.
	float m_constant;

	// Linear attenuation factor.
	float m_linear;

	// Quadratic attenuation factor.
	float m_quadratic;
};