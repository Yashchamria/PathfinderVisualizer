#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;
enum class TileAnimationState : char;
enum class AlgorithmState : char;

class Astar : public IAlgorithm
{
public:
	Astar(Grid* pGrid);
	~Astar();

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
	std::unordered_map<Tile*, std::pair<unsigned int, unsigned int>> m_finalCost;
	std::unordered_map<Tile*, Tile* > m_ClosestPreviousTile;
	std::unordered_map<Tile*, bool > m_IsTileVisited;

	//This list will store all the animations in sequence. Will be used to visualize the algorithm at a much slower pace.
	std::queue<std::pair<Tile*, TileAnimationState>> m_PendingTileAnimation;

private:
	void ProcessNeighbourTiles(Tile* pTile);
	void ProcessTileParameters(Tile* pTile, Tile* pPreviousTile);
	void AddToTileAnimationArray(Tile* pTile, TileAnimationState tileAnimation);
	void GetFinalPathAnimationSequence();
	Tile* GetPriorityTile(); 
	unsigned int GetTileHCost(Tile* pTile);
	unsigned int m_averageTileWeight = 0;

private:
	bool m_stopExecution = false;
	bool m_pathfound = false;
	float m_switchSpeed = 3.0f;

//Algorithm Info Variables
private:
	std::string m_algorithmDuration;
	AlgorithmState m_AlgorithmState;
	unsigned int m_tilesExplored = 0;
	unsigned int m_pathCost = 0;

public:
	virtual std::string GetAlgorithmName() override { return "Astar Algorithm"; }
	virtual std::string GetTimeTaken() override { return m_algorithmDuration; }
	virtual std::string GetTotalCost() override { return (std::to_string(m_pathCost)); }
	virtual std::string GetTilesExplored() override { return (std::to_string(m_tilesExplored)); }
};