#include "Behaviour.h"

Behaviour::Behaviour(GLuint gameObjectId, BehaviourType type) :
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
