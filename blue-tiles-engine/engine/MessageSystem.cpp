#include "MessageSystem.h"
#include "Scene.h"
#include "debugbt/DebugLog.h"
#include "Behaviour.h"

namespace MessageSystem
{
	// statics vars
	MessageManager* MessageManager::instance = new MessageManager;
	// end of static vars

	/*
		GLOBAL FUNCTIONS
	*/

	void MessageSystem::SendMessageToObject(unsigned int targetID, BehaviourType targetBehaviour, std::string message)
	{
		MessageManager::instance->QueueMessage(targetID, targetBehaviour, message);
	}

	void MessageSystem::BroadcastMessage(BehaviourType targetBehaviour, std::string message)
	{
		MessageManager::instance->QueueBroadcastMessage(targetBehaviour, message);
	}

	void MessageSystem::ProcessAllMessages(Scene* targetScene)
	{
		MessageManager::instance->ProcessMessages(targetScene);
		MessageManager::instance->ProcessBroadcast(targetScene);
	}

	/*
		CLASS FUNCTIONS
	*/

	MessageSystem::MessageManager::MessageManager()
	{
	}


	MessageSystem::MessageManager::~MessageManager()
	{
	}

	void MessageSystem::MessageManager::QueueMessage(unsigned int targetID, BehaviourType targetBehaviour, std::string message)
	{
		ObjectMessage msg;

		msg.targetID = targetID;
		msg.targetBehaviour = targetBehaviour;
		msg.message = message;

		messageQueue.push(msg);
	}

	void MessageSystem::MessageManager::QueueBroadcastMessage(BehaviourType targetBehaviour, std::string message)
	{
		ObjectMessage msg;

		msg.targetID = 0;
		msg.targetBehaviour = targetBehaviour;
		msg.message = message;

		broadcastQueue.push(msg);
	}

	void MessageSystem::MessageManager::ProcessMessages(Scene* targetScene)
	{
		while (messageQueue.size() != 0)
		{
			ObjectMessage msg = messageQueue.front();
			messageQueue.pop();

			auto& go = targetScene->getWorldGameObjectById(msg.targetID);

			// ignore message if no target game object found
			if (go != nullptr) go->HandleMessage(msg.message, msg.targetBehaviour);
		}
	}

	void MessageSystem::MessageManager::ProcessBroadcast(Scene* targetScene)
	{
		std::vector<std::unique_ptr<GameObject>> const& objects = targetScene->getWorldGameObjects();

		while (broadcastQueue.size() != 0)
		{
			ObjectMessage msg = broadcastQueue.front();
			broadcastQueue.pop();

			for (auto& obj : objects)
			{
				obj->HandleMessage(msg.message, msg.targetBehaviour);
			}
		}
	}

	void MessageSystem::MessageManager::FlushAllMessages()
	{
		size_t msgQCount = messageQueue.size();
		size_t bcQCount = broadcastQueue.size();

		messageQueue.empty();
		broadcastQueue.empty();

		DebugLog::Info("Flushed all messages in queue! MSG=" + std::to_string(msgQCount) + " BC=" + std::to_string(bcQCount));
	}
}
