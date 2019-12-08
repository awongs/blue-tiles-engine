#pragma once

#include <engine/Scene.h>
#include "../Level.h"
#include "../gameobjects/Tile.h"

class PhysicsEngine;
class GameEngine;
class UITextBehaviour;

class LevelScene : public Scene
{
public:
	LevelScene(Level* level, PhysicsEngine *physEngine, std::shared_ptr<GameEngine> gameEngine);

	// Loads the level.
	// If called again (e.g. by retrying), the previous level state is deleted.
	void LoadScene(PhysicsEngine* physEngine, GameEngine* gameEngine);

	// Updates the level.
	void Update(float deltaTime) override;
	
	// Get the tile at the given x, z-coordinate.
	TileType GetTile(unsigned int x, unsigned int z) const;

	// Get the x, z tile-coordinates from given x, z world coordinates.
	glm::ivec2 GetTileCoordFromPos(glm::vec2 worldPos) const;

	// Get the index for a tile, given its x, z tile coordinates.
	unsigned int GetTileIndexFromXZ(glm::ivec2 tilePos) const;

	// Get the number of tiles in this level.
	glm::ivec2 GetLevelSize() const;

	// Get the integer representation of the level's tiles.
	void GetTiles(std::vector<int> &output) const;

	// Set the tile type at the given x, z-tile coordinates.
	void SetTile(TileType type, unsigned int x, unsigned int z);

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

	std::shared_ptr<GameEngine> m_gameEngine;

	// The level object for this scene.
	std::shared_ptr<Level> m_level;

	// For displaying the elapsed time in the level.
	std::weak_ptr<UITextBehaviour> m_levelTimer;
	float m_elapsedTime { 0 };
};