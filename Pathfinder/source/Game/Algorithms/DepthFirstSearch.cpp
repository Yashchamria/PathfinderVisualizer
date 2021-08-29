#include "FrameworkPCH.h"
#include "DepthFirstSearch.h"
#include "AlgorithmEnum.h"

DepthFirstSearch::DepthFirstSearch(Grid* pGrid)
{
	m_pGrid = pGrid;
}

DepthFirstSearch::~DepthFirstSearch()
{
}

bool DepthFirstSearch::Execute(AlgorithmType algorithmType)
{
	return false;
}

bool DepthFirstSearch::PlayVisualization(float speed, float deltaTime)
{
	return false;
}

void DepthFirstSearch::Stop()
{
}

AlgorithmState DepthFirstSearch::GetAlgorithmState()
{
	return AlgorithmState::Executed;
}