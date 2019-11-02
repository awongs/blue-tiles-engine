
#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/physics/Collider.h>
#include <engine/sound/SoundManager.h>
#include <engine/sound/Music.h>
#include <engine/sound/Sound.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>

#include "../behaviours/SimpleGuardMovementAIBehaviour.h"
#include "LevelScene.h"
#include "../behaviours/PlayerMovement.h"
#include "../behaviours/FollowGameObject.h"
#include "../behaviours/Inventory.h"
#include "../behaviours/GuardDetection.h"
#include "../behaviours/ObjectBehaviour.h"
#include "../behaviours/TileBehaviour.h"

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

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>("player", position, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2));

	ga->AddBehaviour(meshRenderer);
	ga->AddBehaviour(new PlayerMovement(10));
	ga->AddBehaviour(new FollowGameObject(glm::vec3(0.0f, 30.0f, 10.0f)));
	ga->AddBehaviour(new Inventory());

	Collider *playerCol{ new Collider(glm::vec3(2.f)) };
	GameObject *playerObj = ga.get();
	PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, playerCol, [this, playerObj, playerCol](GLuint other)
		{
			GameObject *otherObj{ GetWorldGameObjectById(other) };

			// If this is a "key", then pick it up.
			std::shared_ptr<ObjectBehaviour> otherObjBehaviour{ otherObj->GetBehaviour<ObjectBehaviour>().lock() };
			if (otherObjBehaviour != nullptr)
			{
				std::shared_ptr<Inventory> inventory{ playerObj->GetBehaviour<Inventory>().lock() };
				if (inventory != nullptr)
				{
					switch (otherObjBehaviour->GetType())
					{
						case ObjectType::RED_KEY:
						{
							SoundManager::getInstance().getSound("key-pickup")->play();
							inventory->AddItem(Inventory::ItemType::RED_KEY);
							break;
						}
							
						case ObjectType::BLUE_KEY:
						{
							SoundManager::getInstance().getSound("key-pickup")->play();
							inventory->AddItem(Inventory::ItemType::BLUE_KEY);
							break;
						}

						case ObjectType::GREEN_KEY:
						{
							SoundManager::getInstance().getSound("key-pickup")->play();
							inventory->AddItem(Inventory::ItemType::GREEN_KEY);
							break;
						}

						case ObjectType::OBJECTIVE_ITEM:
						{
							// TODO: replace key pickup sound.
							SoundManager::getInstance().getSound("key-pickup")->play();
							inventory->AddItem(Inventory::ItemType::OBJECTIVE_ITEM);
							break;
						}
					}
				}

				RemoveWorldGameObject(other);
				return;
			}

			// Handle collisions against walls and doors.
			std::shared_ptr<TileBehaviour> otherTile{ otherObj->GetBehaviour<TileBehaviour>().lock() };
			if (otherTile != nullptr)
			{
				TileType type{ otherTile->GetType() };
				std::shared_ptr<Inventory> inventory{ playerObj->GetBehaviour<Inventory>().lock() };
				if (inventory != nullptr)
				{
					switch (type)
					{
						case TileType::RED_DOOR:
						{
							if (inventory->GetNumItem(Inventory::ItemType::RED_KEY) > 0)
							{
								SoundManager::getInstance().getSound("door-unlocked")->play();
								inventory->RemoveItem(Inventory::ItemType::RED_KEY);
								RemoveWorldGameObject(other);
							}
							else
							{
								SoundManager::getInstance().getSound("door-locked")->play();
							}

							break;
						}

						case TileType::BLUE_DOOR:
						{
							if (inventory->GetNumItem(Inventory::ItemType::BLUE_KEY) > 0)
							{
								SoundManager::getInstance().getSound("door-unlocked")->play();
								inventory->RemoveItem(Inventory::ItemType::BLUE_KEY);
								RemoveWorldGameObject(other);
							}
							else
							{
								SoundManager::getInstance().getSound("door-locked")->play();
							}
							break;
						}

						case TileType::GREEN_DOOR:
						{
							if (inventory->GetNumItem(Inventory::ItemType::GREEN_KEY) > 0)
							{
								SoundManager::getInstance().getSound("door-unlocked")->play();
								inventory->RemoveItem(Inventory::ItemType::GREEN_KEY);
								RemoveWorldGameObject(other);
							}
							else
							{
								SoundManager::getInstance().getSound("door-locked")->play();
							}

							break;
						}

						case TileType::EXIT:
						{
							if (inventory->GetNumItem(Inventory::ItemType::OBJECTIVE_ITEM) > 0)
							{
								// TODO: implement win action.
							}

							break;
						}
					}
				}

				switch (type)
				{
					case TileType::WALL:
					case TileType::RED_DOOR:
					case TileType::BLUE_DOOR:
					case TileType::GREEN_DOOR:
					{
						std::shared_ptr<PlayerMovement> playerMovement{ playerObj->GetBehaviour<PlayerMovement>().lock() };
						if (playerMovement == nullptr)
							return;

						std::shared_ptr<PhysicsBehaviour> otherPhys{ otherObj->GetBehaviour<PhysicsBehaviour>().lock() };
						if (otherPhys == nullptr)
							return;

						Collider* otherCol{ otherPhys->GetCollider() };
						if (otherCol == nullptr || otherCol->GetType() != Collider::BOX)
							return;

						// Get the four corner points of the other's collider box.
						glm::vec3 otherColSize{ otherCol->GetHalfSizes() };
						glm::vec2 otherBottomLeft{ otherObj->position.x - otherColSize.x, otherObj->position.z + otherColSize.z };
						glm::vec2 otherBottomRight{ otherObj->position.x + otherColSize.x, otherObj->position.z + otherColSize.z };
						glm::vec2 otherTopLeft{ otherObj->position.x - otherColSize.x, otherObj->position.z - otherColSize.z };
						glm::vec2 otherTopRight{ otherObj->position.x + otherColSize.x, otherObj->position.z - otherColSize.z };

						// Calculate the determinant value of point p for the line 
						// consisting of points a to b.
						auto calculateDeterminant{ [](glm::vec2 a, glm::vec2 b, glm::vec2 p)
						{
							return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
						} };

						// Check player's position against the two diagonal vectors of 
						// the other's collider box. The sign of the determinant value
						// will show which side of the diagonal the player's position
						// lies on. This will let us find the direction that the player
						// is colliding from.
						glm::vec2 playerPos{ playerObj->position.x, playerObj->position.z };
						bool isCollidingTop{ calculateDeterminant(otherTopRight, otherBottomLeft, playerPos) > 0 &&
							calculateDeterminant(otherTopLeft, otherBottomRight, playerPos) < 0 };
						bool isCollidingBottom{ calculateDeterminant(otherTopRight, otherBottomLeft, playerPos) < 0 &&
							calculateDeterminant(otherTopLeft, otherBottomRight, playerPos) > 0 };
						bool isCollidingLeft{ calculateDeterminant(otherTopRight, otherBottomLeft, playerPos) > 0 &&
							calculateDeterminant(otherTopLeft, otherBottomRight, playerPos) > 0 };
						bool isCollidingRight{ calculateDeterminant(otherTopRight, otherBottomLeft, playerPos) < 0 &&
							calculateDeterminant(otherTopLeft, otherBottomRight, playerPos) < 0 };

						// Undo the movement from PlayerMovement.
						glm::vec3 playerVel{ playerMovement->GetCurrentVelocity() };
						playerObj->position -= playerVel;

						// Calculate the new velocity vector based on collision 
						// direction. Handle the two axes separately.
						glm::vec3 newPlayerVel{ playerVel };

						// Handle the vertical axis.
						if (isCollidingTop)
						{
							newPlayerVel.z = glm::max(newPlayerVel.z, 0.f);
						}
						else if (isCollidingBottom)
						{
							newPlayerVel.z = glm::min(newPlayerVel.z, 0.f);
						}

						// Handle the horizontal axis.
						if (isCollidingLeft)
						{
							newPlayerVel.x = glm::max(newPlayerVel.x, 0.f);
						}
						else if (isCollidingRight)
						{
							newPlayerVel.x = glm::min(newPlayerVel.x, 0.f);
						}

						// Apply the new velocity.
						playerObj->position += newPlayerVel;
					}
				}
			}
		}) };
	ga->AddBehaviour(physBehaviour);

	m_worldGameObjects.push_back(std::move(ga));

	// Create the guards
	for (Guard &guard : level->m_guards)
	{
		MeshRenderer *meshRenderer = new MeshRenderer("../Assets/models/robot_kyle.obj");
		meshRenderer->SetTexture("../Assets/textures/robot_kyle.png");

		glm::vec3 position = glm::vec3(
			(float)(guard.tileX) * TILE_SIZE + TILE_SIZE / 2.f,
			0, 
			(float)(guard.tileZ) * TILE_SIZE + TILE_SIZE / 2.f);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>("guard", 
			position, glm::vec3(0, glm::radians(guard.rotAngle), 0), glm::vec3(5, 5, 5));

		ga->AddBehaviour(meshRenderer);
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

			m_worldGameObjects.push_back(std::move(ga));

			break;
		}

		case TileType::EXIT:
		{
			break;
		}

	}
}