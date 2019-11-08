#include <pugixml/pugixml.hpp>
#include <sstream>

#include "Animation.h"

Animation::Animation(std::string name, float length, std::vector<KeyFrame>& keyFrames)
	: name(name)
	, length(length)
	, keyFrames(keyFrames)
{
}

std::shared_ptr<Animation> Animation::CreateAnimationFromFile(std::string filePath)
{
	pugi::xml_document aniFile;

	// Try to open the file.
	if (!aniFile.load_file(filePath.c_str()))
	{
		return nullptr;
	}

	// Parse information for all joints.
	pugi::xml_node animations = aniFile.child("COLLADA")
		.child("library_animations");

	std::vector<std::vector<float>> timeStamps; // TODO: Probably don't need to store every joint's timestamps
	std::unordered_map<std::string, std::vector<glm::mat4>> jointTransformMatrices;

	for (pugi::xml_node& childNode : animations.children("animation"))
	{
		std::string childName = childNode.attribute("name").as_string();

		pugi::xml_node inputNode;
		pugi::xml_node outputNode;

		// Find the input and output nodes for this animation.
		for (auto it = childNode.children().begin(); it != childNode.children().end(); it++)
		{
			std::string id = it->attribute("id").as_string();
			if (id.find("animation-input") != std::string::npos)
			{
				inputNode = *it;
				continue;
			}

			if (id.find("animation-output") != std::string::npos)
			{
				outputNode = *it;
			}
		}

		// Get keyframe and transformation strings.
		std::string keyframeStr = inputNode.child("float_array").text().as_string();
		std::string transformStr = outputNode.child("float_array").text().as_string();

		// Parse all matrix values as floats. TODO: Code duplication
		std::vector<float> matValues;
		std::istringstream iss(transformStr);
		while (iss)
		{
			std::string floatStr;
			iss >> floatStr;
			if (floatStr.length() == 0)
			{
				continue;
			}

			// Set float value in the current joint's bind transform.
			matValues.push_back(stof(floatStr));
		}

		// Parse all keyframe times as floats. TODO: Code duplication
		std::vector<float> timeValues;
		iss = std::istringstream(keyframeStr);
		while (iss)
		{
			std::string timeStr;
			iss >> timeStr;
			if (timeStr.length() == 0)
			{
				continue;
			}

			// Set float value in the current joint's bind transform.
			timeValues.push_back(stof(timeStr));
		}

		std::vector<glm::mat4> transformMatrices;

		// Traverse through float array and construct 4x4 matrices.
		for (size_t i = 0; i < matValues.size() / 16; i++)
		{
			glm::mat4 transformMatrix;
			for (int j = 0; j < 16; j++)
			{
				transformMatrix[j % 4][j / 4] = matValues[(i * 16) + j];
			}
			transformMatrices.push_back(transformMatrix);
		}

		// Append to lists for this child node.
		timeStamps.push_back(timeValues);
		jointTransformMatrices.insert(std::pair<std::string, std::vector<glm::mat4>>(childName, transformMatrices));
	}

	std::vector<KeyFrame> keyFrames;
	float length = timeStamps[0].back(); // Note: Assuming all joint animations have the same timestamps
	
	for (int i = 0; i < timeStamps[0].size(); i++)
	{
		// Get all transforms for this joint's keyframes.
		std::unordered_map<std::string, JointTransform> jointTransforms;
		for (auto& jtm : jointTransformMatrices)
		{
			jointTransforms.insert(std::pair<std::string, JointTransform>(jtm.first, JointTransform(jtm.second[i])));
		}

		keyFrames.push_back(KeyFrame(timeStamps[0][i], jointTransforms));
	}

	// Extract animation file name without the extention.
	int firstIndex = filePath.find_last_of("/") + 1;
	int lastIndex = filePath.find_last_of(".");
	std::string animationName = filePath.substr(firstIndex, lastIndex - firstIndex);

	return std::make_shared<Animation>(animationName, length, keyFrames);
}
