#include "SpotLight.h"
#include "../graphics/Shader.h"
#include "../GameObject.h"

#include <ctime>
#include <glm/gtc/random.hpp>
SpotLight::SpotLight(glm::vec3 colour, glm::vec3 direction, float innerRadius, float outerRadius, 
	float ambient, float diffuse, float specular, 
	float constant, float linear, float quadratic)
	: Light(BehaviourType::SpotLight, colour, ambient, diffuse, specular)
	, m_direction(glm::normalize(direction))
	, m_innerRadius(innerRadius)
	, m_outerRadius(outerRadius)
	, m_constant(constant)
	, m_linear(linear)
	, m_quadratic(quadratic)
{
	// Initialize the struct values
	m_lightStruct = std::shared_ptr<SLight>(new SLight());
	m_lightStruct->ambient = m_ambient;
	m_lightStruct->diffuse = m_diffuse;
	m_lightStruct->specular = m_specular;

	// Offset a value over because the uniform buffer offsets by 4 bytes
	m_lightStruct->colour = glm::vec4(0.0f, m_colour);
	m_lightStruct->direction = glm::vec4(0.0f, m_direction);

	m_lightStruct->innerRadius = m_innerRadius;
	m_lightStruct->outerRadius = m_outerRadius;

	m_lightStruct->constant = m_constant;
	m_lightStruct->linear = m_linear;
	m_lightStruct->quadratic = m_quadratic;
}

void SpotLight::Render(Shader& shader, int bufferOffset)
{
	// Send struct information to shader
	// Assumes that the uniform buffer is already bound before calling this function
	glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, sizeof(SLight), m_lightStruct.get());
}

void SpotLight::Update(float deltaTime)
{
	// Offset a value over because the uniform buffer offsets by 4 bytes
	m_lightStruct->position = glm::vec4(0.0f, gameObject->position);
}
