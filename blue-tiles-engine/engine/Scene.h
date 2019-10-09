#pragma once

#include <glad/glad.h>
#include <memory>

#include "GameObject.h"

class Shader;

class Scene
{
public:
	Scene(std::vector<std::unique_ptr<GameObject>>& worldGameObjects, std::vector<std::unique_ptr<GameObject>>& screenGameObjects);
	~Scene();

	// Updates game objects
	void Update(float deltaTime);

	// Draws world game objects
	void DrawWorld(Shader& shader);

	// Draws screen game objects
	void DrawScreen(Shader& shader);

	// Returns the vector of world game objects
	std::vector<std::unique_ptr<GameObject>> const& getWorldGameObjects() const;

	// Returns a world game object by the index of the vector
	std::unique_ptr<GameObject> const& getWorldGameObjectByIndex(const size_t index) const;

	// Returns a world game object that matches the id in the list of world game objects
	std::unique_ptr<GameObject> const& getWorldGameObjectById(const GLuint id) const;

	// Returns the vector of world game objects
	std::vector<std::unique_ptr<GameObject>> const& getScreenGameObject() const;
private:
	Scene() {};

	std::vector<std::unique_ptr<GameObject>> m_worldGameObjects;
	std::vector<std::unique_ptr<GameObject>> m_screenGameObjects;
};