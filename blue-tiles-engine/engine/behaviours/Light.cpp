#include "Light.h"
#include "Behaviour.h"
#include "../graphics/Shader.h"
#include "../GameObject.h"

Light::Light(BehaviourType type, glm::vec3 colour, float ambient, float diffuse, float specular)
	: Behaviour(type)
	, m_colour(colour)
	, m_ambient(ambient)
	, m_diffuse(diffuse)
	, m_specular(specular)
{
}

void Light::Update(float deltaTime)
{
	// Do nothing
}

void Light::Draw(Shader& shader)
{
	// Do nothing
}

bool Light::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}

void Light::OnCollisionStay(GLuint other)
{
}
