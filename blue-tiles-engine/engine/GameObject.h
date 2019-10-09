#pragma once
#include <glad/glad.h>
#include <string>
#include <list>
#include <memory>
#include <iterator>
#include <glm/glm.hpp>
#include <vector>
#include "behaviours/Behaviour.h"

class Shader;

class GameObject
{
public: 
	// Constructor with default arguments
	GameObject(int _id, std::string n = "Default", glm::vec3 pos = glm::vec3(0, 0, 0), 
		glm::vec3 rot = glm::vec3(0, 0, 0), glm::vec3 sca = glm::vec3(1, 1, 1));

	// Deconstructor
	~GameObject();

	// Update
	virtual void Update(float deltaTime);
	
	// Draw
	virtual void Draw(Shader& shader);

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string message, BehaviourType type);

	// Accessor for the transform matrix.
	glm::mat4 GetTransformMatrix() const;

	// Updates the transform matrix.
	void UpdateTransformMatrix();

	// Gets behaviour of BehaviourType; Returns nullptr if doesn't exist
	Behaviour* GetBehaviour(BehaviourType type);

	// Adds a behaviour
	void AddBehaviour(Behaviour* behaviour);

	// id of GameObject
	GLuint id;

	// Name of GameObject
	std::string name;

	glm::vec3 position;

	glm::vec3 rotation;

	glm::vec3 scale;

	// The game object's forward vector.
	glm::vec3 forward;

private:
	std::vector<std::unique_ptr<Behaviour>> m_Behaviours;

	// The game object's transform/model matrix.
	glm::mat4 m_transformMatrix;
};
