#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;
enum class TileAnimationState : char;
enum class AlgorithmState : char;

class BreadthFirstSearch : public IAlgorithm
{
public:
	BreadthFirstSearch(Grid* pGrid);
	~BreadthFirstSearch();

	virtual bool Execute(AlgorithmType algorithmType) override;
	virtual bool PlayVisualization(float speed, float deltaTime) override;
	virtual void Stop() override;
	virtual AlgorithmState GetAlgorithmState() override { return m_AlgorithmState; }
	virtual bool IsPathFound() override { return m_pathfound; }

private:
	void Init();

private:
	Grid* m_pGrid = nullptr;

	std::vector<Tile*> m_pOpenTiles;
	std::unordered_map<Tile*, Tile* > m_ClosestPreviousTile;
	std::unordered_map<Tile*, bool > m_IsTileVisited;

	//This list will store all the animations in sequence. Will be used to visualize the algorithm at a much slower pace.
	std::queue<std::pair<Tile*, TileAnimationState>> m_PendingTileAnimation;

private:
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);
	void GetFinalPathAnimationSequence();
	Tile* GetPriorityTile();

private:
	bool m_stopExecution = false;
	bool m_pathfound = false;
	float m_switchSpeed = 3.0f;

	std::string m_algorithmDuration;
	AlgorithmState m_AlgorithmState;
	unsigned int m_tilesExplored = 0;
	unsigned int m_pathCost = 0;

public:
	virtual std::string GetAlgorithmName() override { return "Breadth First Search"; }
	virtual std::string GetTimeTaken() override { return m_algorithmDuration; }
	virtual std::string GetTotalCost() override { return (std::to_string(m_pathCost)) + "(Unweighted Algorithm)"; }
	virtual std::string GetTilesExplored() override { return (std::to_string(m_tilesExplored)); }
};