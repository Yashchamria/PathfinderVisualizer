#include "FrameworkPCH.h"
#include "DepthFirstSearch.h"

#include "AlgorithmData.h"
#include "Game/Grid/Tile.h"

#include "Game/Grid/TileType.h"
#include "Game/Grid/Grid.h"

std::shared_ptr<AlgorithmData> DepthFirstSearch::OnExecute(const std::shared_ptr<Grid>& pGrid)
{
	std::vector visitedTiles(pGrid->GridSize.x * pGrid->GridSize.y, false);
	std::stack<uint32_t> openTiles;
	openTiles.push(pGrid->GetStartIndex());

	int pathCost = 0;
	int tilesExplored = 0;
	std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>> tailSequence;

	while (!openTiles.empty())
	{
		tilesExplored++;
		const uint32_t visitingIndex = openTiles.top();
		const auto& visitingTile = pGrid->GetTile(visitingIndex);
	
		if (visitingIndex != pGrid->GetStartIndex() && visitingIndex != pGrid->GetEndIndex())
		{
			m_animationSequence.push({ visitingTile, PROCESSED_TILE_COLOR });
		}
		openTiles.pop();

		if(visitedTiles[visitingIndex]) { continue; }
		visitedTiles[visitingIndex] = true;
	
		// If the path is discovered.
		if (visitingTile->Type == TileType::EndTile)
		{
			std::stack<std::shared_ptr<Tile>> path;
			auto pTile = tailSequence[visitingTile];
			pathCost += pTile->Weight;
	
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
	
			if (!visitedTiles[index] && neighbor->Type != TileType::WallTile)
			{
				openTiles.push(index);
				tailSequence[neighbor] = visitingTile;
	
				if (index != pGrid->GetStartIndex() && index != pGrid->GetEndIndex())
				{
					m_animationSequence.push({ neighbor, PROCESSING_TILE_COLOR });
				}
			}
		}
	}

	return std::make_shared<AlgorithmData>("Depth First Search", InvalidData,
		pathCost == 0 ? InvalidData : std::to_string(pathCost), std::to_string(tilesExplored));
}