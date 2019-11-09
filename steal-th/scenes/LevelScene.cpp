
#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/behaviours/SpotLight.h>
#include <engine/physics/Collider.h>
#include <engine/sound/SoundManager.h>
#include <engine/sound/Music.h>
#include <engine/sound/Sound.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <util/FileManager.h>

#include "../behaviours/SimpleGuardMovementAIBehaviour.h"
#include "LevelScene.h"
#include "../behaviours/PlayerMovement.h"
#include "../behaviours/FollowGameObject.h"
#include "../behaviours/Inventory.h"
#include "../behaviours/GuardDetection.h"
#include "../behaviours/ObjectBehaviour.h"
#include "../behaviours/TileBehaviour.h"
#include "../behaviours/PlayerItemPickup.h"

#include <engine/animation/AnimatedMesh.h>
#include <engine/animation/Animation.h>
#include <engine/animation/Animator.h>


const float LevelScene::TILE_SIZE{ 9.f };

namespace
{
	// TODO: Need a more accurate way to determine this.
	// Just eye-balling it for now...
	const glm::vec2 WALL_HALF_SIZES{ 3.5f, 0.5f };

	const glm::vec3 WALL_SCALE{ LevelScene::TILE_SIZE / 1.2f, LevelScene::TILE_SIZE, LevelScene::TILE_SIZE * 5 };
	const glm::vec3 DOOR_SCALE{ LevelScene::TILE_SIZE, LevelScene::TILE_SIZE, LevelScene::TILE_SIZE * 20 };
}

LevelScene::LevelScene(Level* level, PhysicsEngine *physEngine)
	: Scene(), m_physEngine(physEngine)
{
	m_levelSize = glm::ivec2(level->m_sizeX, level->m_sizeZ);

	//std::vector<int> gridsUsed;

	// Initialize tiles
	unsigned int numTiles{ level->m_sizeX * level->m_sizeZ };
	m_tiles.resize(numTiles);

	// Generate level tiles.
	for (unsigned int i = 0; i < numTiles; ++i)
	{
		// Skip floors for now, we'll create them after.
		int tile{ level->m_layout[i] };
		if (tile == 0) continue;

		TileType type{ static_cast<TileType>(tile) };
		AddTile(type, i);
	}

	// Create the world
	for (unsigned int i = 0; i < level->m_sizeX; i++)
	{
		// Create floors
		for (unsigned int j = 0; j < level->m_sizeZ; j++)
		{
			AddTile(TileType::FLOOR, i, j);
		}
	}
	
	// Create the objects
	for (Object& obj : level->m_objects)
	{
		glm::vec3 position = glm::vec3(
			(float)(obj.tileX) * TILE_SIZE + TILE_SIZE / 2.f,
			0, 
			(float)(obj.tileZ) * TILE_SIZE + TILE_SIZE / 2.f
		);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>("object", position, glm::vec3(0, glm::radians(obj.rotation), 0));

		glm::vec3 scale;
		MeshRenderer *meshRenderer{ nullptr };
		PhysicsBehaviour *physBehaviour{ nullptr };
		ObjectBehaviour *objBehaviour{ nullptr };
		switch (obj.type)
		{
			case ObjectType::RED_KEY:
			case ObjectType::BLUE_KEY:
			case ObjectType::GREEN_KEY:
			{
				meshRenderer = new MeshRenderer("../Assets/models/key.obj");
				scale = glm::vec3(0.5, 0.5, 0.5);

				Collider *col{ new Collider(glm::vec3(2.f)) };
				physBehaviour = new PhysicsBehaviour(m_physEngine, ga->id, col, [this](GLuint other) {});
				break;
			}

			case ObjectType::OBJECTIVE_ITEM:
			{
				meshRenderer = new MeshRenderer("../Assets/models/golden_goose.obj");
				meshRenderer->SetTexture("../Assets/textures/golden_goose.png");
				scale = glm::vec3(0.5, 0.5, 0.5);

				Collider *col{ new Collider(glm::vec3(2.f)) };
				physBehaviour = new PhysicsBehaviour(m_physEngine, ga->id, col, [this](GLuint other) {});
				objBehaviour = new ObjectBehaviour(ObjectType::OBJECTIVE_ITEM);
				break;
			}
		}

		switch (obj.type)
		{
			case ObjectType::RED_KEY:
				objBehaviour = new ObjectBehaviour(ObjectType::RED_KEY);
				meshRenderer->SetTexture("../Assets/textures/red_key.png");
				break;

			case ObjectType::BLUE_KEY:
				objBehaviour = new ObjectBehaviour(ObjectType::BLUE_KEY);
				meshRenderer->SetTexture("../Assets/textures/blue_key.png");
				break;

			case ObjectType::GREEN_KEY:
				objBehaviour = new ObjectBehaviour(ObjectType::GREEN_KEY);
				meshRenderer->SetTexture("../Assets/textures/green_key.png");
				break;
		}

		ga->scale = scale;
		ga->AddBehaviour(meshRenderer);
		ga->AddBehaviour(physBehaviour);
		ga->AddBehaviour(objBehaviour);

		ga->currentScene = this;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create player
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/unity_chan.obj");
	meshRenderer->SetTexture("../Assets/textures/unity_chan.png");

	glm::vec3 position = glm::vec3(
		(float)(level->m_playerSpawnX) * TILE_SIZE + TILE_SIZE / 2.f,
		0, 
		(float)(level->m_playerSpawnZ) * TILE_SIZE + TILE_SIZE / 2.f
	);

	GameObject* playerObj = new GameObject("player", position, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2));

	playerObj->AddBehaviour(meshRenderer);
	playerObj->AddBehaviour(new PlayerMovement(10));
	playerObj->AddBehaviour(new FollowGameObject(glm::vec3(0.0f, 30.0f, 10.0f)));
	playerObj->AddBehaviour(new Inventory());

	// item pickup behaviour for player
	playerObj->AddBehaviour(new PlayerItemPickup());

	// physics collider using player gameobject's on collision callback
	Collider* playerCol{ new Collider(glm::vec3(2.f)) };
	playerObj->AddBehaviour(new PhysicsBehaviour(m_physEngine, playerObj->id, playerCol));

	// Add to world
	AddWorldGameObject(playerObj);

	// Create the guards
	for (Guard &guard : level->m_guards)
	{
		AnimatedMesh* animatedMesh = new AnimatedMesh("../Assets/models/robot_kyle.obj", "../Assets/animations/robot_kyle/KyleWalking.dae");
		animatedMesh->SetTexture("../Assets/textures/robot_kyle.png");

		glm::vec3 position = glm::vec3(
			(float)(guard.tileX) * TILE_SIZE + TILE_SIZE / 2.f,
			0,
			(float)(guard.tileZ) * TILE_SIZE + TILE_SIZE / 2.f);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>("guard",
			position, glm::vec3(0, glm::radians(guard.rotAngle), 0), glm::vec3(5, 5, 5));

		ga->AddBehaviour(animatedMesh);

		// Animations for guards.
		std::shared_ptr<Animation> walk = FileManager::LoadAnimation("../Assets/animations/robot_kyle/KyleWalking.dae");
		std::shared_ptr<Animation> idle = FileManager::LoadAnimation("../Assets/animations/robot_kyle/KyleIdle.dae");
		std::shared_ptr<Animation> look = FileManager::LoadAnimation("../Assets/animations/robot_kyle/KyleLooking.dae");
		Animator* animator = new Animator(ga->GetBehaviour<AnimatedMesh>());
		ga->AddBehaviour(animator);
		animator->AddAnimation(walk);
		animator->AddAnimation(idle);
		animator->AddAnimation(look);

		// Add guard detection behaviour
		ga->AddBehaviour(new GuardDetection(this, playerObj,
			guard.tileViewDistance * LevelScene::TILE_SIZE, guard.tileViewRadius));

		SimpleGuardMovementAIBehaviour* sgmaib = new SimpleGuardMovementAIBehaviour(10.0f, glm::radians(180.0f));

		// move to box
		sgmaib->AddMoveTileAction(1, 2);
		sgmaib->AddTurnCWAction();
		sgmaib->AddMoveTileAction(1, 1);
		sgmaib->AddTurnCWAction();
		sgmaib->AddTurnCWAction();
		sgmaib->AddWaitAction(2);

		// move back
		sgmaib->AddMoveTileAction(1, 2);
		sgmaib->AddTurnCCWAction();
		sgmaib->AddMoveTileAction(2, 2);
		sgmaib->AddTurnCWAction();
		sgmaib->AddTurnCWAction();
		sgmaib->AddWaitAction(2);

		ga->AddBehaviour(sgmaib);

		// Add a spot light in front of the guard
		float theta = atan2f(guard.tileViewRadius * LevelScene::TILE_SIZE, guard.tileViewDistance * LevelScene::TILE_SIZE);
		SpotLight* guardCone = new SpotLight(glm::vec3(1), ga->forward, theta, theta * 1.25f);
		ga->AddBehaviour(guardCone);

		m_worldGameObjects.push_back(std::move(ga));
	}
}

TileType LevelScene::GetTile(unsigned int x, unsigned int z) const
{
	int clampedX{ glm::clamp((int)x, 0, m_levelSize.x - 1) };
	int clampedZ{ glm::clamp((int)z, 0, m_levelSize.y - 1) };
	int index{ clampedZ * m_levelSize.x + clampedX };

	return m_tiles[index];
}

glm::ivec2 LevelScene::GetTileCoordFromPos(glm::vec2 worldPos) const
{
	return glm::ivec2(glm::floor(worldPos / TILE_SIZE));
}

unsigned int LevelScene::GetTileIndexFromXZ(glm::ivec2 tilePos) const
{
	return tilePos.x + tilePos.y * m_levelSize.x;
}

glm::ivec2 LevelScene::GetLevelSize() const
{
	return m_levelSize;
}

void LevelScene::AddTile(TileType type, unsigned int tileIndex)
{
	unsigned int x{ tileIndex % static_cast<unsigned int>(m_levelSize.x) };
	unsigned int z{ static_cast<unsigned int>(glm::floor(static_cast<float>(tileIndex) / m_levelSize.x)) };

	AddTile(type, x, z);
}

void LevelScene::AddTile(TileType type, unsigned int x, unsigned int z)
{
	unsigned int tileIndex{ GetTileIndexFromXZ(glm::ivec2(x, z)) };

	switch (type)
	{
		case TileType::FLOOR:
		{
			MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/quad.obj");
			meshRenderer->SetTexture("../Assets/textures/panel.png");

			glm::vec3 position = glm::vec3(
				x * TILE_SIZE + TILE_SIZE / 2.f,
				-0.5,
				z * TILE_SIZE + TILE_SIZE / 2.f);
			std::unique_ptr<GameObject> ga = std::make_unique<GameObject>("floor", position, glm::vec3(glm::half_pi<float>(), 0, 0), glm::vec3(TILE_SIZE / 2.f));

			ga->AddBehaviour(meshRenderer);

			m_worldGameObjects.push_back(std::move(ga));
			break;
		}
			
		case TileType::WALL:
		case TileType::RED_DOOR:
		case TileType::BLUE_DOOR:
		case TileType::GREEN_DOOR:
		{

			glm::vec3 position = glm::vec3(
				x * TILE_SIZE + TILE_SIZE / 2.f,
				0.f,
				z * TILE_SIZE + TILE_SIZE / 2.f);
			std::unique_ptr<GameObject> ga = std::make_unique<GameObject>("wall", position, glm::vec3(0.f), WALL_SCALE);

			Collider* collider{ new Collider(glm::vec3(TILE_SIZE / 2.f)) };
			PhysicsBehaviour* physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, collider, [](GLuint) {}) };
			ga->AddBehaviour(physBehaviour);

			switch (type)
			{

				case TileType::WALL:
				{
					MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/wall.obj");
					meshRenderer->SetTexture("../Assets/textures/wall.jpg");
					ga->AddBehaviour(meshRenderer);

					TileBehaviour* tileBehaviour{ new TileBehaviour(TileType::WALL) };
					ga->AddBehaviour(tileBehaviour);
					m_tiles[tileIndex] = TileType::WALL;

					break;
				}
				case TileType::RED_DOOR:
				{
					MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/wall.obj");
					meshRenderer->SetTransparent(true);
					meshRenderer->SetTexture("../Assets/textures/red_key_block.png");
					ga->AddBehaviour(meshRenderer);

					TileBehaviour* tileBehaviour{ new TileBehaviour(TileType::RED_DOOR) };
					ga->AddBehaviour(tileBehaviour);
					m_tiles[tileIndex] = TileType::RED_DOOR;
					break;
				}
				case TileType::BLUE_DOOR:
				{
					MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/door.obj");
					meshRenderer->SetTransparent(true);
					meshRenderer->SetTexture("../Assets/textures/blue_key_block.png");
					ga->AddBehaviour(meshRenderer);

					TileBehaviour* tileBehaviour{ new TileBehaviour(TileType::BLUE_DOOR) };
					ga->AddBehaviour(tileBehaviour);
					m_tiles[tileIndex] = TileType::BLUE_DOOR;
					break;
				}
				case TileType::GREEN_DOOR:
				{
					MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/door.obj");
					meshRenderer->SetTransparent(true);
					meshRenderer->SetTexture("../Assets/textures/green_key_block.png");
					ga->AddBehaviour(meshRenderer);

					TileBehaviour* tileBehaviour{ new TileBehaviour(TileType::GREEN_DOOR) };
					ga->AddBehaviour(tileBehaviour);
					m_tiles[tileIndex] = TileType::GREEN_DOOR;
					break;
				}
			}

			ga->currentScene = this;

			m_worldGameObjects.push_back(std::move(ga));

			break;
		}

		case TileType::EXIT:
		{
			break;
		}

	}
}