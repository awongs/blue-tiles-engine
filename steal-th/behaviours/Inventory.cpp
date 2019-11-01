#include "Inventory.h"
#include <engine/debugbt/DebugLog.h>

namespace
{
	void DecrementMinZero(unsigned int &value)
	{
		if (value > 0)
			value--;
	}
}

Inventory::Inventory() : Behaviour(BehaviourType::Inventory) {}

void Inventory::Update(float deltaTime) {}

void Inventory::Draw(Shader &shader) {}

bool Inventory::HandleMessage(unsigned int senderID, std::string& message)
{
	return false;
}

void Inventory::AddItem(ItemType type)
{
	auto it{ m_items.find(type) };
	if (it != m_items.end())
		++it->second;
	else
		m_items.insert({ type, 1 });
}

void Inventory::RemoveItem(ItemType type)
{
	auto it{ m_items.find(type) };
	if (it != m_items.end())
		DecrementMinZero(it->second);
}

unsigned int Inventory::GetNumItem(ItemType type)
{
	auto it{ m_items.find(type) };
	if (it != m_items.end())
		return it->second;

	// No item found, so the inventory doesn't contain it.
	return 0;
}

