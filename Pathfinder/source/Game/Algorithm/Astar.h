#pragma once

#include "IAlgorithm.h"

class Grid;

class AStar final : public IAlgorithm
{
public:
	std::shared_ptr<AlgorithmData> OnExecute(const std::shared_ptr<Grid>& pGrid) override;
};