#pragma once

#include "IAlgorithm.h"

class Tile;

class DijkstrasAlgorithm : public IAlgorithm
{
public:
	std::shared_ptr<AlgorithmData> OnExecute(const std::shared_ptr<Grid>& pGrid) override;
};