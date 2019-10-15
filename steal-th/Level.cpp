#include "Level.h"

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

	levelNum = _levelJSON["info"].value("level", -1);
	width = _levelJSON["info"].value("width", -1);
	length = _levelJSON["info"].value("length", -1);
	startPos = _levelJSON["info"].value("startPos", -1);
	numGrids = width * length;

	// Create the level's objects from the JSON file.
	m_objects.clear();
	for (const auto &object : _levelJSON["objects"])
	{
		Object thisObj;
		thisObj.objectid = object.value("objectid", -1);
		thisObj.name = object["name"];
		thisObj.location = object.value("location", -1);
		thisObj.rotation = object.value("rotation", 0.f);

		m_objects.push_back(thisObj);
	}

	// Create the level's rooms from the JSON file.
	m_rooms.clear();
	for (const auto &room : _levelJSON["rooms"])
	{
		std::vector<Door> doors;
		for (const auto &door : room["doors"])
		{
			Door thisDoor;
			thisDoor.doorid = door.value("doorid", -1);
			thisDoor.location = door.value("location", -1);
			thisDoor.keyRequired = door.value("keyRequired", "");
			thisDoor.facing = door.value("facing", "");
			thisDoor.exit = door.value("exit", false);

			doors.push_back(thisDoor);
		}

		std::vector<Wall> walls;
		for (const auto &wall : room["walls"])
		{
			Wall thisWall;
			thisWall.wallid = wall.value("wallid", -1);
			thisWall.location = wall.value("location", -1);
			thisWall.facing = wall.value("facing", "");

			walls.push_back(thisWall);
		}

		std::vector<int> gridNums;
		for (const auto &num : room["gridUsed"])
		{
			gridNums.push_back(num);
		}

		Room thisRoom;
		thisRoom.roomid = room.value("roomid", -1);
		thisRoom.gridUsed = gridNums;
		thisRoom.doors = doors;
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