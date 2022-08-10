#include "FrameworkPCH.h"
#include "DijkstrasAlgorithm.h"

#include "AlgorithmData.h"
#include "Game/Grid/Grid.h"
#include "Game/Grid/Tile.h"
#include "Game/Grid/TileType.h"

std::shared_ptr<AlgorithmData> DijkstrasAlgorithm::OnExecute(const std::shared_ptr<Grid>& pGrid)
{
	std::priority_queue<std::pair<int, uint32_t>, std::vector<std::pair<int, uint32_t>>, std::greater<>> openTiles;
	std::vector<int> minCost(pGrid->GetTiles().size(), INT_MAX);

	int pathCost = 0;
	int tilesExplored = 0;
	std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>> tailSequence;

	openTiles.push({0, pGrid->GetStartIndex()});
	minCost[pGrid->GetStartIndex()] = 0;

	while (!openTiles.empty())
	{
		tilesExplored++;
		const uint32_t visitingIndex = openTiles.top().second;
		const auto& visitingTile = pGrid->GetTile(visitingIndex);

		openTiles.pop();

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

			if (neighbor->Type != TileType::WallTile)
			{
				const int currentCost = minCost[visitingIndex] + neighbor->Weight;

				if (currentCost < minCost[index])
				{
					minCost[index] = currentCost;
					openTiles.push({currentCost, index});

					tailSequence[neighbor] = visitingTile;

					if (index != pGrid->GetStartIndex() && index != pGrid->GetEndIndex())
					{
						m_animationSequence.push({ neighbor, PROCESSING_TILE_COLOR });
					}
				}
			}
		}
	}

	return std::make_shared<AlgorithmData>(R"(Dijkstra's shortest path)", InvalidData,
		pathCost == 0 ? InvalidData : std::to_string(pathCost), std::to_string(tilesExplored));
}