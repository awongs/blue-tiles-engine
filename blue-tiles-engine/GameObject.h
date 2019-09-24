#pragma once
#include <glad/glad.h>
#include <string>
#include <list>

class Behaviour;

class GameObject
{

public: 
	GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours);
	~GameObject();

	// id of GameObject
	GLuint id;

	// Name of GameObject
	std::string name;

	// Gets the behaviour of type behaviourType
	Behaviour GetBehaviour(std::string behaviourType);

	// Update
	virtual void Update();

	// Draw
	virtual void Draw();

	// Handles messages
	bool HandleMessage(std::string message);

private:
	std::list<std::unique_ptr<Behaviour>> m_Behaviours;
};
