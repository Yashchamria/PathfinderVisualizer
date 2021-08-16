#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;

class DijkstrasAlgorithm : public IAlgorithm
{

public:
	DijkstrasAlgorithm(Grid* grid);

	virtual void Execute() override;
	
private:
	void Init();

private:
	Grid* m_pGrid = nullptr;

	std::vector<Tile*>m_pOpenTiles;
	std::map<Tile*, unsigned int > m_totalCostfromStartTile;
	std::map<Tile*, Tile* > m_ClosestPreviousTile;
	std::map<Tile*, bool > m_IsTileVisited;

private:
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);

	Tile* GetPriorityTile(); 

	void Cleanup();
};