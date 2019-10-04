#pragma once
#include "Door.h"
#include <vector>

class Room
{
	public:
		Room(int _roomid, std::vector<int> _gridUsed, std::vector<Door> _doors);

		int roomid;
		std::vector<int> gridUsed;
		std::vector<Door> doors;
};