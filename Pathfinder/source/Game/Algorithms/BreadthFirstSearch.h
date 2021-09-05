#pragma once

#include "BaseAlgorithm.h"

class Grid;
class Tile;

class BreadthFirstSearch : public BaseAlgorithm
{
public:
	BreadthFirstSearch(Grid* pGrid);
	~BreadthFirstSearch();

	virtual void OnInit() override;
	virtual void OnExecute() override;
	virtual void OnStop() override;

public:
	virtual std::string GetAlgorithmName() override { return "Breadth First Search"; }
	virtual std::string GetTotalCost() override { return (std::to_string(GetPathCost())) + "(Unweighted Algorithm)"; }


private:
	std::vector<Tile*> m_pOpenTiles;
	std::unordered_map<Tile*, bool > m_IsTileVisited;

private:
	Tile* GetPriorityTile();
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);
};