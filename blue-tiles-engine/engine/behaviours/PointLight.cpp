#include "PointLight.h"
#include "../graphics/Shader.h"
#include "../GameObject.h"

PointLight::PointLight(glm::vec3 colour, float ambient, float diffuse, float specular, float constant, float linear, float quadratic)
	: Light(BehaviourType::PointLight, colour, ambient, diffuse, specular)
	, m_constant(constant)
	, m_linear(linear)
	, m_quadratic(quadratic)
{
}

void PointLight::Render(Shader& shader, int lightIndex)
{
	shader.SetUniform1f("pointLights[" + std::to_string(lightIndex) + "].ambientIntensity", m_ambient);
	shader.SetUniform1f("pointLights[" + std::to_string(lightIndex) + "].diffuseIntensity", m_diffuse);
	shader.SetUniform1f("pointLights[" + std::to_string(lightIndex) + "].specularIntensity", m_specular);
	shader.SetUniform3f("pointLights[" + std::to_string(lightIndex) + "].colour", m_colour);
	shader.SetUniform3f("pointLights[" + std::to_string(lightIndex) + "].position", gameObject->position);
	shader.SetUniform1f("pointLights[" + std::to_string(lightIndex) + "].constant", m_constant);
	shader.SetUniform1f("pointLights[" + std::to_string(lightIndex) + "].linear", m_linear);
	shader.SetUniform1f("pointLights[" + std::to_string(lightIndex) + "].quadratic", m_quadratic);
}