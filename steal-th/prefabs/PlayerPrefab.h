#pragma once

#include <glm/vec3.hpp>

class GameObject;
class PhysicsEngine;

namespace Prefab
{
	GameObject* CreatePlayerGameObject(PhysicsEngine* phyEngine, glm::vec3& position);
}