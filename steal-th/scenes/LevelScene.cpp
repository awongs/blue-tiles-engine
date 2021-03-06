#include <glm/gtc/random.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <engine/GameEngine.h>
#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/behaviours/SpotLight.h>
#include <engine/behaviours/DirectionalLight.h>
#include <engine/behaviours/PointLight.h>
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
#include "../behaviours/Rotate.h"
#include "../BlockStarrtAnimation.h"
#include <engine/behaviours/UIMenuBehaviour.h>
#include <engine/behaviours/UIImageBehaviour.h>
#include <engine/behaviours/UIButtonBehaviour.h>
#include <engine/behaviours/UITextBehaviour.h>
#include <engine/graphics/Camera.h>

#include <engine/MessageSystem.h>

#include <engine/animation/AnimatedMesh.h>
#include <engine/animation/Animation.h>
#include <engine/animation/Animator.h>
#include <engine/MessageSystem.h>

#include "../prefabs/ObjectItemPrefab.h"
#include "../prefabs/PlayerPrefab.h"
#include "../prefabs/GuardPrefab.h"
#include "../prefabs/WallPrefab.h"


constexpr glm::vec3 RED = glm::vec3(1, 0, 0);
constexpr glm::vec3 GREEN = glm::vec3(0, 1, 0);
constexpr glm::vec3 BLUE = glm::vec3(0, 0, 1);
constexpr glm::vec3 YELLOW = glm::vec3(1, 1, 0);
constexpr glm::vec3 WHITE = glm::vec3(1, 1, 1);

const float LevelScene::TILE_SIZE{ 9.f };

namespace
{
	// Window size settings.
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 600;
	// TODO: Need a more accurate way to determine this.
	// Just eye-balling it for now...
	const glm::vec2 WALL_HALF_SIZES{ 3.5f, 0.5f };

	const glm::vec3 WALL_SCALE{ LevelScene::TILE_SIZE / 1.2f, LevelScene::TILE_SIZE, LevelScene::TILE_SIZE * 5 };
	const glm::vec3 DOOR_SCALE{ LevelScene::TILE_SIZE, LevelScene::TILE_SIZE, LevelScene::TILE_SIZE * 20 };

	// Camera settings.
	constexpr float CAMERA_FOV = glm::radians(60.0f);
	constexpr float CAMERA_NEAR_CLIP = 1.0f;
	constexpr float CAMERA_FAR_CLIP = 50.0f;
	constexpr glm::vec3 CAMERA_ORIENTATION = glm::vec3(glm::radians(75.0f), 0.0f, 0.0f);
}

LevelScene::LevelScene(Level* level, PhysicsEngine *physEngine, std::shared_ptr<GameEngine> gameEngine)
	: Scene(), m_physEngine(physEngine), m_gameEngine(gameEngine)
{
	m_level = std::shared_ptr<Level>(level);

	// Initialize the level scene here.
	// This causes a duplicate call to LoadScene, but it's a convenient
	// way to dump all the load time to the menu scene.
	LoadScene(physEngine, m_gameEngine.get());
}

void LevelScene::LoadScene(PhysicsEngine* physEngine, GameEngine* gameEngine)
{
	// Clear previous level state.
	for (auto& gameObject : m_worldGameObjects)
	{
		physEngine->RemovePhysicsObject(gameObject->id);
	}
	m_worldGameObjects.clear();
	m_screenGameObjects.clear();
	m_worldGameObjectsToRemove.clear();
	isGameOver = false;
	m_elapsedTime = 0;
	m_levelTimer.reset();

	m_levelSize = glm::ivec2(m_level->m_sizeX, m_level->m_sizeZ);

	//std::vector<int> gridsUsed;

	// Initialize tiles
	unsigned int numTiles{ m_level->m_sizeX * m_level->m_sizeZ };
	m_tiles.resize(numTiles);

	// Generate level tiles.
	for (unsigned int i = 0; i < numTiles; ++i)
	{
		// Skip floors for now, we'll create them after.
		int tile{ m_level->m_layout[i] };
		if (tile == 0) continue;

		TileType type{ static_cast<TileType>(tile) };
		AddTile(type, i);
	}

	// Create the world
	for (unsigned int i = 0; i < m_level->m_sizeX; i++)
	{
		// Create floors
		for (unsigned int j = 0; j < m_level->m_sizeZ; j++)
		{
			AddTile(TileType::FLOOR, i, j);
		}
	}

	// Create the objects
	for (Object& obj : m_level->m_objects)
	{
		glm::vec3 position = glm::vec3(
			(float)(obj.tileX) * TILE_SIZE + TILE_SIZE / 2.f,
			0.5,
			(float)(obj.tileZ) * TILE_SIZE + TILE_SIZE / 2.f
		);

		glm::vec3 rotation = glm::vec3(0, glm::radians(obj.rotation), 0);

		switch (obj.type)
		{
			case ObjectType::RED_KEY:
			case ObjectType::BLUE_KEY:
			case ObjectType::GREEN_KEY:
				AddWorldGameObject(Prefab::CreateKeyGameObject(m_physEngine, position, rotation, obj.type));
				break;

			case ObjectType::OBJECTIVE_ITEM:
				AddWorldGameObject(Prefab::CreateObjectiveGooseGameObject(m_physEngine, position, rotation));
				break;

			case ObjectType::ELECTRIC_SWITCH:
				AddWorldGameObject(Prefab::CreateElectricSwitchGameObject(m_physEngine, position));
				break;
		}
	}

	// Create player
	glm::vec3 position = glm::vec3(
		(float)(m_level->m_playerSpawnX) * TILE_SIZE + TILE_SIZE / 2.f,
		0,
		(float)(m_level->m_playerSpawnZ) * TILE_SIZE + TILE_SIZE / 2.f
	);

	GameObject* playerObj = Prefab::CreatePlayerGameObject(m_physEngine, position);
	playerObj->AddBehaviour(new PlayerMovement(10, this));

	AddWorldGameObject(playerObj);

	// Create the guards
	GuardDetection::InitOpenCL();
	for (Guard& guard : m_level->m_guards)
	{
		AddWorldGameObject(Prefab::CreateGuardGameObject(m_physEngine, this, playerObj, &guard));
	}

	// Play music if it's not already playing.
	if (!Mix_PlayingMusic())
	{
		SoundManager::getInstance().getMusic("music")->play();
	}

	// UI for level
	GameObject* menu = new GameObject();
	menu->AddBehaviour(new UIMenuBehaviour("Inventory", ImVec2(0, 0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground));
	AddScreenGameObject(menu);

	GameObject* testString = new GameObject();
	testString->AddBehaviour(new UITextBehaviour("Inventory"));
	testString->SetParent(menu);
	AddScreenGameObject(testString);

	GameObject* redKey = new GameObject("redKey");
	redKey->isVisible = false;
	redKey->AddBehaviour(new UIImageBehaviour("../Assets/textures/key-card-red.png"));
	redKey->SetParent(menu);
	AddScreenGameObject(redKey);

	GameObject* greenKey = new GameObject("greenKey");
	greenKey->isVisible = false;
	greenKey->AddBehaviour(new UIImageBehaviour("../Assets/textures/key-card-green.png"));
	greenKey->SetParent(menu);
	AddScreenGameObject(greenKey);

	GameObject* blueKey = new GameObject("blueKey");
	blueKey->isVisible = false;
	blueKey->AddBehaviour(new UIImageBehaviour("../Assets/textures/key-card-blue.png"));
	blueKey->SetParent(menu);
	
	AddScreenGameObject(blueKey);

	GameObject* objectiveItem = new GameObject("objective");
	objectiveItem->isVisible = false;
	objectiveItem->AddBehaviour(new UIImageBehaviour("../Assets/textures/key-card-yellow.png"));
	objectiveItem->SetParent(menu);
	AddScreenGameObject(objectiveItem);

	GameObject* retryMenu = new GameObject("retryMenu");
	retryMenu->AddBehaviour(new UIMenuBehaviour("Game Over", ImVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), ImVec2(75, 55), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse));
	retryMenu->isVisible = false;
	AddScreenGameObject(retryMenu);

	GameObject* retryButton = new GameObject();
	retryButton->AddBehaviour(new UIButtonBehaviour("Retry", [&]
		{
			m_gameEngine->SetScene("mainMenu");
		}));
	retryButton->SetParent(retryMenu);
	AddScreenGameObject(retryButton);

	GameObject* winMenu = new GameObject("winMenu");
	winMenu->AddBehaviour(new UIMenuBehaviour("You win!", ImVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), ImVec2(85, 55), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse));
	winMenu->isVisible = false;
	AddScreenGameObject(winMenu);

	GameObject* winButton = new GameObject();
	winButton->AddBehaviour(new UIButtonBehaviour("Main Menu", [&]
		{
			m_gameEngine->SetScene("mainMenu");
		}));
	winButton->SetParent(winMenu);
	AddScreenGameObject(winButton);
	
	// Setup the camera.
	Camera::GetInstance().SetOrientation(CAMERA_ORIENTATION);
	Camera::GetInstance().CalculatePerspectiveView(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);

	// Add a directional light
	GameObject* ga = new GameObject();
	ga->AddBehaviour(new DirectionalLight(glm::vec3(1.0f), glm::vec3(0.0f, -10.0f, -0.3f), 0.0f, 0.4f, 0.5f));

	AddWorldGameObject(ga);

	// Setup the level timer.
	// Not sure how to perfectly center this, so just eyeballing the position.
	GameObject* timerMenu = new GameObject();
	timerMenu->AddBehaviour(new UIMenuBehaviour("Timer", ImVec2(WINDOW_WIDTH / 2 - 70, 0), ImVec2(0, 0),
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground));
	AddScreenGameObject(timerMenu);

	GameObject* timer = new GameObject();
	timer->AddBehaviour(new UITextBehaviour("Timer Here"));
	timer->SetParent(timerMenu);
	AddScreenGameObject(timer);

	m_levelTimer = timer->GetBehaviour<UITextBehaviour>();
}

void LevelScene::Update(float deltaTime)
{
	// Call base scene update.
	Scene::Update(deltaTime);

	// Update the level timer.
	if (!isGameOver)
	{
		m_elapsedTime += deltaTime;
		if (!m_levelTimer.expired())
		{
			int minutes = static_cast<int>(m_elapsedTime) / 60;
			int seconds = static_cast<int>(m_elapsedTime) % 60;

			m_levelTimer.lock()->SetText("Elapsed Time: " + std::to_string(minutes) + "m " +
				std::to_string(seconds) + "s");
		}
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

void LevelScene::GetTiles(std::vector<int>& output) const
{
	for (const TileType& tile : m_tiles)
	{
		output.push_back(static_cast<int>(tile));
	}
}

void LevelScene::SetTile(TileType type, unsigned int x, unsigned int z)
{
	unsigned int tileIndex{ GetTileIndexFromXZ(glm::ivec2(x, z)) };
	m_tiles[tileIndex] = type;
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
			GameObject* floorGO = new GameObject("floor", position, glm::vec3(glm::half_pi<float>(), 0, 0), glm::vec3(TILE_SIZE / 2.f));

			floorGO->AddBehaviour(new BlockStartAnimation(((int)((x + z) / 2.0f)) / 5.0f));
			floorGO->AddBehaviour(meshRenderer);
      
			AddWorldGameObject(floorGO);
			break;
		}
			
		case TileType::WALL:
		{
			glm::vec3 position = glm::vec3(
				x * TILE_SIZE + TILE_SIZE / 2.f,
				0.f,
				z * TILE_SIZE + TILE_SIZE / 2.f);

			GameObject* wallGO = Prefab::CreateWallGameObject(m_physEngine, position, WALL_SCALE, TILE_SIZE);
			m_tiles[tileIndex] = TileType::WALL;
			wallGO->AddBehaviour(new BlockStartAnimation(((int)((x + z) / 2.0f)) / 5.0f));
			AddWorldGameObject(wallGO);
			break;;
		}

		case TileType::RED_DOOR:
		case TileType::BLUE_DOOR:
		case TileType::GREEN_DOOR:
		case TileType::EXIT:
		{
			glm::vec3 position = glm::vec3(
				x * TILE_SIZE + TILE_SIZE / 2.f,
				0.f,
				z * TILE_SIZE + TILE_SIZE / 2.f);

			GameObject* doorGO = Prefab::CreateDoorGameObject(m_physEngine, position, WALL_SCALE, TILE_SIZE, type);
			doorGO->AddBehaviour(new BlockStartAnimation(((int)((x + z) / 2.0f)) / 5.0f));
			m_tiles[tileIndex] = type;
			AddWorldGameObject(doorGO);
			break;;
		}

		case TileType::ELECTRIC_FLOOR:
		case TileType::ELECTRIC_FLOOR_OFF:
		{
			glm::vec3 position = glm::vec3(
				x * TILE_SIZE + TILE_SIZE / 2.f,
				0.f,
				z * TILE_SIZE + TILE_SIZE / 2.f);

			//GameObject* electricFloor = Prefab::CreateWallGameObject(m_physEngine, position, WALL_SCALE, TILE_SIZE);
			GameObject* electricFloor = Prefab::CreateElectricFloorGameObject(m_physEngine, position, TILE_SIZE, type == TileType::ELECTRIC_FLOOR);
			m_tiles[tileIndex] = TileType::ELECTRIC_FLOOR;
			electricFloor->AddBehaviour(new BlockStartAnimation(((int)((x + z) / 2.0f)) / 5.0f));
			AddWorldGameObject(electricFloor);

			break;
		}

	}
}