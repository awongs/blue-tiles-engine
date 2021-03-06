#pragma once

#include <glm/glm.hpp>

#include "Behaviour.h"

class Light : public Behaviour
{
public:
	// Renders the light. Not the same as draw.
	virtual void Render(Shader& shader, int bufferOffset) = 0;

	// Overwritten functions.
	virtual void Update(float deltaTime) override;

	// Note: Does not render the light, only use this for drawing a debug cube
	virtual void Draw(Shader& shader) override; 
	virtual bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

	// Setter for the light's colour
	void SetColour(const glm::vec3& colour);

	// Setter for the light's intensities.
	void SetAmbientIntensity(const float ambient);
	void SetDiffuseIntensity(const float diffuse);
	void SetSpecularIntensity(const float specular);

protected:
	// Constuctor.
	Light(BehaviourType type, glm::vec3 colour, float ambient = 1.0f, float diffuse = 1.0f, float specular = 1.0f);

	// The colour of the light
	glm::vec3 m_colour;

	// The ambient intensity of the light.
	float m_ambient;

	// The diffuse intensity of the light.
	float m_diffuse;

	// The specular intensity of the light.
	float m_specular;
};