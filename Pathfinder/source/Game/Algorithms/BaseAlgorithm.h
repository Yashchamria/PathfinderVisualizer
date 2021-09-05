#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;
enum class TileAnimationState : char;
enum class AlgorithmState : char;
enum class NeighbourTileDirection : char;

class BaseAlgorithm : public IAlgorithm
{
public:
	BaseAlgorithm(Grid* pGrid);
	~BaseAlgorithm();

	virtual bool Execute(AlgorithmType algorithmType) override;
	virtual bool PlayVisualization(float speed, float deltaTime) override;
	virtual void Stop() override;

	virtual bool IsPathFound() override { return m_pathfound; }
	virtual AlgorithmState GetAlgorithmState() override { return m_AlgorithmState; }

public:
	virtual std::string GetAlgorithmName() override { return "Base Algorithm"; }
	virtual std::string GetTimeTaken() override { return m_algorithmDuration; }
	virtual std::string GetTotalCost() override { return (std::to_string(m_pathCost)); }
	virtual std::string GetTilesExplored() override { return (std::to_string(m_tilesExplored)); }

public:
	void Init();
	virtual void OnInit() = 0;
	virtual void OnExecute() = 0;
	virtual void OnStop() = 0;

private:
	Grid* m_pGrid = nullptr;

private:
	bool m_stopExecution = false;
	bool m_pathfound = false;
	float m_switchSpeed = 3.0f;

	std::string m_algorithmDuration;
	AlgorithmState m_AlgorithmState;
	unsigned int m_tilesExplored = 0;
	unsigned int m_pathCost = 0;


private:
	std::unordered_map<Tile*, Tile* > m_ClosestPreviousTile;
	std::queue<std::pair<Tile*, TileAnimationState>> m_PendingTileAnimation;

public:
	Grid* GetGrid() { return m_pGrid; }
	unsigned int GetPathCost() { return m_pathCost; }
	bool IsExecutionStopped() { return m_stopExecution; }

	void SetPathFound(bool pathfound) { m_pathfound = pathfound; }
	void IncrementTileExplored() { m_tilesExplored++; }

public:
	void AddToClosestPreviousTile(Tile* pTile, Tile* pPreviousTile);
	void AddToTileAnimationArray(Tile* pTile, TileAnimationState tileAnimation);

private:
	void GetFinalPathAnimationSequence();

};