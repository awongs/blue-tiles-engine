#pragma once
#include <glad/glad.h>
#include <string>
#include <list>
#include <memory>
#include <iterator>
#include <glm/glm.hpp>
#include <vector>
#include "behaviours/Behaviour.h"

class GameObject
{

public: 

	// Constructor behaviours + name
	GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours);

	// Constructor behaviours + name + position
	GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos);

	// Constructor behaviours + name + position + rotation
	GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos, glm::vec3 rot);

	// Constructor behaviours + name + position + rotation + scale
	GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);

	// Constructor name only
	GameObject(int _id, std::string n);

	// Constructor name + position
	GameObject(int _id, std::string n, glm::vec3 pos);

	// Constructor name + position + rotation
	GameObject(int _id, std::string n, glm::vec3 pos, glm::vec3 rot);

	// Constructor name + position + rotation + scale
	GameObject(int _id, std::string n, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);
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
	virtual void Update(float deltaTime);

	// Draw
	virtual void Draw();

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string message, BehaviourType type);

	// Gets behaviour of BehaviourType; Returns nullptr if doesn't exist
	Behaviour* GetBehaviour(BehaviourType type);

	// Adds a behaviour
	//void AddBehaviour(std::unique_ptr<Behaviour> behaviour);
private:
	std::vector<std::unique_ptr<Behaviour>> m_Behaviours;
};
