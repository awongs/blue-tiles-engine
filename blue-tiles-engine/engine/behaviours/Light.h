#pragma once

#include <glm/glm.hpp>

#include "Behaviour.h"

// Default light is treated as a directional light
class Light : public Behaviour
{
public:
	// Constuctor.
	Light(glm::vec3 colour, float ambient, float diffuse, float specular);

	// Overwritten functions.
	virtual void Update(float deltaTime) override;
	virtual void Draw(Shader& shader) override;
	virtual bool HandleMessage(unsigned int senderID, std::string message) override;

protected:
	// The colour of the light
	glm::vec3 colour;

	// The ambient intensity of the light.
	float ambient;

	// The diffuse intensity of the light.
	float diffuse;

	// The specular intensity of the light.
	float specular;
};