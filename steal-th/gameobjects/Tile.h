#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>
struct Tile
{
	public:
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

		//What's above the tile? (none, wall, door)
		std::string up = "none";

		//What's below the tile? (none, wall, door)
		std::string down = "none";

		//What's to the left of the tile? (none, wall, door)
		std::string left = "none";

		//What's to the right of the tile? (none, wall, door)
		std::string right = "none";

		//What's on the tile? (Name of object... i.e. "none, "block", "key1", "key2")
		std::string on = "none";

		// Tile up from this tile
		Tile* tileUp;

		// Tile right from this tile
		Tile* tileRight;

		// Tile left from this tile
		Tile* tileLeft;

		// Tile down from this tile
		Tile* tileDown;
};