#pragma once

#include <memory>
#include "Light.h"

// Structure that holds point light variables.
// Used for sending lighting variables to the shader.
struct PLight
{
	float ambient; // 4
	float diffuse; // 8
	float specular; // 12

	glm::vec4 colour; // 28
	glm::vec4 position; // 44

	float constant; // 48
	float linear; // 52
	float quadratic; // 56

	// OpenGL pads uniform structs to multiples of 16 bytes
	glm::vec2 padding; // 64
};

class PointLight : public Light
{
public:
	// Constructor.
	PointLight(glm::vec3 colour, float ambient = 1.0f, float diffuse = 1.0f, float specular = 1.0f, 
		float constant = 1.0f, float linear = 1.0f, float quadratic = 1.0f);

	// Overridden render.
	void Render(Shader& shader, int bufferOffset = -1) override;

	// Overridden update.
	virtual void Update(float deltaTime) override;

private:
	// Struct representation of this light source.
	std::shared_ptr<PLight> m_lightStruct;

	// Constant attenuation factor.
	float m_constant;

	// Linear attenuation factor.
	float m_linear;

	// Quadratic attenuation factor.
	float m_quadratic;
};