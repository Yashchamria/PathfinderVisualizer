#pragma once
#include <string>

#define InvalidData "--"

struct AlgorithmData
{
public:
	std::string Name {InvalidData};
	std::string TimeTaken {InvalidData};
	std::string PathCost {InvalidData};
	std::string TilesExplored {InvalidData};

public:
	AlgorithmData(const std::string& name = InvalidData, const std::string& timeTaken = InvalidData,
		const std::string& pathCost = InvalidData, const std::string& tilesExplored = InvalidData) :
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