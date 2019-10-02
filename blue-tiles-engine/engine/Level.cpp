#include "Level.h"

Level::Level(std::string jsonFile) {
	LoadLevel(jsonFile);
}

Level::~Level() {

}

void Level::LoadLevel(std::string jsonFile)
{
	// Load level file
	std::ifstream input("../Assets/levels/" + jsonFile + ".json");

	// If you see a red squiggly, its an IntelliSense problem. It compiles fine.
	_levelJSON = json::parse(input);

	levelNum	= _levelJSON["info"].value("level", -1);
	width		= _levelJSON["info"].value("width", -1);
	length		= _levelJSON["info"].value("length", -1);
	numGrids	= width * length;

	// To clear objects vector
	std::vector<Object> temp;
	objects.swap(temp);
	for (auto object : _levelJSON["objects"])
	{
		objects.push_back(*new Object(
			object.value("objectid", -1),
			object["name"],
			object.value("location", -1),
			object.value("rotation", 0.0)
		));
	}

	// To clear rooms vector
	std::vector<Room> temp2;
	rooms.swap(temp2);
	for (auto room : _levelJSON["rooms"])
	{
		std::vector<Door> doors;
		for (auto door : room["doors"]) {
			doors.push_back(*new Door(
				door.value("doorid", -1),
				door.value("location", -1),
				door.value("keyRequired", ""),
				door.value("facing", ""),
				door.value("exit", false)
			)
			);
		}

		std::vector<int> gridNums;
		for (auto num : room["gridUsed"]) {
			gridNums.push_back(num);
		}
		rooms.push_back(*new Room(
			room.value("roomid", -1),
			gridNums,
			doors
		)
		);
	}
}