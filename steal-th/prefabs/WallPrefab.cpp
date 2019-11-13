#include "WallPrefab.h"

#include <engine/GameObject.h>

#include <engine/physics/Collider.h>
#include <engine/physics/PhysicsEngine.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/behaviours/MeshRenderer.h>
#include "../behaviours/TileBehaviour.h"

GameObject* Prefab::CreateWallGameObject(PhysicsEngine* phyEngine, glm::vec3 position, glm::vec3 scale, float tileSize)
{
	GameObject* ga = new GameObject("wall", position, glm::vec3(0.f), scale);

	Collider* collider{ new Collider(glm::vec3(tileSize / 2.f)) };
	PhysicsBehaviour* physBehaviour{ new PhysicsBehaviour(phyEngine, ga->id, collider, [](GLuint) {}) };
	ga->AddBehaviour(physBehaviour);

	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/wall.obj");
	meshRenderer->SetTexture("../Assets/textures/wall.jpg");
	ga->AddBehaviour(meshRenderer);

	TileBehaviour* tileBehaviour{ new TileBehaviour(TileType::WALL) };
	ga->AddBehaviour(tileBehaviour);

	return ga;
}
