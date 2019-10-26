#pragma once

#include <glm/glm.hpp>

class Camera
{
public:

	// Storing variables for the camera's bounding box.
	// See https://i.stack.imgur.com/YBcd7.png
	struct BoundingBox
	{
		float minX;
		float maxX;
		float minY; 
		float maxY;
		float minZ;
		float maxZ;
	};

	// Deconstructor.
	virtual ~Camera();

	// Prevent copying.
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

	// Instance accessor.
	static Camera& GetInstance();

	// Sets the position to an input vec3.
	void SetPosition(const glm::vec3 position);

	// Sets the orientation to an input vec3.
	void SetOrientation(const glm::vec3 orientation);

	// Performs a translation by the input vec3.
	void Translate(const glm::vec3 translation);

	// Performs a rotation by the input vec3.
	void Rotate(const glm::vec3 rotation);

	// Position accessor.
	glm::vec3 GetPosition() const;
	
	// Orientation accessor.
	glm::vec3 GetOrientation() const;

	// View matrix accessor.
	glm::mat4 GetViewMatrix() const;

	// Projection matrix accessor.
	glm::mat4 GetProjectionMatrix() const;

	// Checks if a given point is within the camera's bounding box.
	// Returns true if it is, otherwise false.
	bool IsWithinBoundingBox(glm::vec3 point) const;

	// Bounding box accessor.
	BoundingBox GetBoundingBox() const;

private:
	// Constructor.
	Camera();

	// Calculates and sets the view matrix.
	void CalculateViewMatrix();

	// Calculates and sets the projection matrix to a perspective view.
	void CalculatePerspectiveView();

	// Calculates and sets the camera's bounding box.
	// See https://gamedev.stackexchange.com/questions/69749/get-the-8-corners-of-camera-frustrum
	void CalculateBoundingBox();

	// Field of view.
	float m_fov = glm::radians(60.0f);

	// Current position of the camera.
	glm::vec3 m_position;

	// Current orientation of the camera.
	glm::vec3 m_orientation;

	// The camera's view matrix.
	glm::mat4 m_viewMatrix;

	// The camera's bounding box.
	BoundingBox m_boundingBox;

	// The camera's projection matrix.
	glm::mat4 m_projectionMatrix;
};