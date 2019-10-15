
#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/physics/Collider.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>

#include "../behaviours/SimpleGuardMovementAIBehaviour.h"
#include "LevelScene.h"
#include "../behaviours/PlayerMovement.h"
#include "../behaviours/FollowGameObject.h"
#include "../behaviours/Inventory.h"
#include <engine/sound/SoundManager.h>
#include <engine/sound/Music.h>
#include <engine/sound/Sound.h>

namespace
{
	const std::string KEY_NAME{ "key" };
	const std::string WALL_NAME{ "wall" };
	const std::string DOOR_NAME{ "door" };
	const std::string BLOCK_NAME{ "block" };

	// TODO: Need a more accurate way to determine this.
	// Just eye-balling it for now...
	const glm::vec2 WALL_HALF_SIZES{ 3.5f, 0.5f };
}

LevelScene::LevelScene(Level* level, PhysicsEngine *physEngine)
	: Scene(), m_physEngine(physEngine)
{
	m_count = 0;
	std::vector<int> gridsUsed;

	// Initialize tiles
	for (unsigned int i = 0; i < level->width; i++)
	{
		// Create tiles
		for (unsigned int j = 0; j < level->length; j++)
		{
			Tile tile;
			tiles.push_back(tile);
		}
	}

	// Generate rooms
	for (Room& room : level->m_rooms)
	{
		for (Wall& wall : room.walls)
		{
			if (wall.facing == "upleft")
			{
				AddWall("up", wall.location, level->width, level->length);
				AddWall("left", wall.location, level->width, level->length);
			}
			else if (wall.facing == "upright")
			{
				AddWall("up", wall.location, level->width, level->length);
				AddWall("right", wall.location, level->width, level->length);
			}
			else if (wall.facing == "downleft")
			{
				AddWall("down", wall.location, level->width, level->length);
				AddWall("left", wall.location, level->width, level->length);
			}
			else if (wall.facing == "downright")
			{
				AddWall("down", wall.location, level->width, level->length);
				AddWall("right", wall.location, level->width, level->length);
			}
			else
			{
				AddWall(wall.facing, wall.location, level->width, level->length);
			}
		}
		for (Door& door : room.doors)
		{
			AddDoor(door.facing, DOOR_NAME + std::to_string(door.doorid), door.location, level->width, level->length);
		}

		for (int x : room.gridUsed)
		{
			gridsUsed.push_back(x);
		}
	}

	// Create the world
	for (unsigned int i = 0; i < level->width; i++)
	{
		// Create floors
		for (unsigned int j = 0; j < level->length; j++)
		{
			MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/quad.obj");
			meshRenderer->SetTexture("../Assets/textures/panel.png");

			glm::vec3 position = glm::vec3((double)i * 9 + 4.5, -0.5, (double)j * 9 + 4.5);
			std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "FLOOR", position, glm::vec3(glm::half_pi<float>(), 0, 0), glm::vec3(4.5, 4.5, 4.5));

			// Current location
			int currLoc = i + j * level->length;

			// Set tile num
			tiles[currLoc].num = currLoc;

			// Set tile center
			tiles[currLoc].center = position;

			// Set tile coords
			tiles[currLoc].startX = (double)i * 9.0f;
			tiles[currLoc].endX = (double)i * 9.0f + 9.0f;
			tiles[currLoc].startZ = (double)j * 9.0f;
			tiles[currLoc].endZ = (double)j * 9.0f + 9.0f;

			ga->AddBehaviour(meshRenderer);

			m_count++;
			m_worldGameObjects.push_back(std::move(ga));

			int currGrid = i + (j * 10);

			// Add walls if to edge of world if not part of a room
			if (find(gridsUsed.begin(), gridsUsed.end(), currGrid) == gridsUsed.end())
			{
				if (j == 0)
				{
					AddWall("up", currGrid, level->width, level->length);
					//DebugLog::Info("Adding wall to: " + std::to_string(currGrid));
				}
				if (j == level->length-1)
				{
					AddWall("down", currGrid, level->width, level->length);
					//DebugLog::Info("Adding wall to: " + std::to_string(currGrid));
				}
				if (i == 0)
				{
					AddWall("left", currGrid, level->width, level->length);
				}
				if (i == level->width-1)
				{
					AddWall("right", currGrid, level->width, level->length);
				}
			}

			// Make sure all tiles know of previous tile's walls/doors
			if (i != 0)
			{
				// Set current tile as right tile of previous tile
				tiles[currLoc - 1].tileRight = &tiles[currLoc];

				// Set previous tile as left of current tile
				tiles[currLoc].tileLeft = &tiles[currLoc - 1];

				// Does the tile to the left have a wall/door on the right?
				if (tiles[currLoc - 1].right != "none")
				{
					tiles[currLoc].left = tiles[currLoc - 1].right;
				}

				// Does the current tile have a wall/door on the left?
				if (tiles[currLoc].left != "none")
				{
					tiles[currLoc - 1].right = tiles[currLoc].left;
				}
			}

			if (j != 0)
			{
				// Set current tile as down tile of previous tile
				tiles[currLoc - level->length].tileDown = &tiles[currLoc];

				// Set previous tile as up of current tile
				tiles[currLoc].tileUp = &tiles[currLoc - level->length];

				// Does the tile above have a wall/door on the bottom?
				if (tiles[currLoc - level->length].down != "none")
				{
					tiles[currLoc].up = tiles[currLoc - level->length].down;
				}

				// Does the current tile have a wall/door on the top?
				if (tiles[currLoc].up != "none")
				{
					tiles[currLoc - level->length].down = tiles[currLoc].up;
				}
			}
		}
	}

	// Create the objects
	for (Object& obj : level->m_objects)
	{
		MeshRenderer* meshRenderer;

		glm::vec3 position = glm::vec3((double)(obj.location % level->width) * 9 + 4.5, 0, (double)(obj.location / level->length) * 9 + 4.5);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, obj.name, position, glm::vec3(0, glm::radians(obj.rotation), 0));

		glm::vec3 scale;
		if (obj.name.find(KEY_NAME) != std::string::npos)
		{
			meshRenderer = new MeshRenderer("../Assets/models/key.obj");
			meshRenderer->SetTexture("../Assets/textures/key.png");
			scale = glm::vec3(0.5, 0.5, 0.5);

			Collider *keyCol{ new Collider(glm::vec3(2.f)) };
			PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, keyCol, [this](GLuint other)
				{

				}) };
			ga->AddBehaviour(physBehaviour);
		}
		else if (obj.name.find(BLOCK_NAME) != std::string::npos) {
			meshRenderer = new MeshRenderer("../Assets/models/cube.obj");
			meshRenderer->SetTexture("../Assets/textures/crate.jpg");
			scale = glm::vec3(8, 8, 8);

			Collider *keyCol{ new Collider(glm::vec3(3.f)) };
			PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, keyCol, [this](GLuint other)
				{

				}) };
			ga->AddBehaviour(physBehaviour);
		}
		else
		{
			meshRenderer = new MeshRenderer("../Assets/models/golden_goose.obj");
			meshRenderer->SetTexture("../Assets/textures/golden_goose.png");
			scale = glm::vec3(0.5, 0.5, 0.5);
		}
		
		// Set object on tile
		tiles[obj.location].on = obj.name;

		ga->scale = scale;
		ga->AddBehaviour(meshRenderer);

		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create the guards
	for (Guard& guard : level->m_guards)
	{
		MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/robot_kyle.obj");
		meshRenderer->SetTexture("../Assets/textures/robot_kyle.png");

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

		glm::vec3 position = glm::vec3((double)(guard.location % level->width) * 9 + 4.5, 0, (double)(guard.location / level->length) * 9 + 4.5);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "guard", position, glm::vec3(0, glm::radians(guard.rotAngle), 0), glm::vec3(5, 5, 5));

		ga->AddBehaviour(meshRenderer);
		ga->AddBehaviour(sgmaib);
		

		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create player
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/unity_chan.obj");
	meshRenderer->SetTexture("../Assets/textures/unity_chan.png");

	glm::vec3 position = glm::vec3((double)(level->startPos % level->width) * 9 + 4.5, 0, (double)(level->startPos / level->length) * 9 + 4.5);

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "player", position, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2));

	ga->AddBehaviour(meshRenderer);
	ga->AddBehaviour(new PlayerMovement(10));
	ga->AddBehaviour(new FollowGameObject(glm::vec3(0.0f, 30.0f, 10.0f)));
	ga->AddBehaviour(new Inventory());

	Collider *playerCol{ new Collider(glm::vec3(2.f)) };
	GameObject *playerObj = ga.get();
	PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, playerCol, [this, playerObj, playerCol](GLuint other)
		{
			// TODO: We should really not be using string-matching to 
			// differentiate between object types.

			// If this is a "key", then pick it up.
			GameObject *otherObj{ GetWorldGameObjectById(other) };
			bool isDoor{ otherObj->name.find(DOOR_NAME) != std::string::npos };
			if (otherObj->name.find(KEY_NAME) != std::string::npos)
			{
				SoundManager::getInstance().getSound("key-pickup")->play();
				std::shared_ptr<Inventory> inventory{ playerObj->GetBehaviour<Inventory>().lock() };
				if (inventory != nullptr) 
				{
					// TODO: Using red key for now.
					inventory->AddItem(Inventory::ObjectType::RED_KEY);
				}

				// KILL IT
				RemoveWorldGameObject(other);
			}
			// Handle collisions against walls, doors, and blocks.
			else if (otherObj->name == WALL_NAME || isDoor || 
				otherObj->name.find(BLOCK_NAME) != std::string::npos)
			{
				// TODO: Unlock door with red key for now. Change this later to support all key/door types.
				std::shared_ptr<Inventory> inventory{ playerObj->GetBehaviour<Inventory>().lock() };
				if (isDoor)
				{	
					if(inventory->GetNumItem(Inventory::ObjectType::RED_KEY) > 0) {
						SoundManager::getInstance().getSound("door-unlocked")->play();
						inventory->RemoveItem(Inventory::ObjectType::RED_KEY);
						RemoveWorldGameObject(other);
					}else {
						SoundManager::getInstance().getSound("door-locked")->play();
					}
				}

				std::shared_ptr<PlayerMovement> playerMovement{ playerObj->GetBehaviour<PlayerMovement>().lock() };
				if (playerMovement == nullptr)
					return;

				std::shared_ptr<PhysicsBehaviour> otherPhys{ otherObj->GetBehaviour<PhysicsBehaviour>().lock() };
				if (otherPhys == nullptr)
					return;

				Collider *otherCol{ otherPhys->GetCollider() };
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
		}) };
	ga->AddBehaviour(physBehaviour);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));

	// ----- For testing -----
	/*int testTile = 13;
	DebugLog::Info("center X = " + std::to_string(tiles[testTile].center.x));
	DebugLog::Info("center Y = " + std::to_string(tiles[testTile].center.y));
	DebugLog::Info("center Z = " + std::to_string(tiles[testTile].center.z));
	DebugLog::Info("StartX = " + std::to_string(tiles[testTile].startX));
	DebugLog::Info("EndX = " + std::to_string(tiles[testTile].endX));
	DebugLog::Info("StartZ = " + std::to_string(tiles[testTile].startZ));
	DebugLog::Info("endZ = " + std::to_string(tiles[testTile].endZ));
	DebugLog::Info("Up = " + tiles[testTile].up);
	DebugLog::Info("Down = " + tiles[testTile].down);
	DebugLog::Info("Left = " + tiles[testTile].left);
	DebugLog::Info("Right = " + tiles[testTile].right);
	DebugLog::Info("TileUp = " + std::to_string(tiles[testTile].tileUp->num));
	DebugLog::Info("TileDown = " + std::to_string(tiles[testTile].tileDown->num));
	DebugLog::Info("TileLeft = " + std::to_string(tiles[testTile].tileLeft->num));
	DebugLog::Info("TileRight = " + std::to_string(tiles[testTile].tileRight->num));*/
}

void LevelScene::AddWall(std::string facing, int location, int width, int length)
{
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/wall.obj");
	meshRenderer->SetTexture("../Assets/textures/wall.jpg");

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 colliderSize;

	if (facing == "up")
	{
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9);
		rotation = glm::vec3(0, 0, 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.x, 0.f, WALL_HALF_SIZES.y);
		tiles[location].up = WALL_NAME;
	}
	else if (facing == "down")
	{
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9 + 9);
		rotation = glm::vec3(0, 0, 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.x, 0.f, WALL_HALF_SIZES.y);
		tiles[location].down = WALL_NAME;
	}
	else if (facing == "left")
	{
		position = glm::vec3((double)(location % width) * 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.y, 0.f, WALL_HALF_SIZES.x);
		tiles[location].left = WALL_NAME;
	}
	else if (facing == "right")
	{
		position = glm::vec3((double)(location % width) * 9 + 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.y, 0.f, WALL_HALF_SIZES.x);
		tiles[location].right = WALL_NAME;
	}

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, WALL_NAME, position, rotation, glm::vec3(9, 9, 9));
	ga->AddBehaviour(meshRenderer);

	Collider *collider{ new Collider(colliderSize) };
	PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, collider, [](GLuint) {}) };
	ga->AddBehaviour(physBehaviour);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}

void LevelScene::AddDoor(std::string facing, std::string name, int location, int width, int length)
{
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/door.obj");
	meshRenderer->SetTexture("../Assets/textures/door.png");

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 colliderSize;

	if (facing == "up")
	{
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9);
		rotation = glm::vec3(0, 0, 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.x, 0.f, WALL_HALF_SIZES.y);
		tiles[location].up = DOOR_NAME;
	}
	else if (facing == "down")
	{
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9 + 9);
		rotation = glm::vec3(0, 0, 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.x, 0.f, WALL_HALF_SIZES.y);
		tiles[location].down = DOOR_NAME;
	}
	else if (facing == "left")
	{
		position = glm::vec3((double)(location % width) * 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.y, 0.f, WALL_HALF_SIZES.x);
		tiles[location].left = DOOR_NAME;
	}
	else if (facing == "right")
	{
		position = glm::vec3((double)(location % width) * 9 + 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		colliderSize = glm::vec3(WALL_HALF_SIZES.y, 0.f, WALL_HALF_SIZES.x);
		tiles[location].right = DOOR_NAME;
	}

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, name, position, rotation, glm::vec3(9, 9, 9));
	ga->AddBehaviour(meshRenderer);

	Collider *collider{ new Collider(colliderSize) };
	PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, collider, [](GLuint) {}) };
	ga->AddBehaviour(physBehaviour);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}