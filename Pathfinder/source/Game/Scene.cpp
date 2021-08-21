#include "FrameworkPCH.h"
#include "Scene.h"

#include "GameCore/Objects/GameObject.h"
#include "Objects/Grid/Grid.h"
#include "Input/InputManager.h"

#include "Algorithms/IAlgorithm.h"
#include "Game/Algorithms/DijkstrasAlgorithm.h"

Scene::Scene(sf::RenderWindow* pWindow)
{
	m_pWindow = pWindow;

	m_pGrid = new Grid();
	m_GridSize = sf::Vector2u(GameConst::GRID_COLUMNS, GameConst::GRID_ROWS);

	m_pGameObjects.push_back(m_pGrid);

	SetZoomedGridSize(16);

	m_pDijkstrasAlgorithm = new DijkstrasAlgorithm(m_pGrid);

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
	//Renders the entire grid from the very begenning, might effect the initial load time
	InitializeGrid(m_GridSize, m_pWindow, m_ZoomedGridSize.x);

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

	if (m_AlgorithmExecuted)
	{
		m_AlgorithmExecuted = !(m_pAlgorithm->PlayVisualization(0.3f, deltaTime));
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

void Scene::ClearGrid()
{
	m_pGrid->ClearGrid();
}

void Scene::ClearAlgorithmSearch()
{
	m_pGrid->ClearAlgorithmSearch();
}

void Scene::UpdateTileSelector(sf::Vector2u mouseTileCoord, sf::RenderWindow* pWindow)
{
	m_pGrid->UpdateTileSelector(mouseTileCoord, pWindow);
}

void Scene::UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType)
{
	m_pGrid->UpdateTileProperty(mouseTileCoord, m_GridSize, tileType);
}


void Scene::ExecuteAlgorithm()
{
	m_AlgorithmExecuted = m_pAlgorithm->Execute();
}

void Scene::StopAlgorithm()
{
	m_AlgorithmExecuted = false;
	m_pAlgorithm->Stop();
}

