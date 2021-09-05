#pragma once

#include "BaseAlgorithm.h"

class Grid;
class Tile;

class DepthFirstSearch : public BaseAlgorithm
{
public:
	DepthFirstSearch(Grid* pGrid);
	~DepthFirstSearch();

	virtual void OnInit() override;
	virtual void OnExecute() override;
	virtual void OnStop() override;

private:
	std::stack<Tile*> m_pOpenTiles; //Requires LIFO
	std::unordered_map<Tile*, bool > m_IsTileVisited;

private:
	Tile* ProcessNextTile();

	void AddToOpenlist(Tile* pTile);
	Tile* GetNeighbourTile(Tile* pCurrentTile, NeighbourTileDirection tileDirection);

public:
	virtual std::string GetAlgorithmName() override { return "Depth First Search"; }
	virtual std::string GetTotalCost() override { return (std::to_string(GetPathCost())) + "(Unweighted Algorithm)"; }
};