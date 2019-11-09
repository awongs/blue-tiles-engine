#pragma once

#include <unordered_map>
#include <string>

class JointTransform;

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class KeyFrame
{
public:
	// Constructor that takes a timestamp and pose.
	KeyFrame(float timeStamp, std::unordered_map<std::string, JointTransform>& pose);

	// The start time in the animation that this keyframe represents
	float timeStamp;

	// The transformed joints/bones for this keyframe. 
	std::unordered_map<std::string, JointTransform> pose;
};