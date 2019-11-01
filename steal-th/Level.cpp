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
	m_sizeX = _levelJSON["info"]["size"].value("x", 0);
	m_sizeZ = _levelJSON["info"]["size"].value("z", 0);
	m_playerSpawnX = _levelJSON["info"]["playerSpawn"].value("x", 0);
	m_playerSpawnZ = _levelJSON["info"]["playerSpawn"].value("z", 0);

	// Create the level's objects from the JSON file.
	m_objects.clear();
	for (const auto &object : _levelJSON["objects"])
	{
		Object thisObj;
		thisObj.tileX = object["position"].value("x", 0);
		thisObj.tileZ = object["position"].value("z", 0);
		thisObj.rotation = object.value("rotation", 0.f);

		// Convert string value from JSON to its corresponding enum.
		// Default to NONE if nothing matches.
		std::string objStr{ object.value("type", "") };
		auto it{ m_objStrToType.find(objStr) };
		thisObj.type = (it == m_objStrToType.end() ? ObjectType::NONE : it->second);

		m_objects.push_back(thisObj);
	}

	// Create the level's tile layout from the JSON file.
	for (const auto &tile : _levelJSON["layout"])
	{
		m_layout.push_back(tile);
	}

	// Create the level's guards from the JSON file.
	m_guards.clear();
	for (const auto &guard : _levelJSON["guards"])
	{
		Guard thisGuard;
		thisGuard.guardId = guard.value("guardId", -1);
		thisGuard.tileX = guard["position"].value("x", 0);
		thisGuard.tileZ = guard["position"].value("z", 0);
		thisGuard.tileViewDistance = guard.value("tileViewDistance", 0);
		thisGuard.tileViewRadius = guard.value("tileViewRadius", 0);
		thisGuard.rotAngle = guard["rotation"].value("angle", 0.f);
		thisGuard.rotFrom = guard["rotation"].value("rotateFrom", 0.f);
		thisGuard.rotTo = guard["rotation"].value("rotateTo", 0.f);
		thisGuard.interval = guard["rotation"].value("interval", 0.f);

		m_guards.push_back(thisGuard);
	}
}