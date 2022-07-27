#include "FrameworkPCH.h"
#include "DijkstrasAlgorithm.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"

DijkstrasAlgorithm::DijkstrasAlgorithm(Grid* pGrid) : BaseAlgorithm(pGrid) {}

DijkstrasAlgorithm::~DijkstrasAlgorithm()
{
	Stop();
}

void DijkstrasAlgorithm::OnInit()
{
	//Intializing all the tiles to unvisited and setting the distance to infinite.
	for (unsigned int x = 0; x < GetGrid()->GetGridSize().x; x++)
	{
		for (unsigned int y = 0; y < GetGrid()->GetGridSize().y; y++)
		{
			Tile* pTile = GetGrid()->GetTile(sf::Vector2u(x, y)).get();
	
			if (pTile->GetTileType() == TileType::WallTile)
				continue;
	
			m_totalCostfromStartTile.insert(std::make_pair(pTile, UINT_MAX));
			m_IsTileVisited.insert(std::make_pair(pTile, false));
		}
	}

	//Dealing with the Start Tile
	Tile* StartTile = GetGrid()->GetStartTile().get();

	m_totalCostfromStartTile[StartTile] = 0;
	AddToClosestPreviousTile(StartTile, StartTile);
	ProcessNeighbourTiles(StartTile);
	IncrementTileExplored();

}

void DijkstrasAlgorithm::OnExecute()
{
	while (!m_pOpenTiles.empty() && !IsPathFound() && !IsExecutionStopped())
	{
		Tile* pTile = GetPriorityTile();

		if (pTile)
		{
			if (pTile->GetTileCoord() == GetGrid()->GetEndTile()->GetTileCoord())
			{
				SetPathFound(true);
				break;
			}
		}

		ProcessNeighbourTiles(pTile);
	}
}

void DijkstrasAlgorithm::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_IsTileVisited[pTile] = true;

	AddToTileAnimationArray(pTile, TileAnimState::Processed);
	
	if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end())
	{
		m_pOpenTiles.erase(std::remove(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile), m_pOpenTiles.end());
	}

	//Look for neighboring tiles and update them.
	sf::Vector2u CurrentTileCoord = pTile->GetTileCoord();

	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Up).get()   , pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Down).get() , pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Right).get(), pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Left).get() , pTile);
}

void DijkstrasAlgorithm::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile == nullptr) { return; }
	if (!m_IsTileVisited[pTile] && pTile->GetTileType() != TileType::WallTile)
	{
		//Stores the previous tile and updates the total cost if valid.
		unsigned int newCost = m_totalCostfromStartTile[pPreviousTile] + pTile->GetTileWeight();

		if (newCost <= m_totalCostfromStartTile[pTile])
		{
			m_totalCostfromStartTile[pTile] = newCost;

			AddToClosestPreviousTile(pTile, pPreviousTile);
		}

		//Pushes the tile to the open list.
		if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) == m_pOpenTiles.end())
		{
			AddToTileAnimationArray(pTile, TileAnimState::Processing);

			m_pOpenTiles.push_back(pTile);
			IncrementTileExplored();
		}
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
			}
		}
	}
	
	return MinDistanceTile.first;
}

void DijkstrasAlgorithm::OnStop()
{
	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	m_totalCostfromStartTile.clear();
	m_IsTileVisited.clear();
}
