#pragma once

#include "IAlgorithm.h"

class Tile;

class DijkstrasAlgorithm : public IAlgorithm
{
public:
	std::shared_ptr<AlgorithmData> OnExecute(const std::shared_ptr<Grid>& pGrid) override;

	void OnAbort() override;
	
private:
	std::vector<Tile*> m_pOpenTiles;
	std::unordered_map<Tile*, unsigned int > m_totalCostfromStartTile;
	std::unordered_map<Tile*, bool > m_IsTileVisited;

private:
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);
	Tile* GetPriorityTile(); 
};