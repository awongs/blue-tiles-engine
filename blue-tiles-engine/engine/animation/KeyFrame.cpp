#include "KeyFrame.h"
#include "JointTransform.h"

KeyFrame::KeyFrame(float timeStamp, std::unordered_map<std::string, JointTransform>& pose)
	: timeStamp(timeStamp)
	, pose(pose)
{
}
