#pragma once
#include <nholmann/json.hpp>
#include <vector>
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>
#include <string>
#include "GameObject.h"
#include "../gameobjects/Guard.h"
#include "../gameobjects/Room.h"
#include "debugbt/DebugLog.h"
using json = nlohmann::json;

class Level
{
	public:
		Level(std::string jsonFile);
		~Level();

		int levelNum;

		// Width of level ^ v
		float width;

		// Length of level <------>
		float length;

		// Number of grids in the level (width * length)
		float numGrids;

		// Game objects
		std::vector<GameObject> objects;

		// Rooms
		std::vector<Room> rooms;

		// Guards (Should be std::vector<Guard> probably)
		std::vector<Guard> guards;


		void LoadLevel(std::string jsonFile);
	private:
		json _levelJSON;
};