#include "Scene.h"
#include "debugbt/DebugLog.h"

Scene::Scene(std::vector<std::unique_ptr<GameObject>>& worldGameObjects, std::vector<std::unique_ptr<GameObject>>& screenGameObjects)
{
	for (int i = 0; i < worldGameObjects.size() - 2; ++i)
	{
		for (int j = i + 1; j < worldGameObjects.size(); ++j)
		{
			if (worldGameObjects[i]->id == worldGameObjects[j]->id)
			{
				DebugLog::Warn(std::string("World GameObject has a matching id: " + worldGameObjects[i]->id));
			}
		}
	}
	for (int i = 0; i < screenGameObjects.size() - 2; ++i)
	{
		for (int j = i + 1; j < screenGameObjects.size(); ++j)
		{
			if (screenGameObjects[i]->id == screenGameObjects[j]->id)
			{
				DebugLog::Warn(std::string("Screen GameObject has a matching id: " + screenGameObjects[i]->id));
			}
		}
	}
	std::move(begin(worldGameObjects), end(worldGameObjects), std::inserter(m_worldGameObjects, end(m_worldGameObjects)));
	std::move(begin(screenGameObjects), end(screenGameObjects), std::inserter(m_screenGameObjects, end(m_screenGameObjects)));
}

Scene::~Scene()
{

}

void Scene::Update()
{
	for (auto& worldGameObj : m_worldGameObjects) worldGameObj->Update();
	for (auto& screenGameObj : m_screenGameObjects) screenGameObj->Update();
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