#include "Behaviour.h"

Behaviour::Behaviour(BehaviourType type) 
	: m_type(type)
	, gameObject(nullptr)
{

}

Behaviour::~Behaviour()
{

}

void Behaviour::OnAttach(GameObject& gameObject)
{
}

BehaviourType Behaviour::GetType() const
{
	return m_type;
}