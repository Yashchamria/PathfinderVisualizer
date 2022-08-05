#pragma once

#include "IAlgorithm.h"

class Tile;
enum class Direction : char;

class DepthFirstSearch : public IAlgorithm
{
public:
	std::shared_ptr<AlgorithmData> OnExecute(const std::shared_ptr<Grid>& pGrid) override;
};