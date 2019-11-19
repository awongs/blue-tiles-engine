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

	void SendMessageToObject(unsigned int senderID, unsigned int targetID, BehaviourType targetBehaviour, const std::string& message)
	{
		MessageManager::instance->QueueMessage(senderID, targetID, targetBehaviour, message);
	}

	void BroadcastMessage(unsigned int senderID, BehaviourType targetBehaviour, const std::string& message)
	{
		MessageManager::instance->QueueBroadcastMessage(senderID, targetBehaviour, message);
	}

	void ProcessAllMessages(Scene* targetScene)
	{
		MessageManager::instance->ProcessMessages(targetScene);
		MessageManager::instance->ProcessBroadcast(targetScene);
	}

	/*
		CLASS FUNCTIONS
	*/
	MessageManager::MessageManager()
		: processing(false)
	{
	}


	MessageManager::~MessageManager()
	{
	}

	void MessageManager::QueueMessage(unsigned int senderID, unsigned int targetID, BehaviourType targetBehaviour, const std::string& message)
	{
		ObjectMessage msg;

		msg.senderID = senderID;
		msg.targetID = targetID;
		msg.targetBehaviour = targetBehaviour;
		msg.message = message;

		if (processing) messageQueueQueue.push(msg);
		else messageQueue.push(msg);
	}

	void MessageManager::QueueBroadcastMessage(unsigned int senderID, BehaviourType targetBehaviour, const std::string& message)
	{
		ObjectMessage msg;

		msg.senderID = senderID;
		msg.targetID = 0;
		msg.targetBehaviour = targetBehaviour;
		msg.message = message;

		if (processing) broadcastQueue.push(msg);
		else broadcastQueueQueue.push(msg);
	}

	void MessageManager::ProcessMessages(Scene* targetScene)
	{
		processing = true;

		while (messageQueue.size() != 0)
		{
			ObjectMessage msg = messageQueue.front();
			messageQueue.pop();

			GameObject* go = targetScene->GetWorldGameObjectById(msg.targetID);

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

	void MessageManager::ProcessBroadcast(Scene* targetScene)
	{
		processing = true;

		std::vector<std::unique_ptr<GameObject>> const& objects = targetScene->GetWorldGameObjects();

		if (objects.size() == 0)
		{
			processing = false;
			return;
		}

		while (broadcastQueue.size() != 0)
		{
			ObjectMessage msg = broadcastQueue.front();
			broadcastQueue.pop();

			DebugLog::Info(msg.message);
			DebugLog::Info(std::to_string(objects.size()));

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

	void MessageManager::FlushAllMessages()
	{
		messageQueue.empty();
		messageQueueQueue.empty();
		broadcastQueue.empty();
		broadcastQueueQueue.empty();

		DebugLog::Info(GetInfo());
	}
	std::string MessageManager::GetInfo()
	{
		size_t msgQCount = messageQueue.size() + messageQueueQueue.size();
		size_t bcQCount = broadcastQueue.size() + broadcastQueueQueue.size();

		return "Flushed all messages in queue! MSG=" + std::to_string(msgQCount) + " BC=" + std::to_string(bcQCount);
	}
}
