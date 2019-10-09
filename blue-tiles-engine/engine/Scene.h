#pragma once

#include <glad/glad.h>
#include <memory>

#include "GameObject.h"

class Scene
{
public:
	Scene();
	Scene(std::vector<std::unique_ptr<GameObject>>& worldGameObjects, std::vector<std::unique_ptr<GameObject>>& screenGameObjects);
	~Scene();

	// Updates game objects
	void Update(float deltaTime);

	// Draws world game objects
	void DrawWorld();

	// Draws screen game objects
	void DrawScreen();

	// Returns the vector of world game objects
	std::vector<std::unique_ptr<GameObject>> const& GetWorldGameObjects() const;

	// Returns a world game object by the index of the vector
	GameObject* GetWorldGameObjectByIndex(const size_t index);

	// Returns a world game object that matches the id in the list of world game objects
	GameObject* GetWorldGameObjectById(const GLuint id);

	// Adds a world game object to the world game objects vector
	bool AddWorldGameObject(GameObject* gameObject);

	// Removes a world game object from the world game objects vector by id
	bool RemoveWorldGameObject(const GLuint id);

	// Returns the vector of screen game objects
	std::vector<std::unique_ptr<GameObject>> const& GetScreenGameObjects() const;

	// Returns a screen game object by the index of the vector
	GameObject* GetScreenGameObjectByIndex(const size_t index);

	// Returns a screen game object that matches the id in the list of screen game objects
	GameObject* GetScreenGameObjectById(const GLuint id);

	// Adds a screen game object to the screen game objects vector
	bool AddScreenGameObject(GameObject* gameObject);

	// Removes a screen game object from the screen game objects vector by id
	bool RemoveScreenGameObject(const GLuint id);
private:

	std::vector<std::unique_ptr<GameObject>> m_worldGameObjects;
	std::vector<std::unique_ptr<GameObject>> m_screenGameObjects;
};