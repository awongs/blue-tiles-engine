#include "Scene.h"

#include "engine/debugbt/DebugLog.h"

Scene::Scene()
{

}


Scene::Scene(const std::list<std::unique_ptr<GameObject>>& worldGameObjects, const std::list<std::unique_ptr<GameObject>>& screenGameObjects)
{
	m_worldGameObjects = worldGameObjects;
	m_screenGameObjects = screenGameObjects;
}

Scene::~Scene()
{
	for (auto&& worldObj : m_worldGameObjects)
		worldObj.reset(); // deletes the raw pointer
	m_worldGameObjects.clear();
	for (auto&& screenObj : m_screenGameObjects)
		screenObj.reset(); // deletes the raw pointer
	m_screenGameObjects.clear();
}

void Scene::Update()
{
	/*
	for (std::list<std::unique_ptr<GameObject>>::iterator it = m_worldGameObjects.begin(); it != m_worldGameObjects.end(); ++it)
		(*it)->Update();
	for (std::list<std::unique_ptr<GameObject>>::iterator it = m_screenGameObjects.begin(); it != m_screenGameObjects.end(); ++it)
		(*it)->Update();
	*/
}

void Scene::DrawWorld()
{
	/*
	for (std::list<std::unique_ptr<GameObject>>::iterator it = m_worldGameObjects.begin(); it != m_worldGameObjects.end(); ++it)
		(*it)->Draw();
	*/
}

void Scene::DrawScreen()
{
	/*
	for (std::list<std::unique_ptr<GameObject>>::iterator it = m_screenGameObjects.begin(); it != m_screenGameObjects.end(); ++it)
		(*it)->Draw();
	*/
}