#pragma once

struct AlgorithmData
{
public:
	std::string Name {"--"};
	std::string TimeTaken { "--" };
	std::string PathCost { "--" };
	std::string TilesExplored { "--" };

public:
	AlgorithmData(const std::string& name = "--", const std::string& timeTaken = "--",
		const std::string& pathCost = "--", const std::string& tilesExplored = "--") :
		Name(name),
		TimeTaken(timeTaken),
		PathCost(pathCost),
		TilesExplored(tilesExplored)
	{
	}
};

enum class AlgorithmType : char
{
	BreadthFirstSearch,
	DepthFirstSearch,
	Dijkstra,
	AStar,
};

//#define Slow 16
//#define Average 48
//#define Fast 144
//#define SuperFast 296
//#define Instant 400

enum class VisualSpeed
{
	Slow = 16,
	Average = 48,
	Fast = 144,
	SuperFast = 296,
	Instant = 400,
};

enum class AlgorithmState : char
{
	UnExecuted,
	Executing,
	Executed,
	Visualizing,
	Visualized,
};