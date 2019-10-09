#include "Scene.h"
#include "debugbt/DebugLog.h"
#include "MessageSystem.h"

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

void Scene::DrawWorld()
{
	for (auto& worldGameObj : m_worldGameObjects) worldGameObj->Draw();
}

void Scene::DrawScreen()
{
	for (auto& screenGameObj : m_screenGameObjects) screenGameObj->Draw();
}

std::vector<std::unique_ptr<GameObject>> const& Scene::getWorldGameObjects() const {
	return m_worldGameObjects;
}


std::unique_ptr<GameObject> const& Scene::getWorldGameObjectByIndex(const size_t index) const {
	return m_worldGameObjects.at(index);
}

std::unique_ptr<GameObject> const& Scene::getWorldGameObjectById(const GLuint id) const {
	for (auto& worldGameObject : m_worldGameObjects)
		if (worldGameObject->id == id)
			return worldGameObject;
	return nullptr;
}

std::vector<std::unique_ptr<GameObject>> const& Scene::getScreenGameObject() const
{
	return m_screenGameObjects;
}
