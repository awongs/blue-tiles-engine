#pragma once
#include <engine/behaviours/Behaviour.h>
#include <glm/glm.hpp>

class Inventory : public Behaviour {
public:
    // Default Constructor
    Inventory();
    enum class ObjectType {
        RED_KEY, 
        BLUE_KEY, 
        GREEN_KEY
    };
    // Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string message) override;
	void OnCollisionStay(GLuint other) override;
    
    // Increment an item in player's inventory.
    void AddItem(ObjectType type);

    // Decrement an item in player's inventory.
    void RemoveItem(ObjectType type);
	
	// Get the number of a specific item in the player's inventory.
	unsigned int GetNumItem(ObjectType type);
    
private:
    // Amount of red keys.
    unsigned int m_redKey;

    // Amount of blue keys.
	unsigned int m_blueKey;

    // Amount of green keys.
	unsigned int m_greenKey;
};