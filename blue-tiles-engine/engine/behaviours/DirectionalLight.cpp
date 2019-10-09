#include "DirectionalLight.h"
#include "../graphics/Shader.h"


DirectionalLight::DirectionalLight(glm::vec3 colour, glm::vec3 direction, float ambient, float diffuse, float specular)
	: Light(BehaviourType::DirectionalLight, colour, ambient, diffuse, specular)
	, m_direction(direction)
{
}

void DirectionalLight::Render(Shader& shader, int lightIndex)
{
	shader.SetUniform1f("dirLight.ambientIntensity", m_ambient);
	shader.SetUniform1f("dirLight.diffuseIntensity", m_diffuse);
	shader.SetUniform1f("dirLight.specularIntensity", m_specular);
	shader.SetUniform3f("dirLight.colour", m_colour);
	shader.SetUniform3f("dirLight.direction", m_direction);
}
