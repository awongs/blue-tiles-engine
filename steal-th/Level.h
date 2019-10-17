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

		unsigned int m_levelNum{ 0 };

		// The number of tiles in the level for each axis.
		unsigned int m_sizeX{ 0 }, m_sizeZ{ 0 };

		// The index of the tile that the player starts on.
		unsigned int m_startTileIndex{ 0 };

		// Game objects
		std::vector<Object> m_objects;

		// Rooms
		std::vector<Room> m_rooms;

		// Guards (Should be std::vector<Guard> probably)
		std::vector<Guard> m_guards;


		void LoadLevel(std::string jsonFile);
	private:
		json _levelJSON;
};