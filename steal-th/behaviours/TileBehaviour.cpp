#include "TileBehaviour.h"

TileBehaviour::TileBehaviour(TileType type) :
	Behaviour(BehaviourType::TileBehaviour), m_type(type)
{
}

void TileBehaviour::Update(float deltaTime)
{
}

void TileBehaviour::Draw(Shader& shader)
{
}

bool TileBehaviour::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

TileType TileBehaviour::GetType() const
{
	return m_type;
}
