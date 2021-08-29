#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;
enum class TileAnimationState : char;
enum class AlgorithmState : char;

class DepthFirstSearch : public IAlgorithm
{
public:
	DepthFirstSearch(Grid* pGrid);
	~DepthFirstSearch();
	

	virtual bool Execute(AlgorithmType algorithmType) override;
	virtual bool PlayVisualization(float speed, float deltaTime) override;
	virtual void Stop() override;
	virtual AlgorithmState GetAlgorithmState() override;
	virtual bool IsPathFound() override { return false; }
	
private:
	Grid* m_pGrid = nullptr;

public:
	virtual std::string GetAlgorithmName() override { return "Depth First Search"; }
	virtual std::string GetTimeTaken() override { return ""; }
	virtual std::string GetTotalCost() override { return ""; }
	virtual std::string GetTilesExplored() override { return ""; }
};