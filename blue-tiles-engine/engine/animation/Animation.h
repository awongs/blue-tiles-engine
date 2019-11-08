#pragma once

#include <vector>
#include <memory>
#include <string>
#include "KeyFrame.h"

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class Animation
{
public:
	// Constructor.
	Animation(std::string name, float length, std::vector<KeyFrame>& keyFrames);

	// Creates an animation from a collada file.
	// TODO: Caching maybe useful?
	static std::shared_ptr<Animation> CreateAnimationFromFile(std::string filePath);

	// The name of the animation file.
	std::string name;

	// The length of the animation in seconds.
	float length;

	// List of keyframes for the animation.
	std::vector<KeyFrame> keyFrames;
};