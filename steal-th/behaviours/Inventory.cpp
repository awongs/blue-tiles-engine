#include "Inventory.h"
#include <engine/debugbt/DebugLog.h>
namespace {
    void DecrementMinZero(uint & value) {
        if(value > 0) {
            value--;
        }
    }
}
Inventory::Inventory() : Behaviour(BehaviourType::Inventory) {
    m_redKey = 0;
    m_blueKey = 0;
    m_greenKey = 0;
}

void Inventory::Update(float deltaTime) {}

void Inventory::Draw(Shader& shader) {}

bool Inventory::HandleMessage(unsigned int senderID, std::string message) { 
    return false;
}

void Inventory::AddItem(ObjectType type) {
    switch(type) {
        case ObjectType::RED_KEY:
            m_redKey++;
            DebugLog::Info("Red Key:" + std::to_string(m_redKey));
            break;

        case ObjectType::BLUE_KEY:
            m_blueKey++;
            break;
        
        case ObjectType::GREEN_KEY:
            m_greenKey++;
            break;
    }
}

void Inventory::RemoveItem(ObjectType type) {
       switch(type) {
        case ObjectType::RED_KEY:
            DecrementMinZero(m_redKey);
            break;

        case ObjectType::BLUE_KEY:
            DecrementMinZero(m_blueKey);
            break;
        
        case ObjectType::GREEN_KEY:
            DecrementMinZero(m_greenKey);
            break;
    }
}


