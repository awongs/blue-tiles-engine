
#include <engine/behaviours/MeshRenderer.h>
#include <glm/gtc/matrix_transform.hpp>

#include "LevelScene.h"

LevelScene::LevelScene(Level* level)
	: Scene()
{
	// Create the room
	for (unsigned int i = 0; i < level->width; i++)
	{
		for (unsigned int j = 0;j < level->length; j++)
		{
			std::vector<std::unique_ptr<Behaviour>> behaviours;
			std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/quad.obj");
			meshRenderer->SetTexture("../Assets/textures/crate.jpg");

			behaviours.push_back(std::move(meshRenderer));

			glm::vec3 position = glm::vec3(i, 0, j);

			std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(behaviours, "FLOOR", position, glm::vec3(glm::half_pi<float>(), 0, 0));



			m_worldGameObjects.push_back(std::move(ga));
		}
	}

	// Create the objects
	for (Object& obj : level->objects)
	{
		std::vector<std::unique_ptr<Behaviour>> behaviours;
		std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>(0, "../Assets/models/golden_goose.obj");
		meshRenderer->SetTexture("../Assets/textures/golden_goose.png");

		behaviours.push_back(std::move(meshRenderer));

		glm::vec3 position = glm::vec3(obj.location % 10, 0, obj.location / 10);
		
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(behaviours, obj.name, position, glm::vec3(0, obj.rotation, 0));

		

		//m_worldGameObjects.push_back(std::move(ga));
	}
}
