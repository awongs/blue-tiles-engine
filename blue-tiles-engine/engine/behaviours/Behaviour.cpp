#include "Behaviour.h"

Behaviour::Behaviour(BehaviourType _type)
{
	type = _type;
}

Behaviour::~Behaviour()
{

}

void Behaviour::Update(float deltaTime)
{

}

void Behaviour::Draw()
{

}

bool Behaviour::HandleMessage(unsigned int senderID, std::string message)
{
	return false;
}