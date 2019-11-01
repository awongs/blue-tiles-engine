#pragma once
#include <glm/glm.hpp>
#include "Light.h"

class DirectionalLight : public Light
{
public:
	// Constructor.
	DirectionalLight(glm::vec3 colour, glm::vec3 direction, float ambient = 1.0f, float diffuse = 1.0f, float specular = 1.0f);

	// Overridden render.
	void Render(Shader& shader, int bufferOffset = -1) override;

	// Accessor for the light's direction.
	glm::vec3 GetDirection() const;

	// Accessor for the light space matrix.
	glm::mat4 GetLightSpaceMatrix() const;

private:

	// The direction that the light is facing towards.
	glm::vec3 m_direction;

	// The light's view matrix. Used for shadow mapping.
	glm::mat4 m_viewMatrix;

	// The light's orthographic projection matrix. Used for shadow mapping.
	glm::mat4 m_projectionMatrix;

	// The light's projection and view matrices combined. Used for shadow mapping.
	glm::mat4 m_lightSpaceMatrix;
};