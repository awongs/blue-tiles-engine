#include "ObjectBehaviour.h"

ObjectBehaviour::ObjectBehaviour(ObjectType type) :
	Behaviour(BehaviourType::ObjectBehaviour), m_type(type)
{
}

void ObjectBehaviour::Update(float deltaTime)
{
}

void ObjectBehaviour::Draw(Shader& shader)
{
}

bool ObjectBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

ObjectType ObjectBehaviour::GetType() const
{
	return m_type;
}
