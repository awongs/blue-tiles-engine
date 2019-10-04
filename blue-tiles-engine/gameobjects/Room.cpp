#include "Room.h"

Room::Room(int _roomid, std::vector<int> _gridUsed, std::vector<Door> _doors)
{
	roomid = _roomid;
	gridUsed.swap(_gridUsed);
	doors.swap(_doors);
}