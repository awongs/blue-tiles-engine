#include "Guard.h"

Guard::Guard(int _guardid, int _location, float _range, float _rotAngle, float _rotFrom, float _rotTo, float _interval)
{
	guardid = _guardid;
	location = _location;
	range = _range;
	rotAngle = _rotAngle;
	rotFrom = _rotFrom;
	rotTo = _rotTo;
	interval = _interval;
}