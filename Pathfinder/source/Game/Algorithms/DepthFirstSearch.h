#pragma once

#include "IAlgorithm.h"

class Grid;
class Tile;
enum class TileAnimationState : char;
enum class AlgorithmState : char;
enum class NeighbourTileDirection : char;

class DepthFirstSearch : public IAlgorithm
{
public:
	DepthFirstSearch(Grid* pGrid);
	~DepthFirstSearch();

	virtual bool Execute(AlgorithmType algorithmType) override;
	virtual bool PlayVisualization(float speed, float deltaTime) override;
	virtual void Stop() override;
	virtual AlgorithmState GetAlgorithmState() override { return m_AlgorithmState; }
	virtual bool IsPathFound() override { return m_pathfound; }

private:
	void Init();

private:
	Grid* m_pGrid = nullptr;

	std::stack<Tile*> m_pOpenTiles; //Requires LIFO
	std::unordered_map<Tile*, Tile* > m_ClosestPreviousTile;
	std::unordered_map<Tile*, bool > m_IsTileVisited;

	void AddToTileAnimationArray(Tile* pTile, TileAnimationState tileAnimation);

	//This list will store all the animations in sequence. Will be used to visualize the algorithm at a much slower pace.
	std::queue<std::pair<Tile*, TileAnimationState>> m_PendingTileAnimation;

private:
	void GetFinalPathAnimationSequence();
	Tile* ProcessNextTile();

	void AddToOpenlist(Tile* pTile);
	Tile* GetNeighbourTile(Tile* pCurrentTile, NeighbourTileDirection tileDirection);

	
private:
	bool m_stopExecution = false;
	bool m_pathfound = false;
	float m_switchSpeed = 3.0f;

	std::string m_algorithmDuration;
	AlgorithmState m_AlgorithmState;
	unsigned int m_tilesExplored = 0;
	unsigned int m_pathCost = 0;

public:
	virtual std::string GetAlgorithmName() override { return "Depth First Search"; }
	virtual std::string GetTimeTaken() override { return m_algorithmDuration; }
	virtual std::string GetTotalCost() override { return (std::to_string(m_pathCost)) + "(Unweighted Algorithm)"; }
	virtual std::string GetTilesExplored() override { return (std::to_string(m_tilesExplored)); }
};