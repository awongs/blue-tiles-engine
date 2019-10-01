#pragma once
#include <nholmann/json.hpp>
#include <vector>
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>
#include <string>
#include "GameObject.h"
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
		vector<GameObject> objects;

		// Guards (Should be std::vector<Guard> probably)
		vector<GameObject> guards;


		void LoadLevel(std::string jsonFile);
	private:
		json _levelJSON;
};