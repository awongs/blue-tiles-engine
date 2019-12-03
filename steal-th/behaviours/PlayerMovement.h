#pragma once
#include <engine/behaviours/Behaviour.h>

#include "Inventory.h"

class GameObject;
class LevelScene;

class PlayerMovement : public Behaviour
{
public:
	// Constructor.
	PlayerMovement(float speed, LevelScene *level);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

	glm::vec3 GetCurrentVelocity() const;
	void ResetVelocity();

private:

	void HandleInteractableCollision(GameObject* otherObj);

	void HandleWallCollision(GameObject* otherObj);

	void UnlockDoor(std::shared_ptr<Inventory> inventory, 
		Inventory::ItemType keyType, GameObject* otherObj);

	// Movement speed in OpenGL units per second.
	float m_speed;

	// The player's velocity for this current frame.
	// This will be influenced by m_speed.
	glm::vec3 m_currentVelocity = glm::vec3(0);

	// Pointer to the current level scene.
	LevelScene *m_level;
};