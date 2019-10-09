#include "Light.h"
#include "Behaviour.h"

void Light::Update(float deltaTime)
{
	// Do nothing
}

void Light::Draw(Shader& shader)
{

}

bool Light::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}

Light::Light(glm::vec3 colour, float ambient, float diffuse, float specular)
	: Behaviour(BehaviourType::Light)
	, colour(colour)
	, ambient(ambient)
	, diffuse(diffuse)
	, specular(specular)
{
}
