#include "PlayerItemPickup.h"
#include "ObjectBehaviour.h"
#include "Inventory.h"

#include <engine/GameObject.h>
#include <engine/Scene.h>
#include <engine/MessageSystem.h>
#include <engine/sound/SoundManager.h>
#include <engine/sound/Music.h>
#include <engine/sound/Sound.h>
#include <engine/debugbt/DebugLog.h>

PlayerItemPickup::PlayerItemPickup()
	: Behaviour(BehaviourType::PlayerItemPickup)
{
}

void PlayerItemPickup::Update(float deltaTime)
{
}

void PlayerItemPickup::Draw(Shader& shader)
{
}

bool PlayerItemPickup::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void PlayerItemPickup::OnCollisionStay(GLuint other)
{
	if (gameObject->currentScene == nullptr)
	{
		DebugLog::Error("Current scene for gameobject with PlayerItemPickup was nullptr!");
		return;
	}

	GameObject* otherObj{ gameObject->currentScene->GetWorldGameObjectById(other) };

	// If this is a "key", then pick it up.
	std::shared_ptr<ObjectBehaviour> otherObjBehaviour{ otherObj->GetBehaviour<ObjectBehaviour>().lock() };

	if (otherObjBehaviour != nullptr)
	{
		// don't pick up an item thats already been picked up.
		if (otherObjBehaviour->pickedUp) return;

		std::shared_ptr<Inventory> inventory{ gameObject->GetBehaviour<Inventory>().lock() };

		if (inventory != nullptr)
		{
			switch (otherObjBehaviour->GetType())
			{
			case ObjectType::RED_KEY:
			{
				SoundManager::getInstance().getSound("key-pickup")->play();
				inventory->AddItem(Inventory::ItemType::RED_KEY);
				DebugLog::Info(gameObject->name + " picked up a RED_KEY");
				otherObjBehaviour->pickedUp = true;
				MessageSystem::SendMessageToObject(gameObject->id, other, BehaviourType::NONE, "die");
				MessageSystem::SendMessageToObject(gameObject->id, "redKey", BehaviourType::NONE, "show");
				break;
			}

			case ObjectType::BLUE_KEY:
			{
				SoundManager::getInstance().getSound("key-pickup")->play();
				inventory->AddItem(Inventory::ItemType::BLUE_KEY);
				DebugLog::Info(gameObject->name + " picked up a BLUE_KEY");
				otherObjBehaviour->pickedUp = true;
				MessageSystem::SendMessageToObject(gameObject->id, other, BehaviourType::NONE, "die");
				MessageSystem::SendMessageToObject(gameObject->id, "blueKey", BehaviourType::NONE, "show");
				break;
			}

			case ObjectType::GREEN_KEY:
			{
				SoundManager::getInstance().getSound("key-pickup")->play();
				inventory->AddItem(Inventory::ItemType::GREEN_KEY);
				DebugLog::Info(gameObject->name + " picked up a GREEN_KEY");
				otherObjBehaviour->pickedUp = true;
				MessageSystem::SendMessageToObject(gameObject->id, other, BehaviourType::NONE, "die");
				MessageSystem::SendMessageToObject(gameObject->id, "greenKey", BehaviourType::NONE, "show");
				break;
			}

			case ObjectType::OBJECTIVE_ITEM:
			{
				// TODO: replace key pickup sound.
				SoundManager::getInstance().getSound("tech-pickup")->play();
				inventory->AddItem(Inventory::ItemType::OBJECTIVE_ITEM);
				DebugLog::Info(gameObject->name + " picked up a OBJECTIVE_ITEM");
				otherObjBehaviour->pickedUp = true;
				MessageSystem::SendMessageToObject(gameObject->id, other, BehaviourType::NONE, "die");
				MessageSystem::SendMessageToObject(gameObject->id, "objectiveItem", BehaviourType::NONE, "show");
				break;
			}
			}
		}
		
	}
}
