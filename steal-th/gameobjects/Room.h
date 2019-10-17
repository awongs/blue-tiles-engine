#pragma once

#include <vector>

struct Room
{
	std::vector<int> tileIndices;
	std::vector<int> redDoors;
	std::vector<int> blueDoors;
	std::vector<int> greenDoors;
	std::vector<int> walls;
};