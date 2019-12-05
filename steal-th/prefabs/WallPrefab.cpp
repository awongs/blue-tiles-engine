#include "WallPrefab.h"

#include <engine/GameObject.h>

#include <engine/physics/Collider.h>
#include <engine/physics/PhysicsEngine.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/PointLight.h>
#include "../behaviours/TileBehaviour.h"

constexpr glm::vec3 RED = glm::vec3(1, 0, 0);
constexpr glm::vec3 GREEN = glm::vec3(0, 1, 0);
constexpr glm::vec3 BLUE = glm::vec3(0, 0, 1);

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

GameObject* Prefab::CreateDoorGameObject(PhysicsEngine* phyEngine, glm::vec3 position, glm::vec3 scale, float tileSize, TileType tileType)
{
	GameObject* ga = new GameObject("door", position, glm::vec3(0.f), scale);

	Collider* collider{ new Collider(glm::vec3(tileSize / 2.f)) };
	PhysicsBehaviour* physBehaviour{ new PhysicsBehaviour(phyEngine, ga->id, collider, [](GLuint) {}) };
	ga->AddBehaviour(physBehaviour);

	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/wall.obj");
	meshRenderer->SetTransparent(true);

	if		(tileType == TileType::RED_DOOR)	meshRenderer->SetTexture("../Assets/textures/red_key_block.png");
	else if (tileType == TileType::GREEN_DOOR)	meshRenderer->SetTexture("../Assets/textures/green_key_block.png");
	else if (tileType == TileType::BLUE_DOOR)	meshRenderer->SetTexture("../Assets/textures/blue_key_block.png");
	else if (tileType == TileType::EXIT)		meshRenderer->SetTexture("../Assets/textures/exit.jpg");

	ga->AddBehaviour(meshRenderer);

	TileBehaviour* tileBehaviour{ new TileBehaviour(tileType) };
	ga->AddBehaviour(tileBehaviour);

	if		(tileType == TileType::RED_DOOR)	ga->AddBehaviour(new PointLight(RED));
	else if (tileType == TileType::GREEN_DOOR)	ga->AddBehaviour(new PointLight(GREEN));
	else if (tileType == TileType::BLUE_DOOR)	ga->AddBehaviour(new PointLight(BLUE));

	return ga;
}
