#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;
enum class TileAnimationState : char;
enum class AlgorithmState : char;

class DijkstrasAlgorithm : public IAlgorithm
{
public:
	DijkstrasAlgorithm(Grid* pGrid);
	~DijkstrasAlgorithm();

	virtual bool Execute(AlgorithmType algorithmType) override;
	virtual bool PlayVisualization(float speed, float deltaTime) override;
	virtual void Stop() override;
	virtual AlgorithmState GetAlgorithmState() override;
	virtual bool IsPathFound() override { return m_pathfound; }
	
private:
	void Init();

private:
	Grid* m_pGrid = nullptr;

	std::vector<Tile*> m_pOpenTiles;
	std::map<Tile*, unsigned int > m_totalCostfromStartTile;
	std::map<Tile*, Tile* > m_ClosestPreviousTile;
	std::map<Tile*, bool > m_IsTileVisited;

	//This list will store all the animations in sequence. Will be used to visualize the algorithm at a much slower pace.
	std::vector<std::pair<Tile*, TileAnimationState>> m_PendingTileAnimation;

private:
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);
	void GetFinalPathAnimationSequence();
	Tile* GetPriorityTile(); 

	void Cleanup();

private:
	bool m_stopExecution = false;
	bool m_pathfound = false;
	unsigned int m_pendingAnimationIndex = 0;
	float m_switchSpeed = 3.0f;

	std::string m_algorithmDuration;
	AlgorithmState m_AlgorithmState;
	unsigned int m_tilesExplored = 0;
	unsigned int m_pathCost = 0;

public:
	virtual std::string GetAlgorithmName() override { return "Dijkstra's Algorithm"; }
	virtual std::string GetTimeTaken() override { return m_algorithmDuration; }
	virtual std::string GetTotalCost() override { return (std::to_string(m_pathCost)); }
	virtual std::string GetTilesExplored() override { return (std::to_string(m_tilesExplored)); }
};