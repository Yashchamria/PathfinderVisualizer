#include "FrameworkPCH.h"
#include "BreadthFirstSearch.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileType.h"
#include "Game/Objects/Grid/Direction.h"

AlgorithmData BreadthFirstSearch::OnExecute(const std::vector<std::shared_ptr<Tile>> pTiles,
	const std::shared_ptr<Tile> pStartTile, const std::shared_ptr<Tile> pEndTile)
{
	//Intializing all the tiles to unvisited.
	/*for (int x = 0; x < GetGrid()->GridSize.x; x++)
	{
		for (int y = 0; y < GetGrid()->GridSize.y; y++)
		{
			Tile* pTile = GetGrid()->GetTile(sf::Vector2u(x, y)).get();

			if (pTile->Type == TileType::WallTile)
				continue;

			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));
		}
	}

	AddToClosestPreviousTile(pStartTile, pStartTile);
	ProcessNeighbourTiles(pStartTile);
	IncrementTileExplored();

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
	}*/
	return AlgorithmData();
}

void BreadthFirstSearch::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_IsTileVisited[pTile] = true;

	//AddToTileAnimationArray(pTile, TileAnimState::Processed);

	//Pop from the open list if exist
	if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end())
	{
		m_pOpenTiles.erase(std::remove(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile), m_pOpenTiles.end());
	}

	//Look for neighbouring tiles and update them
	sf::Vector2u CurrentTileCoord = pTile->Coord;

	//ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Up).get(), pTile);
	//ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Down).get(), pTile);
	//ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Right).get(), pTile);
	//ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Left).get(), pTile);
}

void BreadthFirstSearch::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile == nullptr) { return; }

	if (!m_IsTileVisited[pTile] && pTile->Type != TileType::WallTile)
	{
		//AddToClosestPreviousTile(pTile, pPreviousTile);

		//Pushes the tile to the open list.
		//if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) == m_pOpenTiles.end())
		//{
		//	AddToTileAnimationArray(pTile, TileAnimState::Processing);
		//
		//	m_pOpenTiles.push_back(pTile);
		//	IncrementTileExplored();
		//}
	}
}

Tile* BreadthFirstSearch::GetPriorityTile()
{
	if (!m_pOpenTiles.empty())
	{
		return m_pOpenTiles.front();
	}

	return nullptr;
}

void BreadthFirstSearch::OnAbort()
{
	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	m_IsTileVisited.clear();
}