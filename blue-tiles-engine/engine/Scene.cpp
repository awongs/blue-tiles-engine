#include "Scene.h"
#include "debugbt/DebugLog.h"
#include "MessageSystem.h"

Scene::Scene()
{

}

Scene::Scene(std::vector<std::unique_ptr<GameObject>>& worldGameObjects, std::vector<std::unique_ptr<GameObject>>& screenGameObjects)
{
	for (int i = 0; i < static_cast<int>(worldGameObjects.size()) - 2; ++i)
	{
		for (int j = i + 1; j < worldGameObjects.size(); ++j)
		{
			if (worldGameObjects[i]->id == worldGameObjects[j]->id)
			{
				DebugLog::Warn(std::string("World GameObject has a matching id: " + std::to_string(worldGameObjects[i]->id)));
			}
		}
	}
	for (int i = 0; i < static_cast<int>(worldGameObjects.size()) - 2; ++i)
	{
		for (int j = i + 1; j < screenGameObjects.size(); ++j)
		{
			if (screenGameObjects[i]->id == screenGameObjects[j]->id)
			{
				DebugLog::Warn(std::string("Screen GameObject has a matching id: " + std::to_string(screenGameObjects[i]->id)));
			}
		}
	}
	std::move(begin(worldGameObjects), end(worldGameObjects), std::inserter(m_worldGameObjects, end(m_worldGameObjects)));
	std::move(begin(screenGameObjects), end(screenGameObjects), std::inserter(m_screenGameObjects, end(m_screenGameObjects)));
}

Scene::~Scene()
{

}

void Scene::Update(float deltaTime)
{
	for (auto& worldGameObj : m_worldGameObjects) worldGameObj->Update(deltaTime);
	for (auto& screenGameObj : m_screenGameObjects) screenGameObj->Update(deltaTime);

	// message system updates
	MessageSystem::ProcessAllMessages(this);
}

void Scene::DrawWorld(Shader& shader)
{
	for (auto& worldGameObj : m_worldGameObjects) worldGameObj->Draw(shader);
}

void Scene::DrawScreen(Shader& shader)
{
	for (auto& screenGameObj : m_screenGameObjects) screenGameObj->Draw(shader);
}

std::vector<std::unique_ptr<GameObject>> const& Scene::GetWorldGameObjects() const
{
	return m_worldGameObjects;
}


GameObject* Scene::GetWorldGameObjectByIndex(const size_t index)
{
	return m_worldGameObjects.at(index).get();
}

GameObject* Scene::GetWorldGameObjectById(const GLuint id)
{
	for (auto& worldGameObject : m_worldGameObjects)
		if (worldGameObject->id == id)
			return worldGameObject.get();
	return nullptr;
}

bool Scene::AddWorldGameObject(GameObject* gameObject)
{
	auto it = find_if(m_worldGameObjects.begin(), m_worldGameObjects.end(), [&](GameObject& obj) { return obj.id == gameObject->id; });

	if (it != m_worldGameObjects.end())
	{
		DebugLog::Error("A GameObject with that id already exists.");
		return false;
	}
	m_worldGameObjects.push_back(std::make_unique<GameObject>(gameObject));
	return true;
}


bool Scene::RemoveWorldGameObject(const GLuint id)
{
	auto it = find_if(m_worldGameObjects.begin(), m_worldGameObjects.end(), [&](GameObject& obj) { return obj.id == id; });

	if (it != m_worldGameObjects.end())
	{
		auto retval = std::move(*it);
		m_worldGameObjects.erase(it);
		return true;
	}
	DebugLog::Error("A GameObject with that id does not exist.");
	return false;
}

std::vector<std::unique_ptr<GameObject>> const& Scene::GetScreenGameObjects() const
{
	return m_screenGameObjects;
}


GameObject* Scene::GetScreenGameObjectByIndex(const size_t index)
{
	return m_screenGameObjects.at(index).get();
}

GameObject* Scene::GetScreenGameObjectById(const GLuint id)
{
	for (auto& screenGameObject : m_screenGameObjects)
		if (screenGameObject->id == id)
			return screenGameObject.get();
	return nullptr;
}

bool Scene::AddScreenGameObject(GameObject* gameObject)
{
	auto it = find_if(m_screenGameObjects.begin(), m_screenGameObjects.end(), [&](GameObject& obj) { return obj.id == gameObject->id; });

	if (it != m_screenGameObjects.end())
	{
		DebugLog::Error("A GameObject with that id already exists.");
		return false;
	}
	m_screenGameObjects.push_back(std::make_unique<GameObject>(gameObject));
	return true;
}


bool Scene::RemoveScreenGameObject(const GLuint id)
{
	auto it = find_if(m_screenGameObjects.begin(), m_screenGameObjects.end(), [&](GameObject& obj) { return obj.id == id; });

	if (it != m_screenGameObjects.end())
	{
		auto retval = std::move(*it);
		m_screenGameObjects.erase(it);
		return true;
	}
	DebugLog::Error("A GameObject with that id does not exist.");
	return false;
}
