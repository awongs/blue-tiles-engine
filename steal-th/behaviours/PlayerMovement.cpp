#include <glm/gtx/compatibility.hpp>

#include <engine/input/Input.h>
#include <engine/GameObject.h>
#include <engine/MessageSystem.h>
#include <engine/sound/SoundManager.h>
#include <engine/sound/Music.h>
#include <engine/sound/Sound.h>
#include <engine/Scene.h>
#include <engine/behaviours/PhysicsBehaviour.h>
#include <engine/physics/Collider.h>
#include <engine/debugbt/DebugLog.h>
#include <engine/animation/Animator.h>

#include "PlayerMovement.h"
#include "Inventory.h"
#include "TileBehaviour.h"
#include "../scenes/LevelScene.h"

constexpr float ROTATE_SPEED = 0.05f;

PlayerMovement::PlayerMovement(float speed, LevelScene *level)
	: Behaviour(BehaviourType::PlayerMovement)
	, m_speed(speed), m_level(level)
{
}

void PlayerMovement::Update(float deltaTime)
{
	bool isKeyUp{ Input::GetInstance().IsKeyDown(Input::INPUT_UP) };
	bool isKeyDown{ Input::GetInstance().IsKeyDown(Input::INPUT_DOWN) };
	bool isKeyLeft{ Input::GetInstance().IsKeyDown(Input::INPUT_LEFT) };
	bool isKeyRight{ Input::GetInstance().IsKeyDown(Input::INPUT_RIGHT) };

	// TODO: Make this a member variable instead.
	std::shared_ptr<Animator> animator = std::static_pointer_cast<Animator>(gameObject->GetBehaviour(BehaviourType::Animator).lock());

	// Update current velocity based on input.
	if (isKeyUp)
	{
		m_currentVelocity.z = -m_speed * deltaTime;
	}
	if (isKeyDown)
	{
		m_currentVelocity.z = m_speed * deltaTime;
	}
	if (isKeyLeft)
	{
		m_currentVelocity.x = -m_speed * deltaTime;
	}
	if (isKeyRight)
	{
		m_currentVelocity.x = m_speed * deltaTime;
	}

	// Stop moving if no input pressed.
	if (!isKeyUp && !isKeyDown)
	{
		m_currentVelocity.z = 0.f;
	}
	if (!isKeyLeft && !isKeyRight)
	{
		m_currentVelocity.x = 0.f;
	}

	bool isMoving = glm::length(m_currentVelocity) > 0.0f;

	if (animator != nullptr)
	{
		if (isMoving)
		{
			animator->PlayAnimation("AlexRunning");
		}
		else
		{
			// TODO: Using robot kyle's idle animation for now.
			animator->PlayAnimation("KyleIdle");
		}
	}
	
	if (isMoving)
	{
		// Update position and rotation based on current velocity.
		gameObject->position += (m_currentVelocity);

		glm::vec3 velDirection = glm::normalize(m_currentVelocity);

		glm::quat current = glm::quatLookAt(-gameObject->forward, glm::vec3(0, 1, 0));
		glm::quat desired = glm::quatLookAt(-velDirection, glm::vec3(0, 1, 0));

		gameObject->rotation = glm::eulerAngles(glm::slerp(current, desired, ROTATE_SPEED));
	}
}

void PlayerMovement::Draw(Shader& shader)
{
	// Do nothing
}

bool PlayerMovement::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void PlayerMovement::OnCollisionStay(GLuint other)
{
	GameObject* otherObj{ gameObject->currentScene->GetWorldGameObjectById(other) };

	HandleInteractableCollision(otherObj);
	HandleWallCollision(otherObj);
}

glm::vec3 PlayerMovement::GetCurrentVelocity() const
{
	return m_currentVelocity;
}

void PlayerMovement::ResetVelocity()
{
	m_currentVelocity = glm::vec3(0);
}

void PlayerMovement::HandleInteractableCollision(GameObject* otherObj)
{
	// get tile behaviour
	std::shared_ptr<TileBehaviour> tile{ otherObj->GetBehaviour<TileBehaviour>().lock() };

	// check if other object has tile component
	if (tile == nullptr) return;

	TileType type{ tile->GetType() };

	// check if this object has inventory component
	std::shared_ptr<Inventory> inventory{ gameObject->GetBehaviour<Inventory>().lock() };
	if (inventory == nullptr) return;
	
	// Handle collisions against walls and doors.
	switch (type)
	{
	case TileType::RED_DOOR:
	{
		UnlockDoor(inventory, Inventory::ItemType::RED_KEY, otherObj);
		break;
	}

	case TileType::BLUE_DOOR:
	{
		UnlockDoor(inventory, Inventory::ItemType::BLUE_KEY, otherObj);
		break;
	}

	case TileType::GREEN_DOOR:
	{
		UnlockDoor(inventory, Inventory::ItemType::GREEN_KEY, otherObj);
		break;
	}

	case TileType::EXIT:
	{
		if (inventory->GetNumItem(Inventory::ItemType::OBJECTIVE_ITEM) > 0)
		{
			SoundManager::getInstance().getMusic("music")->stop();
			SoundManager::getInstance().getSound("win")->play();
			inventory->RemoveItem(Inventory::ItemType::OBJECTIVE_ITEM);
			MessageSystem::SendMessageToObject(gameObject->id, otherObj->id, BehaviourType::NONE, "die");
			gameObject->currentScene->stopUpdates();
			DebugLog::Info("Player reached exit. Game over.");
			// Need UI!!!!!
		}
		else
		{
			SoundManager::getInstance().getSound("door-locked")->play();
		}

		break;
	}
	}
	
		
}

void PlayerMovement::HandleWallCollision(GameObject* otherObj)
{
	// get tile behaviour
	std::shared_ptr<TileBehaviour> tile{ otherObj->GetBehaviour<TileBehaviour>().lock() };

	// check if other object has tile component
	if (tile == nullptr) return;

	TileType type{ tile->GetType() };

	switch (type)
	{
	case TileType::WALL:
	case TileType::RED_DOOR:
	case TileType::BLUE_DOOR:
	case TileType::GREEN_DOOR:
	case TileType::EXIT:
	{
		std::shared_ptr<PlayerMovement> playerMovement{ gameObject->GetBehaviour<PlayerMovement>().lock() };
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
		glm::vec2 playerPos{ gameObject->position.x, gameObject->position.z };
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
		gameObject->position -= playerVel;

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
		gameObject->position += newPlayerVel;
	}
	}
}

void PlayerMovement::UnlockDoor(std::shared_ptr<Inventory> inventory,
	Inventory::ItemType keyType, GameObject* otherObj)
{
	if (m_level == nullptr)
	{
		DebugLog::Info("PlayerMovement: null pointer to level scene.");
		return;
	}

	if (inventory->GetNumItem(keyType) > 0)
	{
		inventory->RemoveItem(keyType);
		glm::vec2 otherPos{ otherObj->position.x, otherObj->position.z };
		glm::ivec2 otherTilePos{ m_level->GetTileCoordFromPos(otherPos) };

		SoundManager::getInstance().getSound("door-unlocked")->play();
		m_level->SetTile(TileType::FLOOR, otherTilePos.x, otherTilePos.y);
		MessageSystem::SendMessageToObject(gameObject->id, otherObj->id, BehaviourType::NONE, "die");
	}
	else
	{
		SoundManager::getInstance().getSound("door-locked")->play();
	}
}

