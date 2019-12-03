#include "GuardPrefab.h"

#include <glm/vec3.hpp>

#include <engine/GameObject.h>

#include <engine/physics/Collider.h>
#include <engine/physics/PhysicsEngine.h>

#include <engine/animation/AnimatedMesh.h>
#include <engine/animation/Animator.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/behaviours/SpotLight.h>
#include "../behaviours/GuardDetection.h"
#include "../behaviours/SimpleGuardMovementAIBehaviour.h"

#include "../gameobjects/Guard.h"

#include <util/FileManager.h>

constexpr float TILE_SIZE = 9.f;

constexpr glm::vec3 GUARD_HALF_SIZES{ 1.f };

GameObject* Prefab::CreateGuardGameObject(PhysicsEngine* phyEngine, LevelScene* scene, GameObject* playerGO, Guard* guardInfo)
{
	glm::vec3 position = glm::vec3(
		(float)(guardInfo->tileX) * TILE_SIZE + TILE_SIZE / 2.f,
		0,
		(float)(guardInfo->tileZ) * TILE_SIZE + TILE_SIZE / 2.f);

	GameObject* go = new GameObject("guard", position, glm::vec3(0, glm::radians(guardInfo->rotAngle), 0), glm::vec3(5, 5, 5));

	AnimatedMesh* animatedMesh = new AnimatedMesh("../Assets/models/robot_kyle.obj", "../Assets/animations/robot_kyle/KyleWalking.dae");
	animatedMesh->SetTexture("../Assets/textures/robot_kyle.png");	
	go->AddBehaviour(animatedMesh);

	// Animations for guards.
	std::shared_ptr<Animation> walk = FileManager::LoadAnimation("../Assets/animations/robot_kyle/KyleWalking.dae");
	std::shared_ptr<Animation> idle = FileManager::LoadAnimation("../Assets/animations/robot_kyle/KyleIdle.dae");
	std::shared_ptr<Animation> look = FileManager::LoadAnimation("../Assets/animations/robot_kyle/KyleLooking.dae");

	Animator* animator = new Animator(go->GetBehaviour<AnimatedMesh>());
	go->AddBehaviour(animator);
	animator->AddAnimation(walk);
	animator->AddAnimation(idle);
	animator->AddAnimation(look);

	// Add physics behaviour.
	Collider* guardCol{ new Collider(glm::vec3(GUARD_HALF_SIZES)) };
	go->AddBehaviour(new PhysicsBehaviour(phyEngine, go->id, guardCol));

	// Add guard detection behaviour
	go->AddBehaviour(new GuardDetection(scene, playerGO, guardInfo->tileViewDistance * TILE_SIZE, guardInfo->tileViewRadius));

	SimpleGuardMovementAIBehaviour* sgmaib = new SimpleGuardMovementAIBehaviour(10.0f, glm::radians(180.0f));

	// Setting guard movement
	for (std::string move : guardInfo->movement) {
		if (move == "turncw") {
			sgmaib->AddTurnCWAction();
		}
		else if (move == "turnccw") {
			sgmaib->AddTurnCCWAction();
		}
		else if (move.find("move") != std::string::npos) {
			std::string value1;
			std::string value2;
			bool split = false;
			for (int i = 5; i < std::strlen(move.c_str()); i++) {
				if (move[i] == ',') {
					split = true;
				}
				if (move[i] != ',' && split == false) {
					value1 += move[i];
				}
				else if (move[i] != ',' && split == true) {
					value2 += move[i];
				}
			}
			//DebugLog::Info(value1 + " " + value2);
			sgmaib->AddMoveTileAction(std::stoi(value1), std::stoi(value2));
		}
	}

	go->AddBehaviour(sgmaib);

	// Add a spot light in front of the guard
	float theta = atan2f(guardInfo->tileViewRadius * TILE_SIZE, guardInfo->tileViewDistance * TILE_SIZE);
	SpotLight* guardCone = new SpotLight(glm::vec3(1), go->forward, theta, theta * 1.1f);
	go->AddBehaviour(guardCone);

	return go;
}
