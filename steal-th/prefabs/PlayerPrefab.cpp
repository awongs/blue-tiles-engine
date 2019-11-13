#include "PlayerPrefab.h"

#include <engine/GameObject.h>

#include <engine/physics/Collider.h>
#include <engine/physics/PhysicsEngine.h>

#include <engine/animation/AnimatedMesh.h>
#include <engine/animation/Animator.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/behaviours/PointLight.h>
#include "../behaviours/PlayerMovement.h"
#include "../behaviours/FollowGameObject.h"
#include "../behaviours/Inventory.h"
#include "../behaviours/PlayerItemPickup.h"

#include <util/FileManager.h>

constexpr glm::vec3 WHITE = glm::vec3(1, 1, 1);

GameObject* Prefab::CreatePlayerGameObject(PhysicsEngine* phyEngine, glm::vec3 position)
{
	// Create player
	AnimatedMesh* meshRenderer = new AnimatedMesh("../Assets/models/alex.obj", "../Assets/animations/alex/AlexRunning.dae");
	meshRenderer->SetTexture("../Assets/textures/alex.png");

	GameObject* playerObj = new GameObject("player", position, glm::vec3(0, 0, 0), glm::vec3(4, 4, 4));

	playerObj->AddBehaviour(meshRenderer);
	playerObj->AddBehaviour(new PlayerMovement(10));
	playerObj->AddBehaviour(new FollowGameObject(glm::vec3(0.0f, 30.0f, 10.0f)));
	playerObj->AddBehaviour(new Inventory());
	playerObj->AddBehaviour(new PointLight(WHITE));

	Animator* animator = new Animator(playerObj->GetBehaviour<AnimatedMesh>());
	playerObj->AddBehaviour(animator);

	// Animations for player.
	std::shared_ptr<Animation> run = FileManager::LoadAnimation("../Assets/animations/alex/AlexRunning.dae");
	std::shared_ptr<Animation> idle = FileManager::LoadAnimation("../Assets/animations/robot_kyle/KyleIdle.dae");
	animator->AddAnimation(run);
	animator->AddAnimation(idle);

	// item pickup behaviour for player
	playerObj->AddBehaviour(new PlayerItemPickup());

	// physics collider using player gameobject's on collision callback
	Collider* playerCol{ new Collider(glm::vec3(1.5f)) };
	playerObj->AddBehaviour(new PhysicsBehaviour(phyEngine, playerObj->id, playerCol));

	return playerObj;
}
