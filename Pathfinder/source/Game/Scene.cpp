#include "FrameworkPCH.h"
#include "Scene.h"

#include "GameCore/Objects/GameObject.h"
#include "Objects/Grid/Grid.h"
#include "Input/InputManager.h"

Scene::Scene(sf::RenderWindow* pWindow)
{
	m_pWindow = pWindow;

	m_pGrid = new Grid();

	m_GridSize = sf::Vector2u(GameConst::GRID_COLUMNS, GameConst::GRID_ROWS);


	m_pGameObjects.push_back(m_pGrid);
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
	InitializeGrid(m_GridSize, m_pWindow, m_GridZoomOnScreen);

	for (GameObject* pGameObject : m_pGameObjects) 
	{
		pGameObject->Initialize();
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

void Scene::InitializeGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, unsigned int NumColumnZoom)
{
	m_pGrid->GenerateGrid(gridSize, pWindow);
	m_pGrid->ResizeGrid(NumColumnZoom, pWindow);
}

void Scene::ResizeGrid(unsigned int NumColumn)
{
	m_pGrid->ResizeGrid(NumColumn, m_pWindow);
}

