
#include <engine/behaviours/MeshRenderer.h>
#include <glm/gtc/matrix_transform.hpp>

#include "LevelScene.h"

LevelScene::LevelScene(Level* level)
	: Scene()
{
	m_count = 0;
	// Create the world
	for (unsigned int i = 0; i < level->width; i++)
	{
		// Create floors
		for (unsigned int j = 0;j < level->length; j++)
		{
			std::vector<std::unique_ptr<Behaviour>> behaviours;
			std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/quad.obj");
			meshRenderer->SetTexture("../Assets/textures/panel.png");

			behaviours.push_back(std::move(meshRenderer));

			glm::vec3 position = glm::vec3((double)i*9+4.5, -0.5, (double)j*9+4.5);

			std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "FLOOR", behaviours, position, glm::vec3(glm::half_pi<float>(), 0, 0), glm::vec3(4.5, 4.5, 4.5));

			m_count++;
			m_worldGameObjects.push_back(std::move(ga));
		}
	}

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

		}
	}

	// Create the objects
	for (Object& obj : level->objects)
	{
		std::vector<std::unique_ptr<Behaviour>> behaviours;
		std::unique_ptr<MeshRenderer> meshRenderer;

		glm::vec3 scale = glm::vec3(0.5, 0.5, 0.5);
		if (obj.name.find("key") != std::string::npos)
		{
			meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/key.obj");
			meshRenderer->SetTexture("../Assets/textures/key.png");
		}
		else if (obj.name.find("block") != std::string::npos) {
			meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/cube.obj");
			meshRenderer->SetTexture("../Assets/textures/crate.jpg");
			scale = glm::vec3(8, 8, 8);
		}
		else
		{
			meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/golden_goose.obj");
			meshRenderer->SetTexture("../Assets/textures/golden_goose.png");
		}

		behaviours.push_back(std::move(meshRenderer));

		glm::vec3 position = glm::vec3(((double)(obj.location % level->width)-1)*9 + 4.5, 0, (double)(obj.location / level->length) * 9 + 4.5);
		
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, obj.name, behaviours, position, glm::vec3(0, glm::radians(obj.rotation), 0), scale);

		
		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create the guards
	for (Guard& guard : level->guards)
	{
		std::vector<std::unique_ptr<Behaviour>> behaviours;
		std::unique_ptr<MeshRenderer> meshRenderer;
		meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/robot_kyle.obj");
		meshRenderer->SetTexture("../Assets/textures/robot_kyle.png");

		behaviours.push_back(std::move(meshRenderer));

		glm::vec3 position = glm::vec3(((double)(guard.location % level->width) - 1) * 9 + 4.5, 0, (double)(guard.location / level->length) * 9 + 4.5);

		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "guard", behaviours, position, glm::vec3(0, glm::radians(guard.rotAngle), 0), glm::vec3(5, 5, 5));


		m_count++;
		m_worldGameObjects.push_back(std::move(ga));
	}

	// Create player
	std::vector<std::unique_ptr<Behaviour>> behaviours;
	std::unique_ptr<MeshRenderer> meshRenderer;
	meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/unity_chan.obj");
	meshRenderer->SetTexture("../Assets/textures/unity_chan.png");

	behaviours.push_back(std::move(meshRenderer));

	glm::vec3 position = glm::vec3(((double)(level->startPos % level->width) - 1) * 9 + 4.5, 0, (double)(level->startPos / level->length) * 9 + 4.5);

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "player", behaviours, position, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2));


	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}

void LevelScene::AddWall(std::string facing, int location, int width, int length)
{
	std::vector<std::unique_ptr<Behaviour>> behaviours;
	std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/wall.obj");
	meshRenderer->SetTexture("../Assets/textures/wall.jpg");

	behaviours.push_back(std::move(meshRenderer));
	glm::vec3 position;
	glm::vec3 rotation;

	if (facing == "up")
	{
		position = glm::vec3(((double)(location % width) - 1) * 9 + 4.5, 0, (double)(location / length) * 9);
		rotation = glm::vec3(0, 0, 0);
	}
	else if (facing == "down")
	{
		position = glm::vec3(((double)(location % width) - 1) * 9 + 4.5, 0, (double)(location / length) * 9 + 9);
		rotation = glm::vec3(0, 0, 0);
	}
	else if (facing == "left")
	{
		position = glm::vec3(((double)(location % width) - 1) * 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
	}
	else if (facing == "right")
	{
		position = glm::vec3(((double)(location % width) - 1) * 9 + 9, 0, (double)(location / length) * 9 + 4.5);
		rotation = glm::vec3(0, glm::radians(90.0), 0);
	}

	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(m_count, "WALL", behaviours, position, rotation, glm::vec3(9, 9, 9));

	m_count++;
	m_worldGameObjects.push_back(std::move(ga));
}
