#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include "Camera.h"

Camera::Camera()
	: m_position()
	, m_orientation()
	, m_viewMatrix()
	, m_projectionMatrix()
{
	// Initial matrix values
	CalculateViewMatrix();
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

void Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;

	// Update view matrix and bounding box
	CalculateViewMatrix();
	CalculateBoundingBox();
}

void Camera::SetOrientation(const glm::vec3& orientation)
{
	m_orientation = orientation;

	// Update view matrix and bounding box
	CalculateViewMatrix();
	CalculateBoundingBox();
}

void Camera::Translate(const glm::vec3& translation)
{
	m_position += translation;

	// Update view matrix and bounding box
	CalculateViewMatrix();
	CalculateBoundingBox();
}

void Camera::Rotate(const glm::vec3& rotation)
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

glm::vec3 Camera::GetForward() const
{
	return m_forward;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

std::pair<float, float> Camera::GetZClip() const
{
	return m_zClip;
}

bool Camera::IsWithinBoundingBox(const glm::vec3& point, const glm::vec3& additionalMargin) const
{
	if (point.x > m_boundingBox.maxX + additionalMargin.x || point.x < m_boundingBox.minX - additionalMargin.x ||
		point.y > m_boundingBox.maxY + additionalMargin.y || point.y < m_boundingBox.minY - additionalMargin.y ||
		point.z > m_boundingBox.maxZ + additionalMargin.z || point.z < m_boundingBox.minZ - additionalMargin.z)
	{
		return false;
	}

	return true;
}

void Camera::CalculateViewMatrix()
{
	// Calculate rotation matrix
	glm::quat rotationQuaternion = glm::quat(m_orientation);
	glm::mat4 rotationMatrix = glm::mat4(rotationQuaternion);

	// Forward vector is equal to the z column in the rotation matrix
	m_forward = glm::normalize(glm::vec3(-rotationMatrix[0][2], -rotationMatrix[1][2], rotationMatrix[2][2]));

	// Using negative position here to simulate an actual camera moving instead of the world
	glm::mat4 positionMatrix = glm::translate(glm::mat4(1), -m_position);

	// View matrix is rotation multiplied by position
	m_viewMatrix = rotationMatrix * positionMatrix;
}
 
void Camera::CalculatePerspectiveView(const float fov, const float aspect, const float nearClip, const float farClip)
{
	m_zClip.first = nearClip;
	m_zClip.second = farClip;
	m_projectionMatrix = glm::perspective(fov, aspect, nearClip, farClip);
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
