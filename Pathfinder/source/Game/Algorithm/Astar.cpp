#include "FrameworkPCH.h"
#include "Astar.h"

#include "AlgorithmData.h"
#include "Game/Grid/Grid.h"
#include "Game/Grid/Tile.h"
#include "Game/Grid/TileType.h"

std::shared_ptr<AlgorithmData> AStar::OnExecute(const std::shared_ptr<Grid>& pGrid)
{
	std::vector<std::pair<int, uint32_t>> openTiles;
	std::vector<std::pair<int, int>> minCost(pGrid->GetTiles().size(), { 1000000, 1000000 });

	const auto& endCoord = static_cast<sf::Vector2i>(pGrid->GetEndTile()->Coord);

	int pathCost = 0;
	int tilesExplored = 0;
	std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>> tailSequence;

	openTiles.emplace_back(0, pGrid->GetStartIndex());
	minCost[pGrid->GetStartIndex()] = { 0, 0 };

	while (!openTiles.empty())
	{
		tilesExplored++;
		uint32_t visitingIndex = 0;

		int lowestFCost = INT_MAX;
		int indexToPop = 0;

		for (int i = 0; i < openTiles.size(); i++)
		{
			const int currentFCost = minCost[openTiles[i].second].first + minCost[openTiles[i].second].second;

			if (currentFCost <= lowestFCost)
			{
				lowestFCost = currentFCost;
				visitingIndex = openTiles[i].second;
				indexToPop = i;
			}
		}
		const auto& visitingTile = pGrid->GetTile(visitingIndex);
		openTiles.erase(openTiles.begin() + indexToPop);


		if (visitingIndex != pGrid->GetStartIndex() && visitingIndex != pGrid->GetEndIndex())
		{
			m_animationSequence.push({ visitingTile, PROCESSED_TILE_COLOR });
		}

		// If the path is discovered.
		if (visitingTile->Type == TileType::EndTile)
		{
			std::stack<std::shared_ptr<Tile>> path;
			auto pTile = tailSequence[visitingTile];
			pathCost += visitingTile->Weight;

			while (pTile->Type != TileType::StartTile)
			{
				path.push(pTile);
				pathCost += pTile->Weight;
				pTile = tailSequence[pTile];
			}

			while (!path.empty())
			{
				m_animationSequence.push({ path.top(), FOUND_TILE_COLOR });
				path.pop();
			}
			break;
		}

		// Inserts the valid neighbors to the open list.
		for (const uint32_t& index : pGrid->GetValidNeighborIndices(visitingIndex))
		{
			const auto& neighbor = pGrid->GetTile(index);

			const int currentFCost = minCost[index].first + minCost[index].second;
			if (neighbor->Type != TileType::WallTile)
			{
				// Calculating H-cost based on Manhattan distance.
				const int gCost = minCost[visitingIndex].first + neighbor->Weight;
				const int hCost = (std::abs((int)neighbor->Coord.x - endCoord.x) +
					std::abs((int)neighbor->Coord.y - endCoord.y));
				const int fCost = gCost + hCost;

				if (fCost < currentFCost)
				{
					minCost[index] = { gCost, hCost };

					//Pushes the tile to the open list.
					openTiles.emplace_back(fCost, index);

					tailSequence[neighbor] = visitingTile;

					if (index != pGrid->GetStartIndex() && index != pGrid->GetEndIndex())
					{
						m_animationSequence.push({ neighbor, PROCESSING_TILE_COLOR });
					}
				}
			}
		}
	}

	return std::make_shared<AlgorithmData>("A* Search", InvalidData,
		pathCost == 0 ? InvalidData : std::to_string(pathCost), std::to_string(tilesExplored));
}