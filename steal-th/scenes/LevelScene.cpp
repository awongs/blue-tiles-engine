
#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/physics/Collider.h>
#include <glm/gtc/matrix_transform.hpp>

#include "LevelScene.h"
#include "../behaviours/PlayerMovement.h"
#include "../behaviours/FollowGameObject.h"
#include "../behaviours/Inventory.h"
#include "../behaviours/GuardDetection.h"

const float LevelScene::TILE_SIZE{ 9.f };

LevelScene::LevelScene(Level* level, PhysicsEngine *physEngine)
	: Scene(), m_physEngine(physEngine)
{
	// Can we please use axes instead of "width" and "length".
	m_levelSize = glm::ivec2(level->length, level->width);

	m_count = 0;
	std::vector<int> gridsUsed;

	// Initialize tiles
	for (unsigned int i = 0; i < level->width; i++)
	{
		// Create tiles
		for (unsigned int j = 0; j < level->length; j++)
		{
			Tile tile;
			m_tiles.push_back(tile);
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
			AddDoor(door.facing, "door" + door.doorid, door.location, level->width, level->length);
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

			glm::vec3 position = glm::vec3(
				i * TILE_SIZE + TILE_SIZE / 2.f, -0.5, 
				j * TILE_SIZE + TILE_SIZE / 2.f);
			std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "FLOOR", position, glm::vec3(glm::half_pi<float>(), 0, 0), glm::vec3(4.5, 4.5, 4.5));

			// Current location
			int currLoc = i + j * level->length;

			// Set tile num
			m_tiles[currLoc].num = currLoc;

			// Set tile center
			m_tiles[currLoc].center = position;

			// Set tile coords
			m_tiles[currLoc].startX = i * TILE_SIZE;
			m_tiles[currLoc].endX = i * TILE_SIZE + TILE_SIZE;
			m_tiles[currLoc].startZ = j * TILE_SIZE;
			m_tiles[currLoc].endZ = j * TILE_SIZE + TILE_SIZE;

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
				m_tiles[currLoc - 1].tileRight = &m_tiles[currLoc];

				// Set previous tile as left of current tile
				m_tiles[currLoc].tileLeft = &m_tiles[currLoc - 1];

				// Does the tile to the left have a wall/door on the right?
				if (m_tiles[currLoc - 1].right != Tile::TileEdgeType::NONE)
				{
					m_tiles[currLoc].left = m_tiles[currLoc - 1].right;
				}

				// Does the current tile have a wall/door on the left?
				if (m_tiles[currLoc].left != Tile::TileEdgeType::NONE)
				{
					m_tiles[currLoc - 1].right = m_tiles[currLoc].left;
				}
			}

			if (j != 0)
			{
				// Set current tile as down tile of previous tile
				m_tiles[currLoc - level->length].tileDown = &m_tiles[currLoc];

				// Set previous tile as up of current tile
				m_tiles[currLoc].tileUp = &m_tiles[currLoc - level->length];

				// Does the tile above have a wall/door on the bottom?
				if (m_tiles[currLoc - level->length].down != Tile::TileEdgeType::NONE)
				{
					m_tiles[currLoc].up = m_tiles[currLoc - level->length].down;
				}

				// Does the current tile have a wall/door on the top?
				if (m_tiles[currLoc].up != Tile::TileEdgeType::NONE)
				{
					m_tiles[currLoc - level->length].down = m_tiles[currLoc].up;
				}
			}
		}
	}

	// Create the objects
	Tile::ObjectType thisObjType{ Tile::ObjectType::NONE };
	for (Object& obj : level->m_objects)
	{
		MeshRenderer* meshRenderer;

		glm::vec3 position = glm::vec3(
			(float)(obj.location % level->width) * TILE_SIZE + TILE_SIZE / 2.f,
			0, 
			(float)(obj.location / level->length) * TILE_SIZE + TILE_SIZE / 2.f
		);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, obj.name, position, glm::vec3(0, glm::radians(obj.rotation), 0));

		glm::vec3 scale;
		if (obj.name.find("key") != std::string::npos)
		{
			meshRenderer = new MeshRenderer("../Assets/models/key.obj");
			meshRenderer->SetTexture("../Assets/textures/key.png");
			scale = glm::vec3(0.5, 0.5, 0.5);
			thisObjType = Tile::ObjectType::RED_KEY;

			Collider *keyCol{ new Collider(glm::vec3(2.f)) };
			PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, keyCol, [this](GLuint other)
				{

				}) };
			ga->AddBehaviour(physBehaviour);
		}
		else if (obj.name.find("block") != std::string::npos) {
			meshRenderer = new MeshRenderer("../Assets/models/cube.obj");
			meshRenderer->SetTexture("../Assets/textures/crate.jpg");
			scale = glm::vec3(8, 8, 8);
			thisObjType = Tile::ObjectType::BLOCK;
		}
		else
		{
			meshRenderer = new MeshRenderer("../Assets/models/golden_goose.obj");
			meshRenderer->SetTexture("../Assets/textures/golden_goose.png");
			scale = glm::vec3(0.5, 0.5, 0.5);
		}
		
		// Set object on tile
		m_tiles[obj.location].on = thisObjType;

		ga->scale = scale;
		ga->AddBehaviour(meshRenderer);

		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create player
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/unity_chan.obj");
	meshRenderer->SetTexture("../Assets/textures/unity_chan.png");

	glm::vec3 position = glm::vec3(
		(float)(level->startPos % level->width) * TILE_SIZE + TILE_SIZE / 2.f,
		0, 
		(float)(level->startPos / level->length) * TILE_SIZE + TILE_SIZE / 2.f
	);

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "player", position, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2));

	ga->AddBehaviour(meshRenderer);
	ga->AddBehaviour(new PlayerMovement(10));
	ga->AddBehaviour(new FollowGameObject(glm::vec3(0.0f, 30.0f, 10.0f)));
	ga->AddBehaviour(new Inventory());

	Collider *playerCol{ new Collider(glm::vec3(2.f)) };
	GameObject *playerObj = ga.get();
	PhysicsBehaviour *physBehaviour{ new PhysicsBehaviour(m_physEngine, ga->id, playerCol, [this, playerObj](GLuint other)
		{
			// If this is a "key", then pick it up.
			GameObject *otherObj{ GetWorldGameObjectById(other) };
			if (otherObj->name.find("key") != std::string::npos)
			{
				auto tempInventory = static_pointer_cast<Inventory>(playerObj->GetBehaviour(BehaviourType::Inventory).lock());
				if(tempInventory != 0) {
					//TODO: Using red key for now.
					tempInventory->AddItem(Inventory::ObjectType::RED_KEY);
				}

				// KILL IT
				RemoveWorldGameObject(other);
			}
		}) };
	ga->AddBehaviour(physBehaviour);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));

	// Create the guards
	for (Guard &guard : level->m_guards)
	{
		MeshRenderer *meshRenderer = new MeshRenderer("../Assets/models/robot_kyle.obj");
		meshRenderer->SetTexture("../Assets/textures/robot_kyle.png");

		glm::vec3 position = glm::vec3(
			(guard.location % level->width) * TILE_SIZE + TILE_SIZE / 2.f,
			0, 
			(guard.location / level->length) * TILE_SIZE + TILE_SIZE / 2.f);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "guard", position, glm::vec3(0, glm::radians(guard.rotAngle), 0), glm::vec3(5, 5, 5));

		ga->AddBehaviour(meshRenderer);
		ga->AddBehaviour(new GuardDetection(this, playerObj));

		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

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

const Tile &LevelScene::GetTile(unsigned int x, unsigned int z) const
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

glm::ivec2 LevelScene::GetLevelSize() const
{
	return m_levelSize;
}

void LevelScene::AddWall(std::string facing, int location, int width, int length)
{
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/wall.obj");
	meshRenderer->SetTexture("../Assets/textures/wall.jpg");

	glm::vec3 position;
	glm::vec3 rotation;

	if (facing == "up")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE + TILE_SIZE / 2.f,
			0, 
			(float)(location / length) * TILE_SIZE
		);
		rotation = glm::vec3(0, 0, 0);
		m_tiles[location].up = Tile::TileEdgeType::WALL;
	}
	else if (facing == "down")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE + TILE_SIZE / 2.f,
			0, 
			(float)(location / length) * TILE_SIZE + TILE_SIZE
		);
		rotation = glm::vec3(0, 0, 0);
		m_tiles[location].down = Tile::TileEdgeType::WALL;
	}
	else if (facing == "left")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE,
			0,
			(float)(location / length) * TILE_SIZE + TILE_SIZE / 2.f
		);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		m_tiles[location].left = Tile::TileEdgeType::WALL;
	}
	else if (facing == "right")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE + TILE_SIZE,
			0, 
			(float)(location / length) * TILE_SIZE + TILE_SIZE / 2.f);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		m_tiles[location].right = Tile::TileEdgeType::WALL;
	}

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "WALL", position, rotation, glm::vec3(TILE_SIZE));
	ga->AddBehaviour(meshRenderer);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}

void LevelScene::AddDoor(std::string facing, std::string name, int location, int width, int length)
{
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/door.obj");
	meshRenderer->SetTexture("../Assets/textures/door.png");

	glm::vec3 position;
	glm::vec3 rotation;

	if (facing == "up")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE + TILE_SIZE / 2.f, 
			0, 
			(float)(location / length) * TILE_SIZE);
		rotation = glm::vec3(0, 0, 0);
		m_tiles[location].up = Tile::TileEdgeType::DOOR;
	}
	else if (facing == "down")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE + 4.5,
			0, 
			(float)(location / length) * TILE_SIZE + TILE_SIZE
		);
		rotation = glm::vec3(0, 0, 0);
		m_tiles[location].down = Tile::TileEdgeType::DOOR;
	}
	else if (facing == "left")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE,
			0, 
			(float)(location / length) * TILE_SIZE + TILE_SIZE / 2.f);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		m_tiles[location].left = Tile::TileEdgeType::DOOR;
	}
	else if (facing == "right")
	{
		position = glm::vec3(
			(float)(location % width) * TILE_SIZE + TILE_SIZE,
			0, 
			(float)(location / length) * TILE_SIZE + TILE_SIZE / 2.f);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		m_tiles[location].right = Tile::TileEdgeType::DOOR;
	}

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, name, position, rotation, glm::vec3(TILE_SIZE));
	ga->AddBehaviour(meshRenderer);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}