#include "SpotLight.h"
#include "../graphics/Shader.h"
#include "../GameObject.h"

SpotLight::SpotLight(glm::vec3 colour, glm::vec3 direction, float innerRadius, float outerRadius, 
	float ambient, float diffuse, float specular, 
	float constant, float linear, float quadratic)
	: Light(BehaviourType::SpotLight, colour, ambient, diffuse, specular)
	, m_direction(direction)
	, m_innerRadius(innerRadius)
	, m_outerRadius(outerRadius)
	, m_constant(constant)
	, m_linear(linear)
	, m_quadratic(quadratic)
{
}

void SpotLight::Render(Shader& shader, int lightIndex)
{
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].ambientIntensity", m_ambient);
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].diffuseIntensity", m_diffuse);
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].specularIntensity", m_specular);
	shader.SetUniform3f("spotLights[" + std::to_string(lightIndex) + "].colour", m_colour);
	shader.SetUniform3f("spotLights[" + std::to_string(lightIndex) + "].position", gameObject->position);
	shader.SetUniform3f("spotLights[" + std::to_string(lightIndex) + "].direction", m_direction);
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].innerRadius", m_innerRadius);
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].outerRadius", m_outerRadius);
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].constant", m_constant);
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].linear", m_linear);
	shader.SetUniform1f("spotLights[" + std::to_string(lightIndex) + "].quadratic", m_quadratic);
}
