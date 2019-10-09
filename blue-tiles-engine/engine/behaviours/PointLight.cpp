#include "PointLight.h"

PointLight::PointLight(glm::vec3 colour, float ambient, float diffuse, float specular, float constant, float linear, float quadratic)
	: Light(colour, ambient, diffuse, specular)
	, m_constant(constant)
	, m_linear(linear)
	, m_quadratic(quadratic)
{
}

void PointLight::Draw(Shader& shader)
{
}
