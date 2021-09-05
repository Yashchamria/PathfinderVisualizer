#include "FrameworkPCH.h"
#include "Astar.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"

Astar::Astar(Grid* pGrid)
{
	m_pGrid = pGrid;
	m_AlgorithmState = AlgorithmState::UnExecuted;
}

Astar::~Astar()
{
	m_pGrid = nullptr;
	Stop();
}

void Astar::Init()
{
	m_tilesExplored = 0;
	m_pathCost = 0;
	m_AlgorithmState = AlgorithmState::Executing;


	//Intializing all the tiles to unvisited and setting the distance to infinite.
	for (unsigned int x = 0; x < m_pGrid->GetGridSize().x; x++)
	{
		for (unsigned int y = 0; y < m_pGrid->GetGridSize().y; y++)
		{
			Tile* pTile = m_pGrid->GetTile(sf::Vector2u(x, y));
	
			if (pTile->GetTileType() == TileType::WallTile)
				continue;
	
			m_finalCost.insert(std::make_pair(pTile, std::make_pair(UINT_MAX, UINT_MAX)));
			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));

			m_averageTileWeight += pTile->GetTileWeight();
		}
	}

	m_averageTileWeight /= m_IsTileVisited.size();

	//Dealing with the Start Tile
	Tile* StartTile = m_pGrid->GetStartTile();

	m_finalCost[StartTile] = std::make_pair(0, 0);
	m_ClosestPreviousTile[StartTile] = StartTile;
	ProcessNeighbourTiles(StartTile);
	m_tilesExplored++;
}

bool Astar::Execute(AlgorithmType algorithmType)
{
	auto algorithmStart = std::chrono::steady_clock::now(); //To calculate the time algorithm took.

	Init();

	//Main loop running the algorithm
	while (!m_pOpenTiles.empty() && !m_pathfound)
	{
		if (m_stopExecution) { return false; }

		Tile* pTile = GetPriorityTile();
		if (pTile == m_pGrid->GetEndTile()) { m_pathfound = true; break; } // Path found Condition

		ProcessNeighbourTiles(pTile);
	}

	auto algorithmEnd = std::chrono::steady_clock::now(); //To calculate the time algorithm took.
	auto algorithmDuration = algorithmEnd - algorithmStart;
	m_algorithmDuration = (std::to_string)(std::chrono::duration <double, std::milli>(algorithmDuration).count());
	m_algorithmDuration = m_algorithmDuration.substr(0, 6) + " ms";

	m_AlgorithmState = AlgorithmState::Visualizing;
	if (m_pathfound) { GetFinalPathAnimationSequence(); }
	
	return true;
}

bool Astar::PlayVisualization(float speed, float deltaTime)
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

void Astar::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_IsTileVisited[pTile] = true;	//Mark the tile visited.

	AddToTileAnimationArray(pTile, TileAnimationState::Processed);
	
	//Pop from the open list
	if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end()) //found
	{
		m_pOpenTiles.erase(std::remove(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile), m_pOpenTiles.end());
	}

	//Look for neighbouring tiles and update them //Doesn't support Diagonals yet
	sf::Vector2u CurrentTileCoord = pTile->GetTileCoord();

	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Up)   , pTile);
	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Down) , pTile);
	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Right), pTile);
	ProcessTileParameters(m_pGrid->GetNeighbourTile(CurrentTileCoord, NeighbourTileDirection::Left) , pTile);
}

void Astar::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile == nullptr) { return; }

	if (!m_IsTileVisited[pTile] && pTile->GetTileType() != TileType::WallTile)
	{
		//Calculate the shortest cost for the tile from the previous tiles
		//Also stores the previous tile and the shortest distance
		unsigned int newGCost = m_finalCost[pPreviousTile].first + pTile->GetTileWeight();
		unsigned int newHCost = GetTileHCost(pTile);

		unsigned int newFCost = newGCost + newHCost;
		unsigned int CurrentFCost = m_finalCost[pTile].first + m_finalCost[pTile].second;

		if (newFCost <= CurrentFCost)
		{
			m_finalCost[pTile] = std::make_pair(newGCost, newHCost);
			m_ClosestPreviousTile.insert_or_assign(pTile, pPreviousTile);
		}

		//Pushes the tile to the open list.
		if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) == m_pOpenTiles.end())
		{
			AddToTileAnimationArray(pTile, TileAnimationState::Processing);

			m_pOpenTiles.push_back(pTile);
			m_tilesExplored++;
		}
	}
}

void Astar::AddToTileAnimationArray(Tile* pTile, TileAnimationState tileAnimation)
{
	if (pTile->GetTileCoord() != m_pGrid->GetStartTile()->GetTileCoord() && pTile->GetTileCoord() != m_pGrid->GetEndTile()->GetTileCoord())
	{
		m_PendingTileAnimation.push(std::make_pair(pTile, tileAnimation)); //For Visualization
	}
}

//Searches the open list and returns the tile with the shortest cost from the start.
Tile* Astar::GetPriorityTile()
{
	std::pair<Tile*, unsigned int> MinDistanceTile = std::make_pair(nullptr, UINT_MAX);

	unsigned int CurrentFCost = UINT_MAX;

	for (Tile* pTile : m_pOpenTiles)
	{
		if (m_finalCost.find(pTile) != m_finalCost.end()) //found
		{
			CurrentFCost = m_finalCost[pTile].first + m_finalCost[pTile].second;

			if (CurrentFCost <= MinDistanceTile.second)
			{
				MinDistanceTile = std::make_pair(pTile, CurrentFCost);

			//	if (pTile == m_pGrid->GetEndTile()) { m_pathfound = true; break; } // Path found Condition
			}
		}
	}
	
	return MinDistanceTile.first;
}

unsigned int Astar::GetTileHCost(Tile* pTile)
{
	sf::Vector2u finalTileCoord = m_pGrid->GetEndTile()->GetTileCoord();
	sf::Vector2u CurrentTileCoord = pTile->GetTileCoord();

	unsigned int xValue = (unsigned int)std::abs((int)CurrentTileCoord.x - (int)finalTileCoord.x);
	unsigned int yValue = (unsigned int)std::abs((int)CurrentTileCoord.y - (int)finalTileCoord.y);

	unsigned int hcost = (xValue + yValue) * m_averageTileWeight;

	return hcost;
}

void Astar::GetFinalPathAnimationSequence()
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

	m_pathCost += pEndTile->GetTileWeight(); //Adding cost to go to the end tile.
}

void Astar::Stop()
{
	//Cleanup
	m_pathfound = false;

	m_stopExecution = false;
	m_switchSpeed = 3.0f;

	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	std::queue<std::pair<Tile*, TileAnimationState>> empty;
	std::swap(m_PendingTileAnimation, empty);

	m_finalCost.clear();
	m_ClosestPreviousTile.clear();
	m_IsTileVisited.clear();
}
