#include <pugixml/pugixml.hpp>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <stack>

#include "AnimatedMesh.h"
#include "Animation.h"
#include "../../util/FileManager.h"

#include "../debugbt/DebugLog.h"
#include "../GameObject.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../input/Input.h"

const std::string AnimatedMesh::MODEL_MATRIX = "model";

AnimatedMesh::AnimatedMesh(std::string objPath, std::string skeletonPath)
	: Behaviour(BehaviourType::AnimatedMesh)
	, rootJoint(rootJoint)
	, jointCount(jointCount)
{
	meshmanager::LoadObjWithSkeleton(objPath, skeletonPath, m_vertices, m_indices);
	parseJointHierarchy(skeletonPath);

	SetupBuffers();
}

void AnimatedMesh::SetTexture(std::string texturePath)
{
	m_texture = FileManager::LoadTexture(texturePath);
}

void AnimatedMesh::SetupBuffers()
{
	if (m_vertices.empty() || m_indices.empty())
	{
		DebugLog::Warn("AnimatedMesh created with no vertices or indices");
		return;
	}

	// Generate the buffers
	glGenBuffers(1, &m_vertexBufferObjectID);
	glGenBuffers(1, &m_indicesBufferObjectID);

	// Allocate data into the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(AnimatedVertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// Generate and setup the vertex array object
	glGenVertexArrays(1, &m_vertexArrayObjectID);
	glBindVertexArray(m_vertexArrayObjectID);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	// Normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glEnableVertexAttribArray(2);

	// Joint Ids
	glVertexAttribIPointer(3, 3, GL_INT, sizeof(AnimatedVertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);

	// Joint weights
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::ivec3)));
	glEnableVertexAttribArray(4);

	if (glGetError() != GL_NO_ERROR)
	{
		DebugLog::Warn("Error in setting up AnimatedMesh");
	}

	// Unbind everything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AnimatedMesh::BindJointTransforms(GLuint uniformBuffer)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	std::vector<glm::mat4> jointTransforms = getJointTransforms();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * jointTransforms.size(), &jointTransforms.front());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<Joint> AnimatedMesh::GetJointByName(const std::string& jointName)
{
	for (std::shared_ptr<Joint> joint : m_joints)
	{
		if (joint->name == jointName)
		{
			return joint;
		}
	}

	DebugLog::Error("Couldn't find a joint named: " + jointName);
	return nullptr;
}

void AnimatedMesh::Update(float deltaTime)
{
	// Update transform matrix
	gameObject->UpdateTransformMatrix();
}

void AnimatedMesh::Draw(Shader& shader)
{
	// Bind this mesh's buffers
	glBindVertexArray(m_vertexArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);

	// Set model matrix in shader
	shader.SetUniformMatrix4fv(MODEL_MATRIX, gameObject->GetTransformMatrix());
	
	// Bind the texture
	if (m_texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
	}

	// Draw the mesh
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

bool AnimatedMesh::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void AnimatedMesh::OnCollisionStay(GLuint other)
{
}

bool AnimatedMesh::parseJointHierarchy(std::string skeletonPath)
{
	pugi::xml_document skeFile;

	// Try to open the file.
	if (!skeFile.load_file(skeletonPath.c_str()))
	{
		return false;
	}

	std::vector<std::shared_ptr<Joint>> joints;

	// Get controller name.
	std::string controllerName = skeFile.child("COLLADA")
		.child("library_controllers")
		.child("controller")
		.attribute("id").as_string();
	std::string controllerJointsStr = controllerName + "-Joints";
	std::string controllerMatricesStr = controllerName + "-Matrices";

	// Get total number of joints.
	jointCount = skeFile.child("COLLADA")
		.child("library_controllers")
		.child("controller")
		.child("skin")
		.find_child_by_attribute("id", controllerJointsStr.c_str())
		.child("Name_array").attribute("count").as_int();

	// Parse information for joint names.
	std::string jointNameArray = skeFile.child("COLLADA")
		.child("library_controllers")
		.child("controller")
		.child("skin")
		.find_child_by_attribute("id", controllerJointsStr.c_str())
		.child("Name_array").text().as_string();

	// Parse information for joint matrices.
	std::string jointMatricesArray = skeFile.child("COLLADA")
		.child("library_controllers")
		.child("controller")
		.child("skin")
		.find_child_by_attribute("id", controllerMatricesStr.c_str())
		.child("float_array").text().as_string();

	std::vector<float> jointMatrices;
	std::istringstream iss(jointMatricesArray);
	int jointCount = 0;
	while (iss)
	{
		std::string floatStr;
		iss >> floatStr;
		if (floatStr.length() == 0)
		{
			continue;
		}

		jointMatrices.push_back(stof(floatStr));
	}

	iss = std::istringstream(jointNameArray);
	while (iss)
	{
		std::string jointStr;
		iss >> jointStr;
		if (jointStr.length() == 0)
		{
			continue;
		}

		std::shared_ptr<Joint> joint = std::make_shared<Joint>(jointCount, jointStr);

		for (int i = 0; i < 16; i++)
		{
			joint->inverseBindTransform[i % 4][i / 4] = jointMatrices[((size_t)jointCount * 16) + i];
		}

		joints.push_back(joint);
		jointCount++;
	}

	// Find the root joint.
	pugi::xml_node firstJoint = skeFile.child("COLLADA")
		.child("library_visual_scenes")
		.child("visual_scene")
		.find_child_by_attribute("type", "JOINT");

	// Find the corresponding joint for this node.
	std::string nodeName = firstJoint.attribute("name").as_string();

	// TODO: Messy code duplication
	auto it = find_if(joints.begin(), joints.end(), [&](std::shared_ptr<Joint>& joint) { return joint->name == nodeName; });
	if (it == joints.end())
	{
		DebugLog::Warn("Couldn't find a joint with the same name. .dae file may be incorrect.");
		return false;
	}

	// Set root joint.
	rootJoint = *it;

	// Instantate node stack.
	std::stack<pugi::xml_node> nodeStack;
	nodeStack.push(firstJoint);

	while (!nodeStack.empty())
	{
		// Pop node from the stack.
		pugi::xml_node currentNode = nodeStack.top();
		nodeStack.pop();

		// Find the corresponding joint for this node.
		nodeName = currentNode.attribute("name").as_string();

		// TODO: Messy code duplication
		auto it = find_if(joints.begin(), joints.end(), [&](std::shared_ptr<Joint>& joint) { return joint->name == nodeName; });
		if (it == joints.end())
		{
			DebugLog::Warn("Couldn't find a joint with the same name. .dae file may be incorrect.");
			return false;
		}

		// Get node as a joint.
		std::shared_ptr<Joint> currentJoint = *it;
		
		// Parse matrix string as floats.
		std::string matrixStr = currentNode.child("matrix").text().as_string();
		iss = std::istringstream(matrixStr);
		int matrixIndex = 0;
		while (iss)
		{
			std::string floatStr;
			iss >> floatStr;
			if (floatStr.length() == 0)
			{
				continue;
			}
			
			// Set float value in the current joint's bind transform.
			currentJoint->localBindTransform[matrixIndex % 4][matrixIndex / 4] = stof(floatStr);
			matrixIndex++;
		}

		for (pugi::xml_node& childNode : currentNode.children("node"))
		{
			std::string childNodeName = childNode.attribute("name").as_string();

			// TODO: Messy code duplication
			it = find_if(joints.begin(), joints.end(), [&](std::shared_ptr<Joint>& joint) { return joint->name == childNodeName; });
			if (it == joints.end())
			{
				// Collada files seem to have extra joints. Skipping over them.
				//currentJoint->AddChild(std::make_shared<Joint>(jointCount++, childNodeName));
				continue;
			}

			// Add as child joint.
			currentJoint->AddChild(*it);

			// Push new node to stack.
			nodeStack.push(childNode);
		}
	}

	//rootJoint->CalculateInverseBindTransform(glm::mat4(1));
	m_joints = joints;
}

std::vector<glm::mat4> AnimatedMesh::getJointTransforms()
{
	std::vector<glm::mat4> jointMatrices;
	for (int i = 0; i < jointCount; i++)
	{
		jointMatrices.push_back(glm::mat4(1));
	}
	addJointsToArray(*rootJoint, jointMatrices);
	return jointMatrices;
}

void AnimatedMesh::addJointsToArray(Joint& headJoint, std::vector<glm::mat4>& jointMatrices)
{
	jointMatrices[headJoint.index] = headJoint.animatedTransform;
	for (std::shared_ptr<Joint>& childJoint : headJoint.children)
	{
		addJointsToArray(*childJoint, jointMatrices);
	}
}
