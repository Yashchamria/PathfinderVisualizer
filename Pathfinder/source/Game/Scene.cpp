#include "FrameworkPCH.h"
#include "Scene.h"

#include "../GameCore/Objects/GameObject.h"

#include "Objects/Tile.h"

Scene::Scene()
{
	//Creating game objects
	m_pTile = new Tile();
	m_pGameObjects.push_back(m_pTile);
}

Scene::~Scene()
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		delete pGameObject;
	}
}

void Scene::Initialize()
{
	for (GameObject* pGameObject : m_pGameObjects) 
	{
		pGameObject->Initialize();
	}
}

void Scene::PostInitialize()
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		pGameObject->PostInitialize();
	}
}

void Scene::Update(float deltaTime)
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		pGameObject->Update(deltaTime);
	}
}

void Scene::Draw(sf::RenderWindow* pWindow)
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		pGameObject->Draw(pWindow);
	}
}