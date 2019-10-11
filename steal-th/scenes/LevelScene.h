#pragma once


#include "../blue-tiles-engine/engine/Scene.h"
#include "../Level.h"

class PhysicsEngine;

class LevelScene : public Scene
{
public:
	LevelScene(Level* level, PhysicsEngine *physEngine);
private:
	void AddWall(std::string facing, int location, int width, int length);
	void AddDoor(std::string facing, std::string name, int location, int width, int length);
	int m_count;

	// Hold a raw pointer to the GameEngine's PhysicsEngine.
	// This will be needed for registering PhysicsObjects when PhysicsBehaviours
	// are added to GameObjects.
	PhysicsEngine *m_physEngine;
};