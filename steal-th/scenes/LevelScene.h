#pragma once


#include "../blue-tiles-engine/engine/Scene.h"
#include "../Level.h"
#include "../gameobjects/Tile.h"

class PhysicsEngine;

class LevelScene : public Scene
{
public:
	LevelScene(Level* level, PhysicsEngine *physEngine);
	
	// Get the tile at the given x, z-coordinate.
	TileType GetTile(unsigned int x, unsigned int z) const;

	// Get the x, z tile-coordinates from given x, z world coordinates.
	glm::ivec2 GetTileCoordFromPos(glm::vec2 worldPos) const;

	// Get the index for a tile, given its x, z tile coordinates.
	unsigned int GetTileIndexFromXZ(glm::ivec2 tilePos) const;

	// Get the number of tiles in this level.
	glm::ivec2 GetLevelSize() const;

	// The size of a tile in world units.
	static const float TILE_SIZE;

private:
	// Add a tile to the level at the given x, z-tile coordinates.
	void AddTile(TileType type, unsigned int x, unsigned int z);

	// Add a tile to the level at the given tile index.
	void AddTile(TileType type, unsigned int tileIndex);

	// Hold a raw pointer to the GameEngine's PhysicsEngine.
	// This will be needed for registering PhysicsObjects when PhysicsBehaviours
	// are added to GameObjects.
	PhysicsEngine *m_physEngine;

	// The tiles in the level
	std::vector<TileType> m_tiles;

	// The size of the level in tiles.
	glm::ivec2 m_levelSize;
};