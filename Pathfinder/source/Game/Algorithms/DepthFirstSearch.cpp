#include "FrameworkPCH.h"
#include "DepthFirstSearch.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"

DepthFirstSearch::DepthFirstSearch(Grid* pGrid)
{
	m_pGrid = pGrid;
	m_AlgorithmState = AlgorithmState::UnExecuted;
}

DepthFirstSearch::~DepthFirstSearch()
{
	m_pGrid = nullptr;
	Stop();
}

void DepthFirstSearch::Init()
{
	m_tilesExplored = 0;
	m_pathCost = 0;
	m_AlgorithmState = AlgorithmState::Executing;


	//Intializing all the tiles to unvisited.
	for (unsigned int x = 0; x < m_pGrid->GetGridSize().x; x++)
	{
		for (unsigned int y = 0; y < m_pGrid->GetGridSize().y; y++)
		{
			Tile* pTile = m_pGrid->GetTile(sf::Vector2u(x, y));

			if (pTile->GetTileState() == TileType::WallTile)
				continue;

			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));
		}
	}

	//Dealing with the Start Tile
	AddToOpenlist(m_pGrid->GetStartTile());
	m_tilesExplored++;
}

bool DepthFirstSearch::Execute(AlgorithmType algorithmType)
{
	//To calculate the time algorithm took
	auto algorithmStart = std::chrono::steady_clock::now();

	Init();

	//Main loop running the algorithm
	while (!m_pOpenTiles.empty() && !m_pathfound)
	{
		if (m_stopExecution) { return false; }

		Tile* pTile = ProcessNextTile();
		AddToOpenlist(pTile);
	}

	//To calculate the time algorithm took
	auto algorithmEnd = std::chrono::steady_clock::now();
	auto algorithmDuration = algorithmEnd - algorithmStart;
	m_algorithmDuration = (std::to_string)(std::chrono::duration <double, std::milli>(algorithmDuration).count());
	m_algorithmDuration = m_algorithmDuration.substr(0, 6) + " ms";

	m_AlgorithmState = AlgorithmState::Visualizing;

	if (m_pathfound) { GetFinalPathAnimationSequence(); }

	return true;
}

bool DepthFirstSearch::PlayVisualization(float speed, float deltaTime)
{
	if (speed > 300.0f) //Instant
	{
		while (!m_PendingTileAnimation.empty())
		{
			m_PendingTileAnimation.front().first->SetTileAnimationProperty(m_PendingTileAnimation.front().second);
			m_PendingTileAnimation.front().first->UpdateTileAnimationProperty();
			m_PendingTileAnimation.pop();
		}
	}

	if (m_PendingTileAnimation.empty())
	{
		m_AlgorithmState = AlgorithmState::Visualized;
		return true;
	}

	m_PendingTileAnimation.front().first->SetTileAnimationProperty(m_PendingTileAnimation.front().second);
	m_PendingTileAnimation.front().first->UpdateTileAnimationProperty();

	m_switchSpeed += (deltaTime * speed);

	if (m_switchSpeed >= 1.0f)
	{
		m_PendingTileAnimation.pop();
		m_switchSpeed = 0.0f;
	}

	return false;
}

void DepthFirstSearch::AddToOpenlist(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_pOpenTiles.push(pTile);

	AddToTileAnimationArray(pTile, TileAnimationState::Processing);
}

Tile* DepthFirstSearch::GetNeighbourTile(Tile* pCurrentTile, NeighbourTileDirection tileDirection)
{
	sf::Vector2u CurrentTileCoord = pCurrentTile->GetTileCoord();

	Tile* neighbourTile = m_pGrid->GetNeighbourTile(CurrentTileCoord, tileDirection);

	if (neighbourTile)
	{
		if (neighbourTile->GetTileState() != TileType::WallTile && m_IsTileVisited[neighbourTile] == false)
		{
			return neighbourTile;
		}
	}

	return nullptr;
}

Tile* DepthFirstSearch::ProcessNextTile()
{
	Tile* pTile = m_pOpenTiles.top();

	m_IsTileVisited[pTile] = true;

	if (pTile->GetTileAnimationState() == TileAnimationState::Processed)
	{
		AddToTileAnimationArray(pTile, TileAnimationState::ReProcessing);
	}

	AddToTileAnimationArray(pTile, TileAnimationState::Processed);

	if (pTile == m_pGrid->GetEndTile()) { m_pathfound = true; }
	

	Tile* nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Down);
	if (nextNeighbourTile) { m_ClosestPreviousTile.insert_or_assign(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Up);
	if (nextNeighbourTile) { m_ClosestPreviousTile.insert_or_assign(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Right);
	if (nextNeighbourTile) { m_ClosestPreviousTile.insert_or_assign(nextNeighbourTile, pTile); return nextNeighbourTile; }
	
	nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Left);
	if (nextNeighbourTile) { m_ClosestPreviousTile.insert_or_assign(nextNeighbourTile, pTile); return nextNeighbourTile; }

	//if no unvisited neighbour found, pop the front tile from the list.
	m_pOpenTiles.pop();

	return nullptr;

}

void DepthFirstSearch::AddToTileAnimationArray(Tile* pTile, TileAnimationState tileAnimation)
{
	if (pTile->GetTileCoord() != m_pGrid->GetStartTile()->GetTileCoord() && pTile->GetTileCoord() != m_pGrid->GetEndTile()->GetTileCoord())
	{
		m_PendingTileAnimation.push(std::make_pair(pTile, tileAnimation)); //For Visualization
	}
}

//Function to draw the final path found
void DepthFirstSearch::GetFinalPathAnimationSequence()
{
	std::stack<Tile*> FinalPath;

	Tile* pEndTile = m_pGrid->GetEndTile();
	Tile* pStartTile = m_pGrid->GetStartTile();

	bool pathfinsihed = false;
	Tile* pTile = m_ClosestPreviousTile[pEndTile];

	while (!pathfinsihed)
	{
		FinalPath.push(pTile);

		pTile = m_ClosestPreviousTile[FinalPath.top()];

		pathfinsihed = pTile->GetTileCoord() == pStartTile->GetTileCoord();
	}

	while (!FinalPath.empty())
	{
		m_PendingTileAnimation.push(std::make_pair(FinalPath.top(), TileAnimationState::Found));
		m_pathCost += FinalPath.top()->GetTileWeight();
		FinalPath.pop();
	}

	m_pathCost += pEndTile->GetTileWeight();
}

void DepthFirstSearch::Stop()
{
	//Cleanup.
	m_pathfound = false;

	m_stopExecution = false;
	m_switchSpeed = 3.0f;

	std::stack<Tile*> emptyTileQueue;
	std::swap(m_pOpenTiles, emptyTileQueue);

	std::queue<std::pair<Tile*, TileAnimationState>> emptyAnimQueue;
	std::swap(m_PendingTileAnimation, emptyAnimQueue);

	m_ClosestPreviousTile.clear();
	m_IsTileVisited.clear();
}