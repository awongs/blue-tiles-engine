#pragma once

#include <string>
#include <queue>

class Scene;
enum class BehaviourType;

namespace MessageSystem
{
	// Queues a message to be sent to the game object with the matching ID.
	// senderID			- The ID of the game object that sent the message
	// targetID			- The ID of the game object that the message is for.
	// targetBehaviour	- The behaviour that the message should be handled by in the target game object.
	// message			- The message to be handled.
	void SendMessageToObject(unsigned int senderID, unsigned int targetID, BehaviourType targetBehaviour, std::string message);

	// Queues a message to be sent to all game objects.
	// senderID			- The ID of the game object that sent the message
	// targetBehaviour	- The behaviour that the message should be handled by all game objects.
	// message			- The message to be handled. 
	void BroadcastMessage(unsigned int senderID, BehaviourType targetBehaviour, std::string message);

	// Processes/sends all of the messages in the message and broadcast queue.
	// Game object specific messages are sent first and broadcast messages after.
	// If a target game object is not found, the message is ignored and removed from the queue.
	// targetScene		- The scene to use when searching for the correct game objects.
	void ProcessAllMessages(Scene* targetScene);

	class MessageManager
	{
	public:

		// Singleton pointer to an instance of a MessageManager.
		static MessageManager* instance;

		// Constructor for a MessageManager.
		MessageManager();

		// Deconstructor of an MessageManager.
		~MessageManager();

		// Places the message into the back of the message queue.
		void QueueMessage(unsigned int senderID, unsigned int targetID, BehaviourType targetBehaviour, std::string message);

		// Places the broadcast message into the back of the broadcast queue.
		void QueueBroadcastMessage(unsigned int senderID, BehaviourType targetBehaviour, std::string message);

		// Processes/sends all of the messages in the message queue to the targeted game objects in the target scene.
		void ProcessMessages(Scene* targetScene);

		// Processes/sends all of the broadcast messages in the broadcast queue to all of the game objects in the target scene. 
		void ProcessBroadcast(Scene* targetScene);

		void FlushAllMessages();

	private:
		// struct for every message
		struct ObjectMessage
		{
			unsigned int senderID;

			unsigned int targetID;

			BehaviourType targetBehaviour;

			std::string message;
		};

		// message to objects queue 
		std::queue<ObjectMessage> messageQueue;

		// broadcast message queue
		std::queue<ObjectMessage> broadcastQueue;
	};
};
