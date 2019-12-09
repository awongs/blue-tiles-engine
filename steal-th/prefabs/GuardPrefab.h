#pragma once

class GameObject;
class PhysicsEngine;
class Guard;
class LevelScene;

namespace Prefab
{
	GameObject* CreateGuardGameObject(PhysicsEngine* phyEngine, LevelScene* scene, GameObject* playerGO, Guard* guardInfo);
}