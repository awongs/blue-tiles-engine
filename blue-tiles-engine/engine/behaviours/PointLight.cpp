#include "PointLight.h"
#include "SpotLight.h"
#include "../graphics/Shader.h"
#include "../GameObject.h"

PointLight::PointLight(glm::vec3 colour, float ambient, float diffuse, float specular, float constant, float linear, float quadratic)
	: Light(BehaviourType::PointLight, colour, ambient, diffuse, specular)
	, m_constant(constant)
	, m_linear(linear)
	, m_quadratic(quadratic)
{
	// Initialize the struct values
	m_lightStruct = std::shared_ptr<PLight>(new PLight());
	m_lightStruct->ambient = m_ambient;
	m_lightStruct->diffuse = m_diffuse;
	m_lightStruct->specular = m_specular;

	// Offset a value over because the uniform buffer offsets by 4 bytes
	m_lightStruct->colour = glm::vec4(0.0f, m_colour);

	m_lightStruct->constant = m_constant;
	m_lightStruct->linear = m_linear;
	m_lightStruct->quadratic = m_quadratic;
}

void PointLight::Render(Shader& shader, int bufferOffset)
{
	// Send struct information to shader
	// Assumes that the uniform buffer is already bound before calling this function
	glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, sizeof(PLight), m_lightStruct.get());
}

void PointLight::Update(float deltaTime)
{
	// Offset a value over because the uniform buffer offsets by 4 bytes
	m_lightStruct->position = glm::vec4(0.0f, gameObject->position);
}



