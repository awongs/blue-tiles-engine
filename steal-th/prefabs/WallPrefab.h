#pragma once

#include <glm/vec3.hpp>

class GameObject;
class PhysicsEngine;

namespace Prefab
{
	GameObject* CreateWallGameObject(PhysicsEngine* phyEngine, glm::vec3 position, glm::vec3 scale, float tileSize);
}