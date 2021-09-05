#include "FrameworkPCH.h"
#include "DepthFirstSearch.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"

DepthFirstSearch::DepthFirstSearch(Grid* pGrid) : BaseAlgorithm(pGrid) {}

DepthFirstSearch::~DepthFirstSearch()
{
	Stop();
}

void DepthFirstSearch::OnInit()
{
	//Intializing all the tiles to unvisited.
	for (unsigned int x = 0; x < GetGrid()->GetGridSize().x; x++)
	{
		for (unsigned int y = 0; y < GetGrid()->GetGridSize().y; y++)
		{
			Tile* pTile = GetGrid()->GetTile(sf::Vector2u(x, y));

			if (pTile->GetTileType() == TileType::WallTile)
				continue;

			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));
		}
	}

	//Dealing with the Start Tile
	AddToOpenlist(GetGrid()->GetStartTile());
}

void DepthFirstSearch::OnExecute()
{
	while (!m_pOpenTiles.empty() && !IsPathFound() && !IsExecutionStopped())
	{
		Tile* pTile = ProcessNextTile();

		if (pTile)
		{
			if (pTile->GetTileCoord() == GetGrid()->GetEndTile()->GetTileCoord())
			{
				SetPathFound(true);
				break;
			}
		}

		AddToOpenlist(pTile);
	}
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

	Tile* neighbourTile = GetGrid()->GetNeighbourTile(CurrentTileCoord, tileDirection);

	if (neighbourTile)
	{
		if (neighbourTile->GetTileType() != TileType::WallTile && m_IsTileVisited[neighbourTile] == false)
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
	IncrementTileExplored();

	AddToTileAnimationArray(pTile, TileAnimationState::Processed);

	Tile* nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Down);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Up);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Right);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, NeighbourTileDirection::Left);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	//if no unvisited neighbour left, pop the front tile from the list.
	m_pOpenTiles.pop();

	return nullptr;
}

void DepthFirstSearch::OnStop()
{
	std::stack<Tile*> emptyTileQueue;
	std::swap(m_pOpenTiles, emptyTileQueue);

	m_IsTileVisited.clear();
}