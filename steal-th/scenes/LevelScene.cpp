
#include <engine/behaviours/MeshRenderer.h>
#include <glm/gtc/matrix_transform.hpp>

#include "LevelScene.h"

LevelScene::LevelScene(Level* _level)
	: Scene()
{
	level = _level;
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
	for (Room& room : level->rooms)
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

			glm::vec3 position = glm::vec3((double)i * 9 + 4.5, -0.5, (double)j * 9 + 4.5);
			std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "FLOOR", position, glm::vec3(glm::half_pi<float>(), 0, 0), glm::vec3(4.5, 4.5, 4.5));

			// Current location
			int currLoc = i + j * level->length;

			// Set tile num
			tiles[currLoc].num = currLoc;

			// Set tile center
			tiles[currLoc].center = position;

			// Set tile coords
			tiles[currLoc].startX = (double)i * 9;
			tiles[currLoc].endX = (double)i * 9 + 9;
			tiles[currLoc].startZ = (double)j * 9;
			tiles[currLoc].endZ = (double)j * 9 + 9;

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
					tiles[currLoc].left = tiles[currLoc].right;
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
	for (Object& obj : level->objects)
	{
		MeshRenderer* meshRenderer;

		glm::vec3 scale;
		
		if (obj.name.find("key") != std::string::npos)
		{
			meshRenderer = new MeshRenderer("../Assets/models/key.obj");
			meshRenderer->SetTexture("../Assets/textures/key.png");
			scale = glm::vec3(0.5, 0.5, 0.5);
		}
		else if (obj.name.find("block") != std::string::npos) {
			meshRenderer = new MeshRenderer("../Assets/models/cube.obj");
			meshRenderer->SetTexture("../Assets/textures/crate.jpg");
			scale = glm::vec3(8, 8, 8);
		}
		else
		{
			meshRenderer = new MeshRenderer("../Assets/models/golden_goose.obj");
			meshRenderer->SetTexture("../Assets/textures/golden_goose.png");
			scale = glm::vec3(0.5, 0.5, 0.5);
		}

		// Set object on tile
		tiles[obj.location].on = obj.name;

		glm::vec3 position = glm::vec3((double)(obj.location % level->width) * 9 + 4.5, 0, (double)(obj.location / level->length) * 9 + 4.5);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, obj.name, position, glm::vec3(0, glm::radians(obj.rotation), 0), scale);
		ga->AddBehaviour(meshRenderer);

		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create the guards
	for (Guard& guard : level->guards)
	{
		MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/robot_kyle.obj");
		meshRenderer->SetTexture("../Assets/textures/robot_kyle.png");

		glm::vec3 position = glm::vec3((double)(guard.location % level->width) * 9 + 4.5, 0, (double)(guard.location / level->length) * 9 + 4.5);
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "guard", position, glm::vec3(0, glm::radians(guard.rotAngle), 0), glm::vec3(5, 5, 5));

		ga->AddBehaviour(meshRenderer);

		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create player
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/unity_chan.obj");
	meshRenderer->SetTexture("../Assets/textures/unity_chan.png");

	glm::vec3 position = glm::vec3((double)(level->startPos % level->width) * 9 + 4.5, 0, (double)(level->startPos / level->length) * 9 + 4.5);

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "player", position, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2));

	ga->AddBehaviour(meshRenderer);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}

void LevelScene::AddWall(std::string facing, int location, int width, int length)
{
	MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/wall.obj");
	meshRenderer->SetTexture("../Assets/textures/wall.jpg");

	glm::vec3 position;
	glm::vec3 rotation;

	if (facing == "up")
	{
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9);
		rotation = glm::vec3(0, 0, 0);
		tiles[location].up = "wall";
	}
	else if (facing == "down")
	{
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9 + 9);
		rotation = glm::vec3(0, 0, 0);
		tiles[location].down = "wall";
	}
	else if (facing == "left")
	{
		position = glm::vec3((double)(location % width) * 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		tiles[location].left = "wall";
	}
	else if (facing == "right")
	{
		position = glm::vec3((double)(location % width) * 9 + 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		tiles[location].right = "wall";
	}

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "WALL", position, rotation, glm::vec3(9, 9, 9));
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
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9);
		rotation = glm::vec3(0, 0, 0);
		tiles[location].up = "door";
	}
	else if (facing == "down")
	{
		position = glm::vec3((double)(location % width) * 9 + 4.5, 0, (double)(location / length) * 9 + 9);
		rotation = glm::vec3(0, 0, 0);
		tiles[location].down = "door";
	}
	else if (facing == "left")
	{
		position = glm::vec3((double)(location % width) * 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		tiles[location].left = "door";
	}
	else if (facing == "right")
	{
		position = glm::vec3((double)(location % width) * 9 + 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
		tiles[location].right = "door";
	}

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, name, position, rotation, glm::vec3(9, 9, 9));
	ga->AddBehaviour(meshRenderer);

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}