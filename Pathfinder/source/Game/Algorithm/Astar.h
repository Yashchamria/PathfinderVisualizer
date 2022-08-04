#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;

class AStar final : public IAlgorithm
{
public:
	std::shared_ptr<AlgorithmData> OnExecute(const std::shared_ptr<Grid>& pGrid) override;
	void OnAbort() override;
	
private:
	std::vector<Tile*> m_pOpenTiles;
	std::unordered_map<Tile*, bool > m_IsTileVisited;
	std::unordered_map<Tile*, std::pair<unsigned int, unsigned int>> m_finalCost;

private:
	Tile* GetPriorityTile(); 
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);

	unsigned int GetTileHCost(Tile* pTile);

private:
	unsigned int m_averageTileWeight = 0;

};