#include "GameObject.h"
#include <algorithm>
#include <functional>

GameObject::GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours)
{
	m_Behaviours.merge(behaviours);
}

GameObject::GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours, glm::vec3 pos)
{
	m_Behaviours = behaviours;
	position = pos;
}
GameObject::GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours, glm::vec3 pos, glm::vec3 rot)
{
	m_Behaviours = behaviours;
	position = pos;
	rotation = rot;
}
GameObject::GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
	m_Behaviours = behaviours;
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