#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// Code referenced from: https://www.youtube.com/watch?v=f3Cr8Yx3GGA
class Joint
{
public:
	// Constructor, takes a joint index and name.
	Joint(int index, std::string& name);

	// Adds a child to the list of child joints.
	void AddChild(const std::shared_ptr<Joint> child);

	// Calculates the inverse bind transform.
	// TODO: Could read this in from the .dae file instead.
	void CalculateInverseBindTransform(const glm::mat4 parentBindTransform);

	// Name of the joint.
	std::string name;

	// List of child joints.
	std::vector<std::shared_ptr<Joint>> children;

	// Transformation from joint to mesh origin.
	// Bind means relative to parent joint.
	glm::mat4 inverseBindTransform;
	
	// Original transformation of the joint relative to parent joint.
	glm::mat4 localBindTransform;

	// Transformation from bind position to animation position.
	glm::mat4 animatedTransform;

	// The index of the joint in the skeletal hierarchy.
	int index;
};