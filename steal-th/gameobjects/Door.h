#pragma once
#include <engine/GameObject.h>

class Door
{
	public:
		Door(int _id, int _loc, std::string _key, std::string _facing, bool _exit);

		int doorid;
		int location;
		std::string keyRequired;
		std::string facing;
		bool exit;
};

