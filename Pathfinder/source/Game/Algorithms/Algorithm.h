//Algorithm will work as an adapter for different pathfinding algorithms.

#pragma once
#include "IAlgorithm.h"

class Grid;
class DijkstrasAlgorithm;

class Algorithm : public IAlgorithm
{
public:
	Algorithm(Grid* pGrid);
	~Algorithm();

public:
	virtual bool Execute(AlgorithmType algorithmType) override;
	virtual bool PlayVisualization(float speed, float deltaTime) override;
	virtual void Stop() override;
	virtual AlgorithmState GetAlgorithmState() override;

public:
	virtual std::string GetAlgorithmName() override { return 0; }
	virtual std::string GetTimeTaken() override { return 0; }
	virtual std::string GetTotalCost() override { return 0; }
	virtual std::string GetTilesExplored() override { return 0; }

private:
	Grid* m_pGrid = nullptr;

	IAlgorithm* m_pCurrentAlgorithm = nullptr;
	DijkstrasAlgorithm* m_pDijkstrasAlgorithm = nullptr;


};