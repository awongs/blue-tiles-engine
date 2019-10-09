#include "GameObject.h"
#include "sound/SoundManager.h"
#include <algorithm>
#include <functional>
#include "sound/Music.h"

GameObject::GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours)
{
	id = _id;
	name = n;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	for (std::unique_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->gameObject = this;
	}
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

GameObject::GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos)
{
	id = _id;
	name = n;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	for (std::unique_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->gameObject = this;
	}
	position = pos;
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}
GameObject::GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos, glm::vec3 rot)
{
	id = _id;
	name = n;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	for (std::unique_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->gameObject = this;
	}
	position = pos;
	rotation = rot;
	scale = glm::vec3(1, 1, 1);
}
GameObject::GameObject(int _id, std::string n, std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
	id = _id;
	name = n;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	for (std::unique_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->gameObject = this;
	}
	position = pos;
	rotation = rot;
	scale = sca;
}

GameObject::GameObject(int _id, std::string n)
{
	id = _id;
	name = n;
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

GameObject::GameObject(int _id, std::string n, glm::vec3 pos)
{
	id = _id;
	name = n;
	position = pos;
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

GameObject::GameObject(int _id, std::string n, glm::vec3 pos, glm::vec3 rot)
{
	id = _id;
	name = n;
	position = pos;
	rotation = rot;
	scale = glm::vec3(1, 1, 1);
}

GameObject::GameObject(int _id, std::string n, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
	id = _id;
	name = n;
	position = pos;
	rotation = rot;
	scale = sca;
}

GameObject::~GameObject()
{
	
}

void GameObject::Update(float deltaTime)
{
	// Tell each behaviour to update
	for (std::unique_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->Update(deltaTime);
	}

	// -- Testing Purposes --
	//rotation.y += 3.14f * deltaTime;
	
	// Shared pointer to a music object.
	auto music = SoundManager::getInstance().getMusic("alarm");
	//music->play();
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
		if (behaviour->GetType() == type)
		{
			return behaviour.get();
		}
	}
	return nullptr;
}

bool GameObject::HandleMessage(unsigned int senderID, std::string message, BehaviourType type)
{
	Behaviour* behav = GetBehaviour(type);

	return behav != nullptr ? behav->HandleMessage(senderID, message) : false;
}

void GameObject::AddBehaviour(Behaviour* behaviour)
{
	behaviour->SetId(this->id);
	behaviour->gameObject = this;
	m_Behaviours.push_back(std::move(std::unique_ptr<Behaviour>(behaviour)));
}
