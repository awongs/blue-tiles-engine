#pragma once

#include <glm/glm.hpp>


class Camera
{
public:
	// Deconstructor.
	~Camera();

	// Instance accessor.
	static Camera* GetInstance();

	// Sets the position to an input vec3.
	void SetPosition(glm::vec3 position);

	// Sets the orientation to an input vec3.
	void SetOrientation(glm::vec3 orientation);

	// Performs a translation by the input vec3.
	void Translate(glm::vec3 translation);

	// Performs a rotation by the input vec3.
	void Rotate(glm::vec3 rotation);

	// Position accessor.
	glm::vec3 GetPosition() const;
	
	// Orientation accessor.
	glm::vec3 GetOrientation() const;

private:
	// Constructor.
	Camera();

	// Singleton instance variable.
	static Camera* m_instance;

	// Calculates and sets the view matrix.
	void CalculateViewMatrix();

	// Calculates and sets the projection matrix.
	void CalculateProjectionMatrix();

	// Field of view.
	float m_fov = 60.0f;

	// Current position of the camera.
	glm::vec3 m_position;

	// Current orientation of the camera.
	glm::vec3 m_orientation;

	// The camera's view matrix.
	glm::mat4 m_viewMatrix;

	// The camera's projection matrix.
	glm::mat4 m_projectionMatrix;
};