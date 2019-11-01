#pragma once

#include <engine/behaviours/Behaviour.h>

#include "../gameobjects/Tile.h"

class TileBehaviour : public Behaviour
{

public:
	// Constructor.
	TileBehaviour(TileType type);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;

	TileType GetType() const;

private:
	// This tile's type.
	// We need this to differentiate between room tiles.
	TileType m_type;
};