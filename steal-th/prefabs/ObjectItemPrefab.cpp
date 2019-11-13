#include "ObjectItemPrefab.h"

#include <glm/gtc/constants.hpp>

#include <engine/GameObject.h>

#include <engine/physics/Collider.h>
#include <engine/physics/PhysicsEngine.h>

#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/behaviours/PointLight.h>
#include "../behaviours/ObjectBehaviour.h"
#include "../behaviours/Rotate.h"

constexpr glm::vec3 RED = glm::vec3(1, 0, 0);
constexpr glm::vec3 GREEN = glm::vec3(0, 1, 0);
constexpr glm::vec3 BLUE = glm::vec3(0, 0, 1);
constexpr glm::vec3 YELLOW = glm::vec3(1, 1, 0);

GameObject* Prefab::CreateKeyGameObject(PhysicsEngine* phyEngine, glm::vec3& position, glm::vec3& rotation, ObjectType& type)
{
	// Create base gameobject
	GameObject* go = new GameObject("Key", position, rotation, glm::vec3(0.5, 0.5, 0.5));

	// add mesh
	MeshRenderer* mesh = new MeshRenderer("../Assets/models/key.obj");
	go->AddBehaviour(mesh);

	// add collider
	Collider* col{ new Collider(glm::vec3(2.f)) };
	go->AddBehaviour(new PhysicsBehaviour(phyEngine, go->id, col));

	// assign object behaviour
	go->AddBehaviour(new ObjectBehaviour(type));

	// add rotation behaviour
	go->AddBehaviour(new Rotate(glm::vec3(0, glm::half_pi<float>(), 0)));

	// Set colour and texture according to key colour
	switch (type)
	{
	case ObjectType::RED_KEY:
		mesh->SetTexture("../Assets/textures/red_key.png");
		go->AddBehaviour(new PointLight(RED));
		break;

	case ObjectType::GREEN_KEY:
		mesh->SetTexture("../Assets/textures/green_key.png");
		go->AddBehaviour(new PointLight(GREEN));
		break;

	case ObjectType::BLUE_KEY:
		mesh->SetTexture("../Assets/textures/blue_key.png");
		go->AddBehaviour(new PointLight(BLUE));
		break;
	}

	return go;
}

GameObject* Prefab::CreateObjectiveGooseGameObject(PhysicsEngine* phyEngine, glm::vec3& position, glm::vec3& rotation)
{
	// Create base gameobject
	GameObject* go = new GameObject("Key", position, rotation, glm::vec3(0.5, 0.5, 0.5));

	// add mesh
	MeshRenderer* mesh = new MeshRenderer("../Assets/models/golden_goose.obj");
	mesh->SetTexture("../Assets/textures/golden_goose.png");
	go->AddBehaviour(mesh);

	// add collider
	Collider* col{ new Collider(glm::vec3(2.f)) };
	go->AddBehaviour(new PhysicsBehaviour(phyEngine, go->id, col));

	// assign object behaviour
	go->AddBehaviour(new ObjectBehaviour(ObjectType::OBJECTIVE_ITEM));

	// add rotation behaviour
	go->AddBehaviour(new Rotate(glm::vec3(0, glm::half_pi<float>(), 0)));

	// add light
	go->AddBehaviour(new PointLight(YELLOW));

	return go;
}
