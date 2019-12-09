#pragma once

#include <glm/vec3.hpp>
#include "../gameobjects/Object.h"

class GameObject;
class PhysicsEngine;

namespace Prefab
{
	GameObject* CreateKeyGameObject(PhysicsEngine* phyEngine, glm::vec3 position, glm::vec3 rotation, ObjectType& type);

	GameObject* CreateObjectiveGooseGameObject(PhysicsEngine* phyEngine, glm::vec3 position, glm::vec3 rotation);

	GameObject* CreateElectricSwitchGameObject(PhysicsEngine* phyEngine, glm::vec3 position);
}