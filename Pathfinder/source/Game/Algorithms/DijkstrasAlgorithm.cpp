#include "FrameworkPCH.h"
#include "DijkstrasAlgorithm.h"

#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"

DijkstrasAlgorithm::DijkstrasAlgorithm(Grid* grid)
{
	m_pGrid = grid;
}

void DijkstrasAlgorithm::Execute()
{
	if (m_pGrid->GetStartTile() == nullptr || m_pGrid->GetEndTile() == nullptr)
	{
		std::cout << "Please set a start tile by pressing 'S' & set a End tile by pressing 'E'\n";
		return;
	}

	std::cout << "Executing Dijkstra...\n\n";

	auto algorithmStart = std::chrono::steady_clock::now();

	Init();

	bool pathFound = false;

	while (!m_pOpenTiles.empty())
	{
		Tile* pTile = GetPriorityTile();

		if (pTile == m_pGrid->GetEndTile()) { pathFound = true; break; }

		ProcessNeighbourTiles(pTile);

		for (auto const& x : m_IsTileVisited) { if (x.second) { x.first->SetTileColor(sf::Color::Magenta); } }
		for (Tile* pTile : m_pOpenTiles) { pTile->SetTileColor(sf::Color::Cyan); }
	}

	auto algorithmEnd = std::chrono::steady_clock::now();

	auto algorithmDuration = algorithmEnd - algorithmStart;

	if (pathFound)
	{
		std::cout << "Dijkstra Path found!\n";
	}
	else
	{
		std::cout << "Dijkstra Path not found!\n\n\n";
	}

	std::cout << "Dijkstra's Duration - " << std::chrono::duration <double, std::milli>(algorithmDuration).count() << " ms\n\n";

	Cleanup();
}

void DijkstrasAlgorithm::Init()
{
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
	m_ClosestPreviousTile[StartTile] = m_pGrid->GetStartTile();

	ProcessNeighbourTiles(StartTile);
	m_IsTileVisited[StartTile] = true;

}

void DijkstrasAlgorithm::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	//Mark the tile visited and pop out from the open list
	m_IsTileVisited[pTile] = true;

	if (std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end())
	{
		m_pOpenTiles.erase(std::remove(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile), m_pOpenTiles.end());
	}

	//Look for neighbouring tiles and update them //Doesn't support Diagonals yet
	sf::Vector2u CurrentTileCoord = pTile->GetTileCoord();

	sf::Vector2u UpperTileCoord = sf::Vector2u(CurrentTileCoord.x,     CurrentTileCoord.y - 1);
	sf::Vector2u LowerTileCoord = sf::Vector2u(CurrentTileCoord.x,     CurrentTileCoord.y + 1);
	sf::Vector2u RightTileCoord = sf::Vector2u(CurrentTileCoord.x + 1, CurrentTileCoord.y    );
	sf::Vector2u LeftTileCoord  = sf::Vector2u(CurrentTileCoord.x - 1, CurrentTileCoord.y	 );

	if (m_pGrid->IsTileCoordValid(UpperTileCoord)) { ProcessTileParameters(m_pGrid->GetTile(UpperTileCoord), pTile); }
	if (m_pGrid->IsTileCoordValid(LowerTileCoord)) { ProcessTileParameters(m_pGrid->GetTile(LowerTileCoord), pTile); }
	if (m_pGrid->IsTileCoordValid(RightTileCoord)) { ProcessTileParameters(m_pGrid->GetTile(RightTileCoord), pTile); }
	if (m_pGrid->IsTileCoordValid(LeftTileCoord))  { ProcessTileParameters(m_pGrid->GetTile(LeftTileCoord ), pTile); }
}

void DijkstrasAlgorithm::ProcessTileParameters(Tile* pTile, Tile* pPreviousTile)
{
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
		if (!(std::find(m_pOpenTiles.begin(), m_pOpenTiles.end(), pTile) != m_pOpenTiles.end()))
		{
			m_pOpenTiles.push_back(pTile);
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

void DijkstrasAlgorithm::Cleanup()
{
	m_pOpenTiles.clear();
	m_pOpenTiles.shrink_to_fit();

	m_totalCostfromStartTile.clear();
	m_ClosestPreviousTile.clear();
	m_IsTileVisited.clear();
}