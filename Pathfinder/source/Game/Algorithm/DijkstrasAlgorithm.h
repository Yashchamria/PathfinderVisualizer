#pragma once

#include "IAlgorithm.h"

class Tile;

class DijkstrasAlgorithm : public IAlgorithm
{
public:
	AlgorithmData OnExecute(const std::vector<std::shared_ptr<Tile>> pTiles,
		const std::shared_ptr<Tile> pStartTile, const std::shared_ptr<Tile> pEndTile) override;

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