#include "GameObject.h"

GameObject::GameObject(const std::list<std::unique_ptr<Behaviour>> behaviours)
{
	m_Behaviours = behaviours;
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

/*void GameObject::GetBehaviour(std::string behaviourType)
{
	return ;
}*/

bool GameObject::HandleMessage(std::string message)
{
	return true;
}