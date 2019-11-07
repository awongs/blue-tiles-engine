#include "Joint.h"

Joint::Joint(int index, std::string& name)
	: m_index(index)
	, name(name)
{
	localBindTransform = glm::mat4(1);
	animatedTransform = glm::mat4(1);
	inverseBindTransform = glm::mat4(1);
}

void Joint::AddChild(const std::shared_ptr<Joint> child)
{
	children.push_back(child);
}

void Joint::CalculateInverseBindTransform(const glm::mat4& parentBindTransform)
{
	glm::mat4 bindTransform = parentBindTransform * localBindTransform;
	bindTransform = glm::inverse(bindTransform);

	// Repeat for all child joints.
	for (std::shared_ptr<Joint>& child : children)
	{
		child->CalculateInverseBindTransform(bindTransform);
	}
}
