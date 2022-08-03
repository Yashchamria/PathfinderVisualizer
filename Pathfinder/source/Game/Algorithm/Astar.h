#pragma once

#include "IAlgorithm.h"

class Tile;

class Astar : public IAlgorithm
{
public:
	AlgorithmData OnExecute(const std::vector<std::shared_ptr<Tile>> pTiles,
		const std::shared_ptr<Tile> pStartTile, const std::shared_ptr<Tile> pEndTile) override;

	virtual void OnAbort() override;
	
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