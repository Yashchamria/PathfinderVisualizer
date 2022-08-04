#pragma once
#include "IAlgorithm.h"

class Grid;

class BreadthFirstSearch final : public IAlgorithm
{
public:
	std::shared_ptr<AlgorithmData> OnExecute(const std::shared_ptr<Grid>& pGrid) override;
};