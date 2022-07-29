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