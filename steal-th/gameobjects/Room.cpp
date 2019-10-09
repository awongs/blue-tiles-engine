#include "Room.h"

Room::Room(int _roomid, std::vector<int> _gridUsed, std::vector<Door> _doors, std::vector<Wall> _walls)
{
	roomid = _roomid;
	gridUsed.swap(_gridUsed);
	doors.swap(_doors);
	walls.swap(_walls);
}