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

	m_AlgorithmState = AlgorithmState::Executed;
}

bool DijkstrasAlgorithm::Execute(AlgorithmType algorithmType)
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

	m_algorithmDuration = (std::to_string)(std::chrono::duration <double, std::milli>(algorithmDuration).count()) + " ms";

	m_AlgorithmState = AlgorithmState::Visualizing;

	if (m_pathfound)
	{
		std::cout << "Dijkstra Path found!\nVisualizing Path...";
		GetFinalPathAnimationSequence();
	}
	else
	{
		std::cout << "Dijkstra Path not found!\nVisualizing Path...";
	}

	return true;
}

void DijkstrasAlgorithm::Init()
{
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
	m_ClosestPreviousTile[StartTile] = m_pGrid->GetStartTile();

	ProcessNeighbourTiles(StartTile);
	m_IsTileVisited[StartTile] = true;
}

void DijkstrasAlgorithm::ProcessNeighbourTiles(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	//Mark the tile visited and pop out from the open list
	m_IsTileVisited[pTile] = true;

	if(pTile->GetTileCoord() != m_pGrid->GetStartTile()->GetTileCoord() && pTile->GetTileCoord() != m_pGrid->GetEndTile()->GetTileCoord())
	{
		m_PendingTileAnimation.push_back(std::make_pair(pTile, TileAnimationState::Processed)); //For Visualization
	}
	
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
			if (pTile->GetTileCoord() != m_pGrid->GetStartTile()->GetTileCoord() && pTile->GetTileCoord() != m_pGrid->GetEndTile()->GetTileCoord())
			{
				m_PendingTileAnimation.push_back(std::make_pair(pTile, TileAnimationState::Processing)); //For Visualization
			}

			m_pOpenTiles.push_back(pTile);
			m_tilesExplored++;
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

				if (pTile == m_pGrid->GetEndTile()) { m_pathfound = true; break; } // Path found Condition
			}
		}
	}
	
	return MinDistanceTile.first;
}

bool DijkstrasAlgorithm::PlayVisualization(float speed, float deltaTime)
{
	if (m_PendingTileAnimation.empty()) 
	{
		m_AlgorithmState = AlgorithmState::Executed;

		std::cout << "Finished Path Visualization!\n\n";
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
			Cleanup();

			m_AlgorithmState = AlgorithmState::Executed;
			std::cout << "Finished Path Visualization!\n\n";
			return true;
		}

		m_switchSpeed = 0.0f;
	}

	return false;
}

void DijkstrasAlgorithm::Stop()
{
	Cleanup();
}

AlgorithmState DijkstrasAlgorithm::GetAlgorithmState()
{
	return m_AlgorithmState;
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
	}
}

void DijkstrasAlgorithm::Cleanup()
{
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

	m_algorithmDuration = "";
	m_tilesExplored = 0;
}