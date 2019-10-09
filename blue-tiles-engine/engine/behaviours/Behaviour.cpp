#include "Behaviour.h"

Behaviour::Behaviour(int gameObjectId, BehaviourType type) :
	m_gameObjectId(gameObjectId), m_type(type)
{

}

Behaviour::~Behaviour()
{

}

BehaviourType Behaviour::GetType() const
{
	return m_type;
}

void Behaviour::SetId(int _id)
{
	m_gameObjectId = _id;
}