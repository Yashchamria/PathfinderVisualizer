//Algorithm will work as an adapter for different pathfinding algorithms.

#pragma once
#include "IAlgorithm.h"

class Grid;
class Scene;

struct AlgorithmData;
class BreadthFirstSearch;
class DepthFirstSearch;
class DijkstrasAlgorithm;
class Astar;


class Algorithm : public IAlgorithm
{
public:
	Algorithm(Grid* pGrid, Scene* pScene);
	~Algorithm();

public:
	virtual bool Execute(AlgorithmType algorithmType) override;
	virtual bool PlayVisualization(float speed, float deltaTime) override;
	virtual void Stop() override;
	virtual AlgorithmState GetAlgorithmState() override;
	virtual bool IsPathFound() override;

public:
	virtual std::string GetAlgorithmName() override;
	virtual std::string GetTimeTaken() override;
	virtual std::string GetTotalCost() override;
	virtual std::string GetTilesExplored() override;

private:
	Grid* m_pGrid = nullptr;
	Scene* m_pScene = nullptr;

	IAlgorithm* m_pCurrentAlgorithm = nullptr;
	BreadthFirstSearch* m_pBreathFirstSearch = nullptr;
	DepthFirstSearch* m_pDepthFirstSearch = nullptr;
	DijkstrasAlgorithm* m_pDijkstrasAlgorithm = nullptr;
	Astar* m_pAstar = nullptr;
	
private:
	std::string m_previousTime;
};