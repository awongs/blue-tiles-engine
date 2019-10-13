#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>
struct Tile
{
	public:
		enum class TileEdgeType
		{
			NONE,
			WALL,
			DOOR
		};

		enum class ObjectType
		{
			NONE,
			BLOCK,
			RED_KEY,
			BLUE_KEY,
			GREEN_KEY
		};

		// Tile number
		int num = -1;

		// Center position of the tile (in actual coordinates)
		glm::vec3 center = glm::vec3(0, 0, 0);

		// Start of tile on X axis
		double startX = 0.0;

		// End of tile on X axis
		double endX = 0.0;

		// Start of tile on Z axis
		double startZ = 0.0;

		// End of tile on Z axis
		double endZ = 0.0;

		// What's above the tile? (none, wall, door)
		TileEdgeType up = TileEdgeType::NONE;

		// What's below the tile? (none, wall, door)
		TileEdgeType down = TileEdgeType::NONE;

		// What's to the left of the tile? (none, wall, door)
		TileEdgeType left = TileEdgeType::NONE;

		// What's to the right of the tile? (none, wall, door)
		TileEdgeType right = TileEdgeType::NONE;

		//What's on the tile?
		ObjectType on = ObjectType::NONE;

		// Tile up from this tile
		Tile* tileUp;

		// Tile right from this tile
		Tile* tileRight;

		// Tile left from this tile
		Tile* tileLeft;

		// Tile down from this tile
		Tile* tileDown;
};