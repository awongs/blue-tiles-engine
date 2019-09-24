#pragma once
#include <glad/glad.h>
#include <string>
#include <list>
#include <memory>
#include <glm/glm.hpp>
#include "Behaviour.h"

class GameObject
{

public: 
	GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours);
	GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours, glm::vec3 pos);
	GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours, glm::vec3 pos, glm::vec3 rot);
	GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);
	~GameObject();

	// id of GameObject
	GLuint id;

	// Name of GameObject
	std::string name;

	glm::vec3 position;

	glm::vec3 rotation;

	glm::vec3 scale;

	// Gets the behaviour of type behaviourType
	/*std::unique_ptr<Behaviour> GetBehaviour(BehaviourType type);*/

	// Update
	virtual void Update();

	// Draw
	virtual void Draw();

	// Handles messages
	virtual bool HandleMessage(std::string message, BehaviourType type);
private:
	std::list<std::unique_ptr<Behaviour>> m_Behaviours;
};
