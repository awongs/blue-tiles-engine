#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Light.h"

// Structure that holds spot light variables.
// Used for sending lighting variables to the shader.
struct SLight
{
	float ambient; // 4
	float diffuse; // 8
	float specular; // 12

	glm::vec4 colour; // 28
	glm::vec4 position; // 44
	glm::vec4 direction; // 60

	float innerRadius; // 64
	float outerRadius; // 68

	float constant; // 72
	float linear; // 76
	float quadratic; // 80
};

class SpotLight : public Light
{
public:
	// Constructor.
	SpotLight(glm::vec3 colour, glm::vec3 direction, float innerRadius, float outerRadius, 
		float ambient = 1.0f, float diffuse = 1.0f, float specular = 1.0f,
		float constant = 1.0f, float linear = 1.0f, float quadratic = 1.0f);

	// Overridden render.
	void Render(Shader& shader, int bufferOffset = -1) override;

	// Overridden update.
	virtual void Update(float deltaTime) override;

	// Setter for the position offset.
	void SetPositionOffset(const glm::vec3 positionOffset);

private:
	// Struct representation of this light source.
	std::shared_ptr<SLight> m_lightStruct;

	// The direction that the light is facing towards.
	glm::vec3 m_direction;

	// Radius of the inner ring.
	float m_innerRadius;

	// Radius of the outer ring.
	float m_outerRadius;

	// Constant attenuation factor.
	float m_constant;

	// Linear attenuation factor.
	float m_linear;

	// Quadratic attenuation factor.
	float m_quadratic;

	// How much to offset the position of the spot light by.
	glm::vec3 m_positionOffset;
};