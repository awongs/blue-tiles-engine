#pragma once
#include <engine/behaviours/Behaviour.h>
#include <glm/glm.hpp>

#include <unordered_map>

class Inventory : public Behaviour {
public:
  // Default Constructor
  Inventory();

	// Define different pickupable items.
  enum class ItemType {
    RED_KEY, 
    BLUE_KEY, 
    GREEN_KEY,
    OBJECTIVE_ITEM
  };

  // Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;
    
  // Increment an item in player's inventory.
  void AddItem(ItemType type);

  // Decrement an item in player's inventory.
  void RemoveItem(ItemType type);
	
	// Get the number of a specific item in the player's inventory.
	unsigned int GetNumItem(ItemType type);
    
private:
	// Hold item counts for each item type.
	std::unordered_map<ItemType, unsigned int> m_items;
};