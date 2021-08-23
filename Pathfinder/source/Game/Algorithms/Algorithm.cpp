#pragma once
#include "FrameworkPCH.h"
#include "Algorithm.h"

#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"

#include "AlgorithmEnum.h"
#include "DijkstrasAlgorithm.h"

Algorithm::Algorithm(Grid* pGrid)
{
	m_pGrid = pGrid;

	m_pDijkstrasAlgorithm = new DijkstrasAlgorithm(pGrid);


	m_pCurrentAlgorithm = m_pDijkstrasAlgorithm; // Default Algorithm
}

Algorithm::~Algorithm()
{
	m_pCurrentAlgorithm = nullptr;
	delete m_pDijkstrasAlgorithm;
}

bool Algorithm::Execute(AlgorithmType algorithmType)
{
	//Check for start and end tile
	if (m_pGrid->GetStartTile() == nullptr || m_pGrid->GetEndTile() == nullptr || 
		m_pGrid->GetStartTile()->GetTileCoord() == m_pGrid->GetEndTile()->GetTileCoord())
	{
		std::cout << "\nPlease set a start tile by pressing 'S' & set a End tile by pressing 'E'\n";
		return false;
	}

	switch (algorithmType)
	{
	case AlgorithmType::Dijkstra:
		
		std::cout << "\n\nExecuting Dijkstra...\n";
		m_pCurrentAlgorithm = m_pDijkstrasAlgorithm;
		break;

	default:
		break;
	}

	return m_pCurrentAlgorithm->Execute(algorithmType);
}

bool Algorithm::PlayVisualization(float speed, float deltaTime)
{
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