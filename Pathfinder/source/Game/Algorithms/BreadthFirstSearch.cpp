#include "FrameworkPCH.h"
#include "BreadthFirstSearch.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileEnum.h"

BreadthFirstSearch::BreadthFirstSearch(Grid* pGrid) : BaseAlgorithm(pGrid) {}

BreadthFirstSearch::~BreadthFirstSearch()
{
	Stop();
}

void BreadthFirstSearch::OnInit()
{
	//Intializing all the tiles to unvisited.
	for (unsigned int x = 0; x < GetGrid()->GetGridSize().x; x++)
	{
		for (unsigned int y = 0; y < GetGrid()->GetGridSize().y; y++)
		{
			Tile* pTile = GetGrid()->GetTile(sf::Vector2u(x, y)).get();

			if (pTile->Type == TileType::WallTile)
				continue;

			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));
		}
	}

	//Dealing with the Start Tile
	Tile* StartTile = GetGrid()->GetStartTile().get();

	AddToClosestPreviousTile(StartTile, StartTile);
	ProcessNeighbourTiles(StartTile);
	IncrementTileExplored();
}

void BreadthFirstSearch::OnExecute()
{
	while (!m_pOpenTiles.empty() && !IsPathFound() && !IsExecutionStopped())
	{
		Tile* pTile = GetPriorityTile();

		if (pTile)
		{
			if (pTile->GetCoord() == GetGrid()->GetEndTile()->GetCoord()) 
			{
				SetPathFound(true); 
				break; 
			}
		}

		ProcessNeighbourTiles(pTile);
	}
}

void BreadthFirstSearch::ProcessNeighbourTiles(Tile* pTile)
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
	sf::Vector2u CurrentTileCoord = pTile->GetCoord();

	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Up).get(), pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Down).get(), pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Right).get(), pTile);
	ProcessTileParameters(GetGrid()->GetNeighborTile(CurrentTileCoord, Direction::Left).get(), pTile);
}

void BreadthFirstSearch::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
	if (pTile == nullptr) { return; }

	if (!m_IsTileVisited[pTile] && pTile->Type != TileType::WallTile)
	{
		AddToClosestPreviousTile(pTile, pPreviousTile);

		//Pushes the tile to the open list.
		if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) == m_pOpenTiles.end())
		{
			AddToTileAnimationArray(pTile, TileAnimState::Processing);

			m_pOpenTiles.push_back(pTile);
			IncrementTileExplored();
		}
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

void BreadthFirstSearch::OnStop()
{
	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	m_IsTileVisited.clear();
}