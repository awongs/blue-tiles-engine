#include "Door.h"

Door::Door(int _id, int _loc, std::string _key, std::string _facing, bool _exit)
{
	doorid = _id;
	location = _loc;
	keyRequired = _key;
	facing = _facing;
	exit = _exit;
}