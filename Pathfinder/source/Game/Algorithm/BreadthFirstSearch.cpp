#include "FrameworkPCH.h"
#include "BreadthFirstSearch.h"

#include "AlgorithmData.h"
#include "Game/Grid/Grid.h"
#include "Game/Grid/Tile.h"
#include "Game/Grid/TileType.h"

std::shared_ptr<AlgorithmData> BreadthFirstSearch::OnExecute(const std::shared_ptr<Grid>& pGrid)
{
	std::vector visitedTiles(pGrid->GridSize.x * pGrid->GridSize.y, false);
	std::queue<uint32_t> openTiles;
	openTiles.push(pGrid->GetStartIndex());

	int pathCost = 0;
	int tilesExplored = 0;
	std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>> tailSequence;

	while(!openTiles.empty())
	{
		++tilesExplored;
		const uint32_t visitingIndex = openTiles.front();
		const auto& visitingTile = pGrid->GetTile(visitingIndex);

		if(visitingIndex != pGrid->GetStartIndex() && visitingIndex != pGrid->GetEndIndex())
		{
			m_animationSequence.push({visitingTile, PROCESSED_TILE_COLOR});
		}
		openTiles.pop();

		visitedTiles[visitingIndex] = true;

		// If the path is discovered.
		if(visitingTile->Type == TileType::EndTile)
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

			while(!path.empty())
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

			if(!visitedTiles[index] && neighbor->Type != TileType::WallTile)
			{
				openTiles.push(index);
				visitedTiles[index] = true;
				tailSequence[neighbor] = visitingTile;

				if (index != pGrid->GetStartIndex() && index != pGrid->GetEndIndex())
				{
					m_animationSequence.push({neighbor, PROCESSING_TILE_COLOR});
				}
			}
		}
	}

	return std::make_shared<AlgorithmData>("Breadth First Search", InvalidData,
		pathCost == 0 ? InvalidData : std::to_string(pathCost), std::to_string(tilesExplored));
}