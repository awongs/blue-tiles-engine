#pragma once
#include <string>

class Object
{
	public:
		Object(int _objectid, std::string _name, int _location, float _rotation);

		int objectid;
		std::string name;
		int location;
		float rotation;
};

