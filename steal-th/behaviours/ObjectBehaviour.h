#pragma once

#include <engine/behaviours/Behaviour.h>

#include "../gameobjects/Object.h"

class ObjectBehaviour : public Behaviour
{

public:
	// Constructor.
	ObjectBehaviour(ObjectType type);

	// Overridden functions.
	void Update(float deltaTime) override;
	void Draw(Shader& shader) override;
	bool HandleMessage(unsigned int senderID, std::string& message) override;
	void OnCollisionStay(GLuint other) override;

	ObjectType GetType() const;

private:
	// This object's type.
	// We need this to differentiate between pick-upable objects.
	ObjectType m_type;
};