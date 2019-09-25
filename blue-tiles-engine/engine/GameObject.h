#pragma once
#include <glad/glad.h>
#include <string>
#include <list>
#include <memory>
#include <iterator>
#include <glm/glm.hpp>
#include <vector>
#include "Behaviour.h"

class GameObject
{

public: 
	// Constructor only behaviour
	GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours);

	// Constructor behaviours + name
	GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n);

	// Constructor behaviours + name + position
	GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n, glm::vec3 pos);

	// Constructor behaviours + name + position + rotation
	GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n, glm::vec3 pos, glm::vec3 rot);

	// Constructor behaviours + name + position + rotation + scale
	GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);

	// Deconstructor
	~GameObject();

	// id of GameObject
	GLuint id;

	// Name of GameObject
	std::string name;

	glm::vec3 position;

	glm::vec3 rotation;

	glm::vec3 scale;

	// Update
	virtual void Update();

	// Draw
	virtual void Draw();

	// Handles messages
	virtual bool HandleMessage(std::string message, BehaviourType type);

	// Gets behaviour of BehaviourType; Returns nullptr if doesn't exist
	Behaviour* GetBehaviour(BehaviourType type);
private:
	std::vector<std::unique_ptr<Behaviour>> m_Behaviours;
};
