#include "Scene.h"

Scene::Scene(std::list<std::unique_ptr<GameObject>>& worldGameObjects, std::list<std::unique_ptr<GameObject>>& screenGameObjects)
{
	std::move(begin(worldGameObjects), end(worldGameObjects), std::inserter(m_worldGameObjects, end(m_worldGameObjects)));
	std::move(begin(screenGameObjects), end(screenGameObjects), std::inserter(m_screenGameObjects, end(m_screenGameObjects)));
}

Scene::~Scene()
{

}

void Scene::Update()
{
	for (auto&& worldGameObj : m_worldGameObjects) worldGameObj->Update();
	for (auto&& screenGameObj : m_screenGameObjects) screenGameObj->Update();
}

void Scene::DrawWorld()
{
	for (auto&& worldGameObj : m_worldGameObjects) worldGameObj->Draw();
}

void Scene::DrawScreen()
{
	for (auto&& screenGameObj : m_screenGameObjects) screenGameObj->Draw();
}