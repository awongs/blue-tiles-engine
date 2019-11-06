#pragma once

#include <unordered_map>
#include <string>

#include "JointTransform.h"

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class KeyFrame
{
public:
	KeyFrame(float timeStamp, std::unordered_map<std::string, JointTransform>& pose);

	// The start time in the animation that this keyframe represents
	float timeStamp;

	// The transformed joints/bones for this keyframe. 
	std::unordered_map<std::string, JointTransform> pose;
};