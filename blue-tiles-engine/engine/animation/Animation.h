#pragma once

#include <vector>
#include "KeyFrame.h"

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class Animation
{
public:
	// Constructor.
	Animation(float length, std::vector<KeyFrame>& keyFrames);

	// The length of the animation in seconds.
	float length;

	// List of keyframes for the animation.
	std::vector<KeyFrame> keyFrames;
};