#include "FrameworkPCH.h"
#include "Astar.h"

#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileType.h"
#include "Game/Objects/Grid/Direction.h"

Astar::Astar(Grid* pGrid) : BaseAlgorithm(pGrid) {}

Astar::~Astar()
{
	Stop();
}

void Astar::OnInit()
{
	//Intializing all the tiles to unvisited and setting the distance to infinite.
	for (unsigned int x = 0; x < GetGrid()->GridSize.x; x++)
	{
		for (unsigned int y = 0; y < GetGrid()->GridSize.y; y++)
		{
			const auto& pTile = GetGrid()->GetTile(sf::Vector2u(x, y));
	
			if (pTile->Type == TileType::WallTile)
				continue;
	
			m_finalCost.insert(std::make_pair(pTile.get(), std::make_pair(UINT_MAX, UINT_MAX)));
			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile.get(), false));

			m_averageTileWeight += pTile->Weight;
		}
	}

	m_averageTileWeight /= m_IsTileVisited.size();

	//Dealing with the Start Tile
	Tile* StartTile = GetGrid()->GetStartTile().get();

	m_finalCost[StartTile] = std::make_pair(0, 0);
	AddToClosestPreviousTile(StartTile, StartTile);
	ProcessNeighbourTiles(StartTile);
	IncrementTileExplored();

}

void Astar::OnExecute()
{
	while (!m_pOpenTiles.empty() && !IsPathFound() && !IsExecutionStopped())
	{
		Tile* pTile = GetPriorityTile();

		if (pTile)
		{
			if (pTile->Coord == GetGrid()->GetEndTile()->Coord)
			{
				SetPathFound(true);
				break;
			}
		}

		ProcessNeighbourTiles(pTile);
	}
}

void Astar::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_IsTileVisited[pTile] = true;

	AddToTileAnimationArray(pTile, TileAnimState::Processed);
	
	//Pop from the open list if exist
	if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end())
	{
		m_pOpenTiles.erase(std::remove(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile), m_pOpenTiles.end());
	}

	//Look for neighbouring tiles and update them
	sf::Vector2u CurrentTileCoord = pTile->Coord;

	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Up).get()   , pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Down).get() , pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Right).get(), pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Left).get() , pTile);
}

void Astar::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile == nullptr) { return; }

	if (!m_IsTileVisited[pTile] && pTile->Type != TileType::WallTile)
	{
		unsigned int newGCost = m_finalCost[pPreviousTile].first + pTile->Weight;
		unsigned int newHCost = GetTileHCost(pTile);

		unsigned int newFCost = newGCost + newHCost;
		unsigned int CurrentFCost = m_finalCost[pTile].first + m_finalCost[pTile].second;

		//Stores the previous tile and updates the path costs if valid.
		if (newFCost <= CurrentFCost)
		{
			m_finalCost[pTile] = std::make_pair(newGCost, newHCost);
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
Tile* Astar::GetPriorityTile()
{
	std::pair<Tile*, unsigned int> MinDistanceTile = std::make_pair(nullptr, UINT_MAX);

	unsigned int CurrentFCost = UINT_MAX;

	for (Tile* pTile : m_pOpenTiles)
	{
		if (m_finalCost.find(pTile) != m_finalCost.end())
		{
			CurrentFCost = m_finalCost[pTile].first + m_finalCost[pTile].second;

			if (CurrentFCost <= MinDistanceTile.second)
			{
				MinDistanceTile = std::make_pair(pTile, CurrentFCost);
			}
		}
	}
	
	return MinDistanceTile.first;
}

unsigned int Astar::GetTileHCost(Tile* pTile)
{
	sf::Vector2u finalTileCoord = GetGrid()->GetEndTile()->Coord;
	sf::Vector2u CurrentTileCoord = pTile->Coord;

	unsigned int xValue = (unsigned int)std::abs((int)CurrentTileCoord.x - (int)finalTileCoord.x);
	unsigned int yValue = (unsigned int)std::abs((int)CurrentTileCoord.y - (int)finalTileCoord.y);

	unsigned int hcost = (xValue + yValue) * m_averageTileWeight;

	return hcost;
}

void Astar::OnStop()
{
	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	m_finalCost.clear();
	m_IsTileVisited.clear();
}
