#pragma once

struct Guard
{
	int guardId{ -1 };
	unsigned int tileX{ 0 }, tileZ{ 0 };
	unsigned int tileViewDistance{ 0 };
	unsigned int tileViewRadius{ 0 };
	float rotAngle{ 0.f };
	vector<std::string> movement;
};