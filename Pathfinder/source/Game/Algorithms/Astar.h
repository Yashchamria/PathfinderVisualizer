#pragma once

#include "BaseAlgorithm.h"

class Grid;
class Tile;

class Astar : public BaseAlgorithm
{
public:
	Astar(Grid* pGrid);
	~Astar();

	virtual void OnInit() override;
	virtual void OnExecute() override;
	virtual void OnStop() override;
	
public:
	virtual std::string GetAlgorithmName() override { return "Astar Algorithm"; }

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