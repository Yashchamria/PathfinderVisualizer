#pragma once
#include "FrameworkPCH.h"
#include "Algorithm.h"

#include "Game/Objects/Grid/Grid.h"
#include "Game/Scene.h"
#include "Game/Objects/Grid/Tile.h"

#include "AlgorithmEnum.h"
#include "DijkstrasAlgorithm.h"
#include "DepthFirstSearch.h"

Algorithm::Algorithm(Grid* pGrid, Scene* pScene)
{
	m_pGrid = pGrid;
	m_pScene = pScene;

	m_pDijkstrasAlgorithm = new DijkstrasAlgorithm(pGrid);
	m_pDepthFirstSearch = new DepthFirstSearch(pGrid);
}

Algorithm::~Algorithm()
{
	m_pCurrentAlgorithm = nullptr;
	m_pGrid = nullptr;
	m_pScene = nullptr;

	delete m_pDijkstrasAlgorithm;
	delete m_pDepthFirstSearch;
}

bool Algorithm::Execute(AlgorithmType algorithmType)
{
	//Check for start and end tile
	if (m_pGrid->GetStartTile() == nullptr)
	{
		m_pScene->UpdateWidgetLog("Select Start Tile (Press 'S')");
		return false;
	} 
	if (m_pGrid->GetEndTile() == nullptr)
	{
		m_pScene->UpdateWidgetLog("Select End Tile (Press 'E')");
		return false;
	}
	if (m_pGrid->GetStartTile()->GetTileCoord() == m_pGrid->GetEndTile()->GetTileCoord()) { return false; }

	m_pScene->UpdateWidgetLog("Executing Algorithm");

	switch (algorithmType)
	{
	case AlgorithmType::Dijkstra:
		
		m_pCurrentAlgorithm = m_pDijkstrasAlgorithm;
		break;

	case AlgorithmType::DepthFirstSearch:

		m_pCurrentAlgorithm = m_pDepthFirstSearch;
		break;

	default:
		break;
	}

	return m_pCurrentAlgorithm->Execute(algorithmType);
}

bool Algorithm::PlayVisualization(float speed, float deltaTime)
{
	if (IsPathFound())
	{
		m_pScene->UpdateWidgetLog("Path Found! Visualizing Path!");
	}
	else
	{
		m_pScene->UpdateWidgetLog("Path Not Found! Visualizing Path!");
	}

	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->PlayVisualization(speed, deltaTime);
	}

	return true;
}

void Algorithm::Stop()
{
	if (m_pCurrentAlgorithm)
	{
		m_pCurrentAlgorithm->Stop();
	}
}

AlgorithmState Algorithm::GetAlgorithmState()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetAlgorithmState();
	}

	return AlgorithmState::Executed;
}

bool Algorithm::IsPathFound()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->IsPathFound();
	}

	return false;
}

std::string Algorithm::GetAlgorithmName()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetAlgorithmName();
	}

	return "Not Selected!";
}

std::string Algorithm::GetTimeTaken()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetTimeTaken();
	}

	return "Not Selected!";
}

std::string Algorithm::GetTotalCost()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetTotalCost();
	}

	return "Not Selected!";
}

std::string Algorithm::GetTilesExplored()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetTilesExplored();
	}

	return "Not Selected!";
}