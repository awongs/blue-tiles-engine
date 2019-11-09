#include <pugixml/pugixml.hpp>
#include <sstream>
#include <glm/glm.hpp>

#include "Animation.h"
#include "JointTransform.h"

Animation::Animation(std::string name, float length, std::vector<KeyFrame>& keyFrames)
	: name(name)
	, length(length)
	, keyFrames(keyFrames)
{
}