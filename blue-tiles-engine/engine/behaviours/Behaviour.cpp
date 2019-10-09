#include "Behaviour.h"

Behaviour::Behaviour(BehaviourType type) :
	 m_type(type)
{

}

Behaviour::~Behaviour()
{

}

BehaviourType Behaviour::GetType() const
{
	return m_type;
}