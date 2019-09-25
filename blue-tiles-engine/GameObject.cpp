#include "GameObject.h"
#include <algorithm>
#include <functional>

GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours)
{
	id = -1;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos)
{
	id = -1;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = pos;
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}
GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos, glm::vec3 rot)
{
	id = -1;
	std::move(begin(behaviours), end(behaviours), std::inserter(m_Behaviours, end(m_Behaviours)));
	position = pos;
	rotation = rot;
	scale = glm::vec3(1, 1, 1);
}
GameObject::GameObject(std::vector<std::unique_ptr<Behaviour>>& behaviours, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
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

}

void GameObject::Draw()
{

}

/*std::unique_ptr<Behaviour> GameObject::GetBehaviour(BehaviourType type)
{
	if(std::find_if(m_Behaviours.front, m_Behaviours.end, std::bind(TypeComparison, std::placeholders::_1, type) == m_Behaviours.end))
	{
		return NULL;
	}
	auto found = std::find_if(m_Behaviours.front, m_Behaviours.end, std::bind(TypeComparison, std::placeholders::_1, type));
	return found;
}*/

bool GameObject::HandleMessage(std::string message, BehaviourType type)
{
	return false;
}

/*bool TypeComparison(Behaviour& behaviour, BehaviourType type)
{
	if (behaviour.type == type)
		return true;
	else
		return false;
}*/