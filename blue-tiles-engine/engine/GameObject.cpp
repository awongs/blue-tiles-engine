#include <algorithm>
#include <functional>
#include <glm/ext/matrix_transform.hpp>

#include "GameObject.h"
#include "sound/SoundManager.h"
#include "sound/Music.h"
#include "graphics/Shader.h"
#include "behaviours/Behaviour.h"


GameObject::GameObject(int _id, std::string n, glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
	: id(_id)
	, name(n)
	, position(pos)
	, rotation(rot)
	, scale(sca)
	, m_transformMatrix(glm::mat4(1))
{
}

GameObject::~GameObject()
{
	
}

void GameObject::Update(float deltaTime)
{
	// Tell each behaviour to update
	for (std::shared_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->Update(deltaTime);
	}

	// -- Testing Purposes --
	//rotation.y += 3.14f * deltaTime;
	
	// Shared pointer to a music object.
	//auto music = SoundManager::getInstance().getMusic("alarm");
	//music->play();
}

void GameObject::Draw(Shader& shader)
{
	// Tell each behaviour to draw
	for (std::shared_ptr<Behaviour>& behaviour : m_Behaviours)
	{
		behaviour->Draw(shader);
	}
}

glm::mat4 GameObject::GetTransformMatrix() const
{
	return m_transformMatrix;
}

void GameObject::UpdateTransformMatrix()
{
	// Rotation
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0, 0, 1));

	// Translation
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1), position);

	// Set transformation matrix after scaling
	m_transformMatrix = glm::scale((translationMatrix * rotationMatrix), scale);

	// Forward vector is equal to the z column in the rotation matrix
	forward = glm::normalize(glm::vec3(-rotationMatrix[0][2], -rotationMatrix[1][2], -rotationMatrix[2][2]));
}

std::weak_ptr<Behaviour> GameObject::GetBehaviour(BehaviourType type)
{
	for (auto& behaviour : m_Behaviours)
	{
		if (behaviour->GetType() == type)
		{
			return behaviour;
		}
	}
	// Return an empty weak pointer
	return std::weak_ptr<Behaviour>();
}

bool GameObject::HandleMessage(unsigned int senderID, std::string message, BehaviourType type)
{
	std::weak_ptr<Behaviour> behav = GetBehaviour(type);

	return behav.expired() ? false : behav.lock()->HandleMessage(senderID, message);
}

void GameObject::AddBehaviour(Behaviour* behaviour)
{
	behaviour->gameObject = this;
	m_Behaviours.push_back(std::move(std::unique_ptr<Behaviour>(behaviour)));
}
