#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "behaviours/Behaviour.h"

class Scene;
class Shader;

class GameObject
{
public: 
	// Create a GameObject, where the id is automatically incremented.
	GameObject(std::string n = "Default", glm::vec3 pos = glm::vec3(0, 0, 0),
		glm::vec3 rot = glm::vec3(0, 0, 0), glm::vec3 sca = glm::vec3(1, 1, 1));

	// Create a GameObject with a parent, where the id is automatically incremented.
	GameObject(GameObject* parent, std::string n = "Default", glm::vec3 pos = glm::vec3(0, 0, 0),
		glm::vec3 rot = glm::vec3(0, 0, 0), glm::vec3 sca = glm::vec3(1, 1, 1));

	// Deconstructor
	~GameObject();

	// Update
	virtual void Update(float deltaTime);
	
	// Draw
	virtual void Draw(Shader& shader);

	virtual void OnCollisionStay(GLuint other);

	// Handles messages
	virtual bool HandleMessage(unsigned int senderID, std::string& message, BehaviourType type);

	// Accessor for the transform matrix.
	glm::mat4 GetTransformMatrix() const;

	// Updates the transform matrix.
	void UpdateTransformMatrix();

	// Gets behaviour of BehaviourType; Returns an empty weak pointer if doesn't exist
	std::weak_ptr<Behaviour> GetBehaviour(BehaviourType type);

	// Gets behaviour of type T. Returns an empty weak pointer if it doesn't exist.
	// Significantly slower than the other GetBehaviour, so don't call this multiple
	// times per frame.
	template <typename T>
	std::weak_ptr<T> GetBehaviour()
	{
		std::type_index index(typeid(T));
		if (m_behaviours.count(index) != 0)
		{
			return std::static_pointer_cast<T>(m_behaviours[index]);
		}
		else
		{
			return std::weak_ptr<T>();
		}
	}

	// Adds a behaviour
	void AddBehaviour(Behaviour* behaviour);

	// Returns the parent
	GameObject* GetParent();

	// Sets the parent of this GameObject
	void SetParent(GameObject* parent);

	// Returns the child
	std::vector<GameObject*> GetChildren();

	// Returns the child at the index of the container
	GameObject* GetChildAtIndex(const size_t index);

	// Adds a child to the list of children
	void AddChild(GameObject* child);

	// Removes a child at the index
	void RemoveChild(const size_t index);

	// Removes a child by its id
	void RemoveChildByID(const GLuint id);

	// id of GameObject
	GLuint id;

	// Name of GameObject
	std::string name;

	glm::vec3 position;

	glm::vec3 rotation;

	glm::vec3 scale;

	// The game object's forward vector.
	glm::vec3 forward;

	// Pointer to the current scene that this gameobject is in.
	Scene* currentScene;

	// Flag if this gameobject is a screen object in the current scene.
	bool isScreenObject;

	// If the GameObject should be rendered
	bool isVisible;

private:
	// Hashmap of behaviours currently attached to this game object.
	// Access using std::type_index(typeid(childBehaviour)).
	std::unordered_map<std::type_index, std::shared_ptr<Behaviour>> m_behaviours;

	// The game object's transform/model matrix.
	glm::mat4 m_transformMatrix;

	// Pointer to the parent of this GameObject, if it exists
	GameObject* m_parent;

	// List of children, if they exists
	std::vector<GameObject*> m_children;

	static int idCounter;
};
