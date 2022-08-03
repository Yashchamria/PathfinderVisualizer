#pragma once
#include <string>

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

enum class AlgorithmState : char
{
	UnExecuted,
	Executing,
	Executed,
	Visualizing,
	Visualized,
};