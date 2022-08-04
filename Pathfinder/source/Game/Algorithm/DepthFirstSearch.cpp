#include "FrameworkPCH.h"
#include "DepthFirstSearch.h"

#include "AlgorithmData.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileType.h"
#include "Game/Objects/Grid/Direction.h"

std::shared_ptr<AlgorithmData> DepthFirstSearch::OnExecute(const std::shared_ptr<Grid>& pGrid)
{
	//Intializing all the tiles to unvisited.
	/*for (auto pTile : pTiles)
	{
		if (pTile->Type != TileType::WallTile)
		{
			m_IsTileVisited.insert(std::make_pair(pTile, false));
		}
	}

	AddToOpenlist(pStartTile);

	//while (!m_pOpenTiles.empty() && !IsPathFound() && !IsExecutionStopped())
	{
		const auto& pTile = ProcessNextTile();

		if (pTile)
		{
			if (pTile->Coord == pEndTile->Coord)
			{
				//SetPathFound(true);
				//break;
			}
		}

		AddToOpenlist(pTile);
	}*/
	return std::make_shared<AlgorithmData>();
}

void DepthFirstSearch::AddToOpenlist(const std::shared_ptr<Tile>& pTile)
{
	if (pTile == nullptr) { return; }

	m_pOpenTiles.push(pTile);

	//AddToTileAnimationArray(pTile, TileAnimState::Processing);
}

std::shared_ptr<Tile> DepthFirstSearch::GetNeighbourTile(const std::shared_ptr<Tile>& pCurrentTile, const Direction tileDirection)
{
	sf::Vector2u CurrentTileCoord = pCurrentTile->Coord;

	//Tile* neighbourTile = GetGrid()->GetNeighborTile(CurrentTileCoord, tileDirection).get();
	//
	//if (neighbourTile)
	//{
	//	if (neighbourTile->Type != TileType::WallTile && m_IsTileVisited[neighbourTile] == false)
	//	{
	//		return neighbourTile;
	//	}
	//}

	return nullptr;
}

std::shared_ptr<Tile> DepthFirstSearch::ProcessNextTile()
{
	const auto& pTile = m_pOpenTiles.top();

	m_IsTileVisited[pTile] = true;
	//IncrementTileExplored();
	//
	//AddToTileAnimationArray(pTile, TileAnimState::Processed);
	//
	//Tile* nextNeighbourTile = GetNeighbourTile(pTile, Direction::Down);
	//if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }
	//
	//nextNeighbourTile = GetNeighbourTile(pTile, Direction::Up);
	//if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }
	//
	//nextNeighbourTile = GetNeighbourTile(pTile, Direction::Right);
	//if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }
	//
	//nextNeighbourTile = GetNeighbourTile(pTile, Direction::Left);
	//if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	//if no unvisited neighbour left, pop the front tile from the list.
	m_pOpenTiles.pop();

	return nullptr;
}

void DepthFirstSearch::OnAbort()
{
	std::stack<std::shared_ptr<Tile>> emptyTileQueue;
	std::swap(m_pOpenTiles, emptyTileQueue);

	m_IsTileVisited.clear();
}