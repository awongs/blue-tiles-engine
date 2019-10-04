#include "Object.h"

Object::Object(int _objectid, std::string _name, int _location, float _rotation)
{
	objectid = _objectid;
	name = _name;
	location = _location;
	rotation = _rotation;
}