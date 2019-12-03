#pragma once

#include <glm/vec3.hpp>

#include "../gameobjects/Tile.h"

class GameObject;
class PhysicsEngine;

namespace Prefab
{
	GameObject* CreateWallGameObject(PhysicsEngine* phyEngine, glm::vec3 position, glm::vec3 scale, float tileSize);

	GameObject* CreateDoorGameObject(PhysicsEngine* phyEngine, glm::vec3 position, glm::vec3 scale, float tileSize, TileType tileType);
}