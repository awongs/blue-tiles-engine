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

	levelNum	= _levelJSON["info"]["level"];
	width		= _levelJSON["info"]["width"];
	length		= _levelJSON["info"]["length"];
	numGrids	= width * length;
	std::vector<GameObject> temp;
	objects.swap(temp);
	for (auto room : _levelJSON["rooms"])
	{
		
	}
}