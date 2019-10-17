#pragma once

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