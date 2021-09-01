#include "FrameworkPCH.h"
#include "DijkstrasAlgorithm.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"

DijkstrasAlgorithm::DijkstrasAlgorithm(Grid* pGrid) 
{
	m_pGrid = pGrid;
	m_AlgorithmState = AlgorithmState::UnExecuted;
}

DijkstrasAlgorithm::~DijkstrasAlgorithm()
{
	m_pGrid = nullptr;
	Stop();
}

void DijkstrasAlgorithm::Init()
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

			if (pTile->GetTileState() == TileType::WallTile)
				continue;

			m_totalCostfromStartTile.insert(std::pair <Tile*, unsigned int>(pTile, UINT_MAX));
			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));
		}
	}

	//Dealing with the Start Tile
	Tile* StartTile = m_pGrid->GetStartTile();

	m_totalCostfromStartTile[StartTile] = 0;
	m_ClosestPreviousTile[StartTile] = StartTile;
	ProcessNeighbourTiles(StartTile);
	m_tilesExplored++;
}

bool DijkstrasAlgorithm::Execute(AlgorithmType algorithmType)
{
	auto algorithmStart = std::chrono::steady_clock::now(); //To calculate the time algorithm took.

	Init();

	//Main loop running the algorithm
	while (!m_pOpenTiles.empty() && !m_pathfound)
	{
		if (m_stopExecution) { return false; }

		Tile* pTile = GetPriorityTile();
		ProcessNeighbourTiles(pTile);
	}

	auto algorithmEnd = std::chrono::steady_clock::now(); //To calculate the time algorithm took.
	auto algorithmDuration = algorithmEnd - algorithmStart;
	m_algorithmDuration = (std::to_string)(std::chrono::duration <double, std::milli>(algorithmDuration).count()) + " ms";


	m_AlgorithmState = AlgorithmState::Visualizing;
	if (m_pathfound) { GetFinalPathAnimationSequence(); }
	
	return true;
}

bool DijkstrasAlgorithm::PlayVisualization(float speed, float deltaTime)
{
	if (m_PendingTileAnimation.empty())
	{
		m_AlgorithmState = AlgorithmState::Visualized;
		return true;
	}

	m_switchSpeed += (deltaTime * speed);

	if (m_switchSpeed >= 1.0f)
	{
		m_PendingTileAnimation[m_pendingAnimationIndex].first->SetTileAnimationProperty(m_PendingTileAnimation[m_pendingAnimationIndex].second);
		m_PendingTileAnimation[m_pendingAnimationIndex].first->UpdateTileAnimationProperty();

		if (m_pendingAnimationIndex < (m_PendingTileAnimation.size() - 1))
		{
			m_pendingAnimationIndex++;
		}
		else
		{
			m_AlgorithmState = AlgorithmState::Visualized;
			Stop();
			return true;
		}

		m_switchSpeed = 0.0f;
	}

	return false;
}

void DijkstrasAlgorithm::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_IsTileVisited[pTile] = true;	//Mark the tile visited.

	AddToTileAnimationArray(pTile, TileAnimationState::Processed);
	
	if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end())
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

void DijkstrasAlgorithm::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile == nullptr) { return; }
	if (!m_IsTileVisited[pTile] && pTile->GetTileState() != TileType::WallTile)
	{
		//Calculate the shortest cost for the tile from the previous tiles
		//Also stores the previous tile and the shortest distance
		unsigned int newCost = m_totalCostfromStartTile[pPreviousTile] + pTile->GetTileWeight();

		if (newCost <= m_totalCostfromStartTile[pTile])
		{
			m_totalCostfromStartTile[pTile] = newCost;

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

void DijkstrasAlgorithm::AddToTileAnimationArray(Tile* pTile, TileAnimationState tileAnimation)
{
	if (pTile->GetTileCoord() != m_pGrid->GetStartTile()->GetTileCoord() && pTile->GetTileCoord() != m_pGrid->GetEndTile()->GetTileCoord())
	{
		m_PendingTileAnimation.push_back(std::make_pair(pTile, tileAnimation)); //For Visualization
	}
}

//Searches the open list and returns the tile with the shortest cost from the start.
Tile* DijkstrasAlgorithm::GetPriorityTile()
{
	std::pair<Tile*,unsigned int> MinDistanceTile = std::make_pair(nullptr , UINT_MAX);

	for (Tile* pTile : m_pOpenTiles)
	{
		if (m_totalCostfromStartTile.find(pTile) != m_totalCostfromStartTile.end()) 
		{
			if (m_totalCostfromStartTile[pTile] <= MinDistanceTile.second)
			{
				MinDistanceTile = std::make_pair(pTile, m_totalCostfromStartTile[pTile]);

				if (pTile == m_pGrid->GetEndTile()) { m_pathfound = true; break; } // Path found Condition
			}
		}
	}
	
	return MinDistanceTile.first;
}

//Function to draw the final path found
void DijkstrasAlgorithm::GetFinalPathAnimationSequence()
{
	std::vector<Tile*> FinalPath;

	Tile* pEndTile = m_pGrid->GetEndTile();
	Tile* pStartTile = m_pGrid->GetStartTile();

	Tile* pTile = nullptr;
	bool pathfinsihed = false;

	FinalPath.push_back(pEndTile);

	while (!pathfinsihed)
	{
		pTile = m_ClosestPreviousTile[FinalPath[FinalPath.size() - 1]];
		pathfinsihed = pTile->GetTileCoord().x == pStartTile->GetTileCoord().x && pTile->GetTileCoord().y == pStartTile->GetTileCoord().y;

		FinalPath.push_back(pTile);
	}

	for (int i = FinalPath.size() - 2; i >= 1; i--)
	{
		m_PendingTileAnimation.push_back(std::make_pair(FinalPath[i], TileAnimationState::Found)); //For Visualization
		m_pathCost += FinalPath[i]->GetTileWeight(); //Adding cost for all the intermediate tiles from Start to End
	}

	m_pathCost += pEndTile->GetTileWeight(); //Adding cost to go to the end tile.
}

void DijkstrasAlgorithm::Stop()
{
	//Cleanup
	m_pathfound = false;

	m_stopExecution = false;
	m_pendingAnimationIndex = 0;
	m_switchSpeed = 3.0f;

	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	m_PendingTileAnimation.clear();
	m_PendingTileAnimation.shrink_to_fit();

	m_totalCostfromStartTile.clear();
	m_ClosestPreviousTile.clear();
	m_IsTileVisited.clear();
}
