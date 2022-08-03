#pragma once

#include "IAlgorithm.h"

class Tile;
enum class Direction : char;

class DepthFirstSearch : public IAlgorithm
{
public:
	AlgorithmData OnExecute(const std::vector<std::shared_ptr<Tile>> pTiles,
		const std::shared_ptr<Tile> pStartTile, const std::shared_ptr<Tile> pEndTile) override;

	void OnAbort() override;

private:
	std::stack<std::shared_ptr<Tile>> m_pOpenTiles; //Requires LIFO
	std::unordered_map<std::shared_ptr<Tile>, bool > m_IsTileVisited;

private:
	std::shared_ptr<Tile> ProcessNextTile();

	void AddToOpenlist(const std::shared_ptr<Tile>& pTile);
	std::shared_ptr<Tile> GetNeighbourTile(const std::shared_ptr<Tile>& pCurrentTile, const Direction tileDirection);
};