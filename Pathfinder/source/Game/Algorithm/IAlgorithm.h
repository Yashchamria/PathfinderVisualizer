#pragma once
#include "AlgorithmData.h"

class Tile;

class IAlgorithm
{
public:
	virtual ~IAlgorithm() = default;

	virtual AlgorithmData OnExecute(const std::vector<std::shared_ptr<Tile>> pTiles,
		const std::shared_ptr<Tile> pStartTile, const std::shared_ptr<Tile> pEndTile) = 0;

	virtual void OnAbort() = 0;
};