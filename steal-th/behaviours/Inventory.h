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
    
    // Increment an item in player's inventory.
    void AddItem(ObjectType type);

    // Decrement an item in player's inventory.
    void RemoveItem(ObjectType type);
    
private:
    // Amount of red keys.
    uint m_redKey;

    // Amount of blue keys.
    uint m_blueKey;

    // Amount of green keys.
    uint m_greenKey;
};