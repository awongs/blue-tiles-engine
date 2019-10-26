#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera()
	: m_position()
	, m_orientation()
	, m_viewMatrix()
	, m_projectionMatrix()
{
	SetOrientation(glm::vec3(glm::radians(75.0f), 0.0f, 0.0f));

	// Initial matrix values
	CalculateViewMatrix();
	CalculatePerspectiveView();
	CalculateBoundingBox();
}

Camera::~Camera()
{
}

Camera& Camera::GetInstance()
{
	// This instance will be initialized the first time GetInstance is called
	static Camera instance;
	return instance;
}

void Camera::SetPosition(const glm::vec3 position)
{
	m_position = position;

	// Update view matrix and bounding box
	CalculateViewMatrix();
	CalculateBoundingBox();
}

void Camera::SetOrientation(const glm::vec3 orientation)
{
	m_orientation = orientation;

	// Update view matrix and bounding box
	CalculateViewMatrix();
	CalculateBoundingBox();
}

void Camera::Translate(const glm::vec3 translation)
{
	m_position += translation;

	// Update view matrix and bounding box
	CalculateViewMatrix();
	CalculateBoundingBox();
}

void Camera::Rotate(const glm::vec3 rotation)
{
	m_orientation += rotation;

	// Update view matrix and bounding box
	CalculateViewMatrix();
	CalculateBoundingBox();
}

glm::vec3 Camera::GetPosition() const
{
	return m_position;
}

glm::vec3 Camera::GetOrientation() const
{
	return m_orientation;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

bool Camera::IsWithinBoundingBox(glm::vec3 point) const
{
	if (point.x > m_boundingBox.maxX || point.y > m_boundingBox.maxY || point.y > m_boundingBox.maxZ
	 || point.x < m_boundingBox.minX || point.y < m_boundingBox.minY || point.z < m_boundingBox.minZ)
	{
		return false;
	}

	return true;
}

Camera::BoundingBox Camera::GetBoundingBox() const
{
	return m_boundingBox;
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

	// Using negative position here to simulate an actual camera moving instead of the world
	positionMatrix = glm::translate(positionMatrix, -m_position);

	// View matrix is rotation multiplied by position
	m_viewMatrix = rotationMatrix * positionMatrix;
}
 
void Camera::CalculatePerspectiveView()
{
	// TODO: Make aspect ratio not hard coded
	m_projectionMatrix = glm::perspective(m_fov, 4.0f / 3.0f, 1.0f, 50.0f);
}

void Camera::CalculateBoundingBox()
{
	// Vector4 definitions for the eight corners of a perspective camera
	glm::vec4 corners[8] = { glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
							glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
							glm::vec4(1.0f, -1.0f, 1.0f, 1.0f), glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),
							glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, -1.0f, 1.0f) };

	// Get the inverse of the camera's projection and view matrices
	glm::mat4 inverseProjectionView = glm::inverse(m_projectionMatrix * m_viewMatrix);
	for (int i = 0; i < 8; i++)
	{
		corners[i] = inverseProjectionView * corners[i];
		corners[i] /= corners[i].w;
	}

	// Find the minimum and maximum values for the three coordinate axes
	m_boundingBox.minX = corners[0].x, m_boundingBox.minY = corners[0].y, m_boundingBox.minZ = corners[0].z,
	m_boundingBox.maxX = corners[0].x, m_boundingBox.maxY = corners[0].y, m_boundingBox.maxZ = corners[0].z;
	for (int i = 1; i < 8; i++)
	{
		if (corners[i].x < m_boundingBox.minX)
			m_boundingBox.minX = corners[i].x;
		if (corners[i].x > m_boundingBox.maxX)
			m_boundingBox.maxX = corners[i].x;
		if (corners[i].y < m_boundingBox.minY)
			m_boundingBox.minY = corners[i].y;
		if (corners[i].y > m_boundingBox.maxY)
			m_boundingBox.maxY = corners[i].y;
		if (corners[i].z < m_boundingBox.minZ)
			m_boundingBox.minZ = corners[i].z;
		if (corners[i].z > m_boundingBox.maxZ)
			m_boundingBox.maxZ = corners[i].z;
	}
}
