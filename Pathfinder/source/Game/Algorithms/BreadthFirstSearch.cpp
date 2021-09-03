#include "FrameworkPCH.h"
#include "BreadthFirstSearch.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"

BreadthFirstSearch::BreadthFirstSearch(Grid* pGrid)
{
	m_pGrid = pGrid;
	m_AlgorithmState = AlgorithmState::UnExecuted;
}

BreadthFirstSearch::~BreadthFirstSearch()
{
	m_pGrid = nullptr;
	Stop();
}

void BreadthFirstSearch::Init()
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

			if (pTile->GetTileType() == TileType::WallTile)
				continue;

			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));
		}
	}

	//Dealing with the Start Tile
	Tile* StartTile = m_pGrid->GetStartTile();

	m_ClosestPreviousTile[StartTile] = StartTile;
	ProcessNeighbourTiles(StartTile);
	m_tilesExplored++;
}

bool BreadthFirstSearch::Execute(AlgorithmType algorithmType)
{
	//To calculate the time algorithm took
	auto algorithmStart = std::chrono::steady_clock::now();

	Init();

	//Main loop running the algorithm
	while (!m_pOpenTiles.empty() && !m_pathfound)
	{
		if (m_stopExecution) { return false; }

		Tile* pTile = GetPriorityTile();
		ProcessNeighbourTiles(pTile);
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

bool BreadthFirstSearch::PlayVisualization(float speed, float deltaTime)
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

void BreadthFirstSearch::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_IsTileVisited[pTile] = true;	//Mark the tile visited.

	//if pTile is not start or end tile then update animation.
	if (pTile->GetTileCoord() != m_pGrid->GetStartTile()->GetTileCoord() && pTile->GetTileCoord() != m_pGrid->GetEndTile()->GetTileCoord())
	{
		m_PendingTileAnimation.push(std::make_pair(pTile, TileAnimationState::Processed)); //For Visualization
	}

	//If exist in open list then remove the tile.
	if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end())
	{
		m_pOpenTiles.erase(std::remove(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile), m_pOpenTiles.end());
	}

	//Look for neighbouring tiles and update them //Doesn't support Diagonals yet
	sf::Vector2u CurrentTileCoord = pTile->GetTileCoord();

	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Up), pTile);
	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Down), pTile);
	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Right), pTile);
	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Left), pTile);
}

void BreadthFirstSearch::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile == nullptr) { return; }

	if (!m_IsTileVisited[pTile] && pTile->GetTileType() != TileType::WallTile)
	{
		//if the tile doesn't exist then only insert otherwise there already has to be a shorter route to reach it.
		if (m_ClosestPreviousTile.find(pTile) == m_ClosestPreviousTile.end()) //not found
		{
			m_ClosestPreviousTile.insert_or_assign(pTile, pPreviousTile);
		}

		//Pushes the tile to the open list.
		if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) == m_pOpenTiles.end())
		{
			if (pTile->GetTileCoord() != m_pGrid->GetStartTile()->GetTileCoord() && pTile->GetTileCoord() != m_pGrid->GetEndTile()->GetTileCoord())
			{
				m_PendingTileAnimation.push(std::make_pair(pTile, TileAnimationState::Processing)); //For Visualization
			}

			m_pOpenTiles.push_back(pTile);
			m_tilesExplored++;
		}
	}
}

//Searches the open list and returns the closest tile from the start.
Tile* BreadthFirstSearch::GetPriorityTile()
{
	if (!m_pOpenTiles.empty())
	{
		Tile* pTile = m_pOpenTiles.at(0);

		if (pTile == m_pGrid->GetEndTile())
		{
			m_pathfound = true;
		}

		return pTile;
	}

	return nullptr;
}

void BreadthFirstSearch::GetFinalPathAnimationSequence()
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

void BreadthFirstSearch::Stop()
{
	//Cleanup.
	m_pathfound = false;

	m_stopExecution = false;
	m_switchSpeed = 3.0f;

	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	std::queue<std::pair<Tile*, TileAnimationState>> empty;
	std::swap(m_PendingTileAnimation, empty);

	m_ClosestPreviousTile.clear();
	m_IsTileVisited.clear();
}