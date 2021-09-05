#include "FrameworkPCH.h"
#include "Scene.h"

#include "GameCore/Objects/GameObject.h"
#include "Input/InputManager.h"

#include "Game/Objects/UI/TopHUDWidget.h"
#include "Objects/Grid/Grid.h"
#include "Objects/Grid/TileEnum.h"

#include "Algorithms/Algorithm.h"
#include "Algorithms/AlgorithmEnum.h"

Scene::Scene(sf::RenderWindow* pWindow)
{
	m_pWindow = pWindow;

	m_pTopHUDWidget = new TopHUDWidget(sf::Vector2f(GameConst::TOP_WIDGET_WIDTH,GameConst::TOP_WIDGET_HEIGHT), sf::Color(242, 166, 73), pWindow);

	m_pGrid = new Grid();
	m_GridSize = sf::Vector2u(GameConst::GRID_COLUMNS, GameConst::GRID_ROWS);
	m_pGameObjects.push_back(m_pGrid);
	SetZoomedGridSize(GameConst::GRID_COLUMNS);

	m_pCurrentAlgorithmData = new AlgorithmData();
	m_pPreviousAlgorithmData = new AlgorithmData();

	m_pAlgorithm = new Algorithms(m_pGrid, this);
	m_pCurrentAlgorithmData->SetName(m_pAlgorithm->GetAlgorithmName());

	m_pGameObjects.push_back(m_pTopHUDWidget);

	m_AlgorithmSpeed = AlgorithmVisualSpeed::Average;
	UpdateTopWidgetLabels(3, VisualSpeedToString(m_AlgorithmSpeed));

}

Scene::~Scene()
{

	delete m_pCurrentAlgorithmData;
	delete m_pPreviousAlgorithmData;

	for (GameObject* pGameObject : m_pGameObjects)
	{
		delete pGameObject;
	}
	m_pGameObjects.clear();
	m_pGameObjects.shrink_to_fit();

	if (m_pAlgorithm) { delete m_pAlgorithm; }

	m_pTopHUDWidget = nullptr;
	m_pGrid = nullptr;
	m_pWindow = nullptr;
}

void Scene::Initialize()
{
	//Renders the entire grid from the get-go, might effect the initial load time
	InitializeGrid(m_GridSize, m_pWindow, m_ZoomedGridSize.x, m_pTopHUDWidget->GetWidgetBoxSize());

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

	AutoUpdateTopWidget();

	if (m_AlgorithmExecuted)
	{
		m_AlgorithmExecuted = !(m_pAlgorithm->PlayVisualization((float)m_AlgorithmSpeed, deltaTime));

		if (GetAlgorithmState() == AlgorithmState::Visualized)
		{
			UpdateWidgetLog("Finished Visualization!!");
		}
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

void Scene::GenerateRandomGrid(unsigned int wallPercent, unsigned int StartQuadrant, unsigned int EndQuadrant)
{
	m_pGrid->GenerateRandomWalls(wallPercent);
	m_pGrid->GenerateRandomTile(TileType::StartTile, StartQuadrant, true);
	m_pGrid->GenerateRandomTile(TileType::EndTile,   EndQuadrant, true);
}

void Scene::UpdateTileProperty(sf::Vector2u mouseTileCoord, TileType tileType)
{
	m_pGrid->UpdateTileProperty(mouseTileCoord, m_GridSize, tileType);
}


void Scene::ExecuteAlgorithm(AlgorithmType algorithmType)
{
	if (m_pAlgorithm->GetAlgorithmState() != AlgorithmState::UnExecuted)
	{
		m_pPreviousAlgorithmData->SetName(m_pAlgorithm->GetAlgorithmName());
		m_pPreviousAlgorithmData->SetTimeTaken(m_pAlgorithm->GetTimeTaken());
		m_pPreviousAlgorithmData->SetTilesExplored(m_pAlgorithm->GetTilesExplored());
		m_pPreviousAlgorithmData->SetPathCost(m_pAlgorithm->GetTotalCost());
	}

	m_AlgorithmExecuted = m_pAlgorithm->Execute(algorithmType);

	m_pCurrentAlgorithmData->SetName(m_pAlgorithm->GetAlgorithmName());
	m_pCurrentAlgorithmData->SetTimeTaken(m_pAlgorithm->GetTimeTaken());
	m_pCurrentAlgorithmData->SetTilesExplored(m_pAlgorithm->GetTilesExplored());
	m_pCurrentAlgorithmData->SetPathCost(m_pAlgorithm->GetTotalCost());
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

void Scene::UpdateTopWidgetLabels(unsigned int LabelNum, std::string AppendString)
{
	m_pTopHUDWidget->UpdateLabel(LabelNum, AppendString);
}

void Scene::UpdateWidgetLog(std::string AppendString)
{
	UpdateTopWidgetLabels(6, AppendString);
}

void Scene::AutoUpdateTopWidget()
{
	UpdateTopWidgetLabels(1, m_pCurrentAlgorithmData->GetName());
	UpdateTopWidgetLabels(2, m_pCurrentAlgorithmData->GetTimeTaken());
	UpdateTopWidgetLabels(4, m_pCurrentAlgorithmData->GetPathCost());
	UpdateTopWidgetLabels(5, m_pCurrentAlgorithmData->GetTilesExplored());

	UpdateTopWidgetLabels(7, m_pPreviousAlgorithmData->GetName());
	UpdateTopWidgetLabels(8, m_pPreviousAlgorithmData->GetTimeTaken());
	UpdateTopWidgetLabels(9, m_pPreviousAlgorithmData->GetPathCost());
}
