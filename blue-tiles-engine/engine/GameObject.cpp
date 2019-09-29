#include "GameObject.h"
#include <algorithm>
#include <functional>

GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours)
{
	id = -1;
	name = "GameObject";
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n)
{
	id = -1;
	name = n;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n, glm::vec3 pos)
{
	id = -1;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = pos;
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}
GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n, glm::vec3 pos, glm::vec3 rot)
{
	id = -1;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = pos;
	rotation = rot;
	scale = glm::vec3(1, 1, 1);
}
GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, std::string n, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
	id = -1;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = pos;
	rotation = rot;
	scale = sca;
}

GameObject::~GameObject()
{
	
}

void GameObject::Update()
{
	// Tell each behaviour to update
	for (std::unique_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->Update();
	}

	// -- Testing Purposes --
	rotation.y += 0.01f;
}

void GameObject::Draw()
{
	// Tell each behaviour to draw
	for (std::unique_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->Draw();
	}
}

Behaviour* GameObject::GetBehaviour(BehaviourType type)
{
	for (auto& behaviour : m_Behaviours)
	{
		if (behaviour->type == type)
		{
			return behaviour.get();
		}
	}
	return nullptr;
}

bool GameObject::HandleMessage(std::string message, BehaviourType type)
{
	return false;
}