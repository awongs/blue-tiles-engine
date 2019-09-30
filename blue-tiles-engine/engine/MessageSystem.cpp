#include "MessageSystem.h"
#include "Scene.h"
#include "debugbt/DebugLog.h"
#include "behaviours/Behaviour.h"

namespace MessageSystem
{
	// statics vars
	MessageManager* MessageManager::instance = new MessageManager;
	// end of static vars

	/*
		GLOBAL FUNCTIONS
	*/

	void MessageSystem::SendMessageToObject(unsigned int senderID, unsigned int targetID, BehaviourType targetBehaviour, std::string message)
	{
		MessageManager::instance->QueueMessage(senderID, targetID, targetBehaviour, message);
	}

	void MessageSystem::BroadcastMessage(unsigned int senderID, BehaviourType targetBehaviour, std::string message)
	{
		MessageManager::instance->QueueBroadcastMessage(senderID, targetBehaviour, message);
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
		: processing(false)
	{
	}


	MessageSystem::MessageManager::~MessageManager()
	{
	}

	void MessageSystem::MessageManager::QueueMessage(unsigned int senderID, unsigned int targetID, BehaviourType targetBehaviour, std::string message)
	{
		ObjectMessage msg;

		msg.senderID = senderID;
		msg.targetID = targetID;
		msg.targetBehaviour = targetBehaviour;
		msg.message = message;

		if (processing) messageQueueQueue.push(msg);
		else messageQueue.push(msg);
	}

	void MessageSystem::MessageManager::QueueBroadcastMessage(unsigned int senderID, BehaviourType targetBehaviour, std::string message)
	{
		ObjectMessage msg;

		msg.senderID = senderID;
		msg.targetID = 0;
		msg.targetBehaviour = targetBehaviour;
		msg.message = message;

		if (processing) broadcastQueue.push(msg);
		else broadcastQueueQueue.push(msg);
	}

	void MessageSystem::MessageManager::ProcessMessages(Scene* targetScene)
	{
		processing = true;

		while (messageQueue.size() != 0)
		{
			ObjectMessage msg = messageQueue.front();
			messageQueue.pop();

			auto& go = targetScene->getWorldGameObjectById(msg.targetID);

			// ignore message if no target game object found
			if (go != nullptr) go->HandleMessage(msg.senderID, msg.message, msg.targetBehaviour);
		}

		// copy queuequeue to queue
		while (messageQueueQueue.size() != 0)
		{
			messageQueue.push(messageQueueQueue.front());
			messageQueueQueue.pop();
		}

		processing = false;
	}

	void MessageSystem::MessageManager::ProcessBroadcast(Scene* targetScene)
	{
		processing = true;

		std::vector<std::unique_ptr<GameObject>> const& objects = targetScene->getWorldGameObjects();

		while (broadcastQueue.size() != 0)
		{
			ObjectMessage msg = broadcastQueue.front();
			broadcastQueue.pop();

			for (auto& obj : objects)
			{
				obj->HandleMessage(msg.senderID, msg.message, msg.targetBehaviour);
			}
		}

		// copy broadcast messages queuequeue to queue
		while (broadcastQueueQueue.size() != 0)
		{
			broadcastQueue.push(broadcastQueueQueue.front());
			broadcastQueueQueue.pop();
		}

		processing = false;
	}

	void MessageSystem::MessageManager::FlushAllMessages()
	{
		size_t msgQCount = messageQueue.size() + messageQueueQueue.size();
		size_t bcQCount = broadcastQueue.size() + broadcastQueueQueue.size();

		messageQueue.empty();
		messageQueueQueue.empty();
		broadcastQueue.empty();
		broadcastQueueQueue.empty();

		DebugLog::Info("Flushed all messages in queue! MSG=" + std::to_string(msgQCount) + " BC=" + std::to_string(bcQCount));
	}
}
