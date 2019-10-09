#pragma once

#include <glm/glm.hpp>

#include "Behaviour.h"

class Light : public Behaviour
{
public:
	// Overwritten functions.
	virtual void Update(float deltaTime) override;
	virtual bool HandleMessage(unsigned int senderID, std::string message) override;

protected:
	Light(glm::vec3 colour, float ambient, float diffuse, float specular);

	// The colour of the light
	glm::vec3 colour;

	// The ambient intensity of the light.
	float ambient;

	// The diffuse intensity of the light.
	float diffuse;

	// The specular intensity of the light.
	float specular;
};