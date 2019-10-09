#pragma once
#include <nholmann/json.hpp>
#include <vector>
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>
#include <string>
#include <engine/debugbt/DebugLog.h>

#include "gameobjects/Object.h"
#include "gameobjects/Guard.h"
#include "gameobjects/Room.h"


using json = nlohmann::json;

class Level
{
	public:
		Level(std::string jsonFile);
		~Level();

		int levelNum;

		// Width of level ^ v
		int width;

		// Length of level <------>
		int length;

		// Starting position of the player
		int startPos;

		// Number of grids in the level (width * length)
		int numGrids;

		// Game objects
		std::vector<Object> objects;

		// Rooms
		std::vector<Room> rooms;

		// Guards (Should be std::vector<Guard> probably)
		std::vector<Guard> guards;


		void LoadLevel(std::string jsonFile);
	private:
		json _levelJSON;
};