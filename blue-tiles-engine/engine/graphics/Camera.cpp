#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

// Singleton instance definition
Camera* Camera::m_instance;

Camera::Camera()
	: m_position()
	, m_orientation()
	, m_viewMatrix()
	, m_projectionMatrix()
{
}

Camera::~Camera()
{
	m_instance = nullptr;
}

Camera* Camera::GetInstance()
{
	// Initialize camera if this is the first time
	if (m_instance == nullptr)
	{
		m_instance = new Camera();
	}
	return m_instance;
}

void Camera::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void Camera::SetOrientation(glm::vec3 orientation)
{
	m_orientation = orientation;
}

void Camera::Translate(glm::vec3 translation)
{
	m_position += translation;
}

void Camera::Rotate(glm::vec3 rotation)
{
	m_orientation += rotation;
}

glm::vec3 Camera::GetPosition() const
{
	return m_position;
}

glm::vec3 Camera::GetOrientation() const
{
	return m_orientation;
}

void Camera::CalculateViewMatrix()
{
	// Create an identity matrix for rotation
	glm::mat4 rotationMatrix = glm::mat4(1);

	// Calculate rotation matrix
	rotationMatrix = glm::rotate(rotationMatrix, m_orientation.x, glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, m_orientation.y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, m_orientation.z, glm::vec3(0, 0, 1));

	// Create an identity matrix for position
	glm::mat4 positionMatrix = glm::mat4(1);

	// Calculate position matrix
	positionMatrix = glm::translate(positionMatrix, m_position);

	// View matrix is rotation multiplied by position
	m_viewMatrix = rotationMatrix * positionMatrix;
}

void Camera::CalculatePerspectiveView()
{
	// TODO: Make aspect ratio not hard coded
	m_projectionMatrix = glm::perspective(m_fov, 4.0f / 3.0f, 1.0f, 400.0f);
}
