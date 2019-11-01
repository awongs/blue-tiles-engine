#pragma once

struct Guard
{
	int guardId{ -1 };
	unsigned int tileX{ 0 }, tileZ{ 0 };
	unsigned int tileViewDistance{ 0 };
	unsigned int tileViewRadius{ 0 };
	float rotAngle{ 0.f };
	float rotFrom{ 0.f };
	float rotTo{ 0.f };
	float interval{ 0.f };
};