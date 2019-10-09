#pragma once
#include <engine/GameObject.h>

class Wall
{
	public:
		Wall(int _id, int _loc, std::string _facing);

		int wallid;
		int location;
		std::string facing;
};

