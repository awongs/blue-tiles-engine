#include "Light.h"
#include "Behaviour.h"

void Light::Update(float deltaTime)
{
	// Do nothing
}

bool Light::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}

Light::Light(int gameObjectID, glm::vec3 colour, float ambient, float diffuse, float specular)
	: Behaviour(gameObjectID, BehaviourType::Light)
	, colour(colour)
	, ambient(ambient)
	, diffuse(diffuse)
	, specular(specular)
{
}
