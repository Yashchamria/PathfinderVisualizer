#pragma once

#include "BaseAlgorithm.h"

class Grid;
class Tile;

class DijkstrasAlgorithm : public BaseAlgorithm
{
public:
	DijkstrasAlgorithm(Grid* pGrid);
	~DijkstrasAlgorithm();

	virtual void OnInit() override;
	virtual void OnExecute() override;
	virtual void OnStop() override;
	
private:
	std::vector<Tile*> m_pOpenTiles;
	std::unordered_map<Tile*, unsigned int > m_totalCostfromStartTile;
	std::unordered_map<Tile*, bool > m_IsTileVisited;

private:
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);
	Tile* GetPriorityTile(); 

public:
	virtual std::string GetAlgorithmName() override { return "Dijkstra's Algorithm"; }
};