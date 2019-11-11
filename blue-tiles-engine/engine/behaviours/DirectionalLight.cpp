#include <glm/gtc/matrix_transform.hpp>
#include <utility>

#include "DirectionalLight.h"
#include "../graphics/Shader.h"
#include "../graphics/Camera.h"

DirectionalLight::DirectionalLight(glm::vec3 colour, glm::vec3 direction, float ambient, float diffuse, float specular)
	: Light(BehaviourType::DirectionalLight, colour, ambient, diffuse, specular)
	, m_direction(glm::normalize(direction))
{
	// Vector4 definitions for the eight corners of a perspective camera
	glm::vec4 corners[8] = { glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
							glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
							glm::vec4(1.0f, -1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),
							glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, -1.0f, 1.0f) };

	// Get the inverse of the camera's projection matrix
	glm::mat4 inverseCameraProjection = glm::inverse(Camera::GetInstance().GetProjectionMatrix());
	for (int i = 0; i < 8; i++)
	{
		corners[i] = inverseCameraProjection * corners[i];
		corners[i] /= corners[i].w;
	}

	// Find the minimum and maximum values for the three coordinate axes
	float minX = corners[0].x, minY = corners[0].y, minZ = corners[0].z, maxX = corners[0].x, maxY = corners[0].y, maxZ = corners[0].z;
	for (int i = 1; i < 8; i++)
	{
		if (corners[i].x < minX)
			minX = corners[i].x;
		if (corners[i].x > maxX)
			maxX = corners[i].x;
		if (corners[i].y < minY)
			minY = corners[i].y;
		if (corners[i].y > maxY)
			maxY = corners[i].y;
		if (corners[i].z < minZ)
			minZ = corners[i].z;
		if (corners[i].z > maxZ)
			maxZ = corners[i].z;
	}

	// Create the orthographic projection matrix for shadow mapping
	// TODO: Hardcoded -10.0f
	std::pair<float, float> zClip = Camera::GetInstance().GetZClip();
	m_projectionMatrix = glm::ortho(minX, maxX, minY, maxY, zClip.first, zClip.second - 10.0f);

	// Calculate view matrix for this light
	m_viewMatrix = glm::lookAt(-m_direction,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// Calculate light space matrix.
	m_lightSpaceMatrix = m_projectionMatrix * m_viewMatrix;
}

void DirectionalLight::Render(Shader& shader, int bufferOffset)
{
	shader.SetUniform1f("dirLight.ambientIntensity", m_ambient);
	shader.SetUniform1f("dirLight.diffuseIntensity", m_diffuse);
	shader.SetUniform1f("dirLight.specularIntensity", m_specular);
	shader.SetUniform3f("dirLight.colour", m_colour);
	shader.SetUniform3f("dirLight.direction", m_direction);
}

glm::vec3 DirectionalLight::GetDirection() const
{
	return m_direction;
}

glm::mat4 DirectionalLight::GetLightSpaceMatrix() const
{
	return m_lightSpaceMatrix;
}
