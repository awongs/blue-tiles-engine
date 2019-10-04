#pragma once
#include <engine/GameObject.h>

class Guard
{
	public:
		Guard(int _guardid, int _location, float _range, float _rotAgnle, float _rotFrom, float _rotTo, float _interval);

		int guardid;
		int location;
		float range;
		float rotAngle;
		float rotFrom;
		float rotTo;
		float interval;
};