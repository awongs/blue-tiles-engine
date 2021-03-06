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

using json = nlohmann::json;

class Level
{
	public:
		Level(std::string jsonFile);
		~Level();

		unsigned int m_levelNum{ 0 };

		// The number of tiles in the level for each axis.
		unsigned int m_sizeX{ 0 }, m_sizeZ{ 0 };

		// The tile position that the player starts on.
		unsigned int m_playerSpawnX{ 0 }, m_playerSpawnZ{ 0 };

		// Hold all level objects.
		std::vector<Object> m_objects;

		// Hold the types of each tile on this level.
		std::vector<int> m_layout;

		// Hold all guards on this level.
		std::vector<Guard> m_guards;

		void LoadLevel(std::string jsonFile);

	private:
		json _levelJSON;
};