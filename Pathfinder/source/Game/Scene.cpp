#include "FrameworkPCH.h"
#include "Scene.h"

#include "GameCore/Objects/GameObject.h"
#include "Input/InputManager.h"

#include "Game/Objects/UI/TopHUDWidget.h"
#include "Objects/Grid/Grid.h"

#include "Algorithms/Algorithm.h"
#include "Algorithms/AlgorithmEnum.h"

Scene::Scene(sf::RenderWindow* pWindow)
{
	m_pWindow = pWindow;

	m_pTopHUDWidget = new TopHUDWidget(sf::Vector2f(GameConst::TOP_WIDGET_WIDTH,GameConst::TOP_WIDGET_HEIGHT), sf::Color::Cyan, pWindow);

	m_pGrid = new Grid();
	m_GridSize = sf::Vector2u(GameConst::GRID_COLUMNS, GameConst::GRID_ROWS);
	m_pGameObjects.push_back(m_pGrid);
	SetZoomedGridSize(16);

	m_pAlgorithm = new Algorithm(m_pGrid);

	m_pGameObjects.push_back(m_pTopHUDWidget);

	m_AlgorithmSpeed = AlgorithmVisualSpeed::Average;
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
	InitializeGrid(m_GridSize, m_pWindow, m_ZoomedGridSize.x, m_pTopHUDWidget->GetWidgetBoxSize());

	for (GameObject* pGameObject : m_pGameObjects) 
	{
		pGameObject->Initialize();
	}
}

void Scene::Update(float deltaTime)
{
	m_pTopHUDWidget->UpdateLabel(1, "hello");

	for (GameObject* pGameObject : m_pGameObjects)
	{
		pGameObject->Update(deltaTime);
	}

	if (m_AlgorithmExecuted)
	{
		m_AlgorithmExecuted = !(m_pAlgorithm->PlayVisualization((float)m_AlgorithmSpeed, deltaTime));
	}
}

void Scene::Draw(sf::RenderWindow* pWindow)
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		pGameObject->Draw(pWindow);
	}
}

void Scene::InitializeGrid(sf::Vector2u gridSize, sf::RenderWindow* pWindow, unsigned int NumColumnZoom, sf::Vector2f TopWidgetSize)
{
	m_pGrid->GenerateGrid(gridSize, pWindow, TopWidgetSize);
	m_pGrid->ResizeGrid(NumColumnZoom, pWindow, TopWidgetSize);
}

void Scene::ResizeGrid(unsigned int NumColumn, sf::Vector2f TopWidgetSize)
{
	m_pGrid->ResizeGrid(NumColumn, m_pWindow, TopWidgetSize);
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
	if((mouseTileCoord.x <= m_ZoomedGridSize.x - 1) && (mouseTileCoord.y <= m_ZoomedGridSize.y - 1))
	m_pGrid->UpdateTileSelector(mouseTileCoord, pWindow, m_pTopHUDWidget->GetWidgetBoxSize());
}

void Scene::UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType)
{
	m_pGrid->UpdateTileProperty(mouseTileCoord, m_GridSize, tileType);
}


void Scene::ExecuteAlgorithm(AlgorithmType algorithmType)
{
	m_AlgorithmExecuted = m_pAlgorithm->Execute(algorithmType);
}

void Scene::StopAlgorithm()
{
	m_AlgorithmExecuted = false;
	m_pAlgorithm->Stop();
}

sf::Vector2f Scene::GetTopWidgetSize()
{
	return m_pTopHUDWidget->GetWidgetBoxSize();
}

AlgorithmState Scene::GetAlgorithmState()
{
	return m_pAlgorithm->GetAlgorithmState();
}
