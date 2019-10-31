#pragma once

// Define different types of objects that can appear on the level.
enum class ObjectType
{
	NONE,
	RED_KEY,
	BLUE_KEY,
	GREEN_KEY,
	OBJECTIVE_ITEM
};

struct Object
{
	ObjectType type;
	int tileIndex;
	float rotation;
};