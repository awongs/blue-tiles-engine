#include "KeyFrame.h"

KeyFrame::KeyFrame(float timeStamp, std::unordered_map<std::string, JointTransform>& pose)
	: timeStamp(timeStamp)
	, pose(pose)
{
}
