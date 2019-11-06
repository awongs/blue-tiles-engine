#include "Animation.h"

Animation::Animation(float length, std::vector<KeyFrame>& keyFrames)
	: length(length)
	, keyFrames(keyFrames)
{
}
