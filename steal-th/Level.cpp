#include "Level.h"
#include <unordered_map>

namespace
{
	std::unordered_map<std::string, ObjectType> m_objStrToType
	{
		{ "red_key", ObjectType::RED_KEY },
		{ "blue_key", ObjectType::BLUE_KEY },
		{ "green_key", ObjectType::GREEN_KEY },
		{ "objective_item", ObjectType::OBJECTIVE_ITEM }
	};
}

Level::Level(std::string jsonFile)
{
	LoadLevel(jsonFile);
}

Level::~Level()
{

}

void Level::LoadLevel(std::string jsonFile)
{
	// Load level file
	std::ifstream input("../Assets/levels/" + jsonFile + ".json");

	// If you see a red squiggly, its an IntelliSense problem. It compiles fine.
	_levelJSON = json::parse(input);

	m_levelNum = _levelJSON["info"].value("level", 0);
	m_sizeX = _levelJSON["info"].value("sizeX", 0);
	m_sizeZ = _levelJSON["info"].value("sizeZ", 0);
	m_startTileIndex = _levelJSON["info"].value("startTileIndex", 0);

	// Create the level's objects from the JSON file.
	m_objects.clear();
	for (const auto &object : _levelJSON["objects"])
	{
		Object thisObj;
		thisObj.tileIndex = object.value("tileIndex", 0);
		thisObj.rotation = object.value("rotation", 0.f);

		// Convert string value from JSON to its corresponding enum.
		// Default to NONE if nothing matches.
		std::string objStr{ object.value("type", "") };
		auto it{ m_objStrToType.find(objStr) };
		thisObj.type = (it == m_objStrToType.end() ? ObjectType::NONE : it->second);

		m_objects.push_back(thisObj);
	}

	// Create the level's rooms from the JSON file.
	m_rooms.clear();
	for (const auto &room : _levelJSON["rooms"])
	{
		std::vector<int> redDoors;
		for (const auto& tileIndex : room["redDoors"])
		{
			redDoors.push_back(tileIndex);
		}

		std::vector<int> greenDoors;
		for (const auto& tileIndex : room["greenDoors"])
		{
			greenDoors.push_back(tileIndex);
		}

		std::vector<int> blueDoors;
		for (const auto& tileIndex : room["blueDoors"])
		{
			blueDoors.push_back(tileIndex);
		}

		std::vector<int> walls;
		for (const auto &tileIndex : room["walls"])
		{
			walls.push_back(tileIndex);
		}

		std::vector<int> tiles;
		for (const auto & tileIndex : room["tileIndices"])
		{
			tiles.push_back(tileIndex);
		}

		Room thisRoom;
		thisRoom.tileIndices = tiles;
		thisRoom.redDoors = redDoors;
		thisRoom.blueDoors = blueDoors;
		thisRoom.greenDoors = greenDoors;
		thisRoom.walls = walls;

		m_rooms.push_back(thisRoom);
	}

	// Create the level's guards from the JSON file.
	m_guards.clear();
	for (const auto &guard : _levelJSON["guards"])
	{
		Guard thisGuard;
		thisGuard.guardid = guard.value("guardid", -1);
		thisGuard.location = guard.value("location", -1);
		thisGuard.range = guard.value("range", 0.f);
		thisGuard.rotAngle = guard["rotation"].value("angle", 0.f);
		thisGuard.rotFrom = guard["rotation"].value("rotateFrom", 0.f);
		thisGuard.rotTo = guard["rotation"].value("rotateTo", 0.f);
		thisGuard.interval = guard["rotation"].value("interval", 0.f);

		m_guards.push_back(thisGuard);
	}
}