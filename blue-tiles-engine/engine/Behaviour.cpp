#include "Behaviour.h"

Behaviour::Behaviour(BehaviourType _type)
{
	type = _type;
}

Behaviour::~Behaviour()
{

}

void Behaviour::Update()
{

}

void Behaviour::Draw()
{

}

bool Behaviour::HandleMessage(std::string message)
{
	return false;
}