#pragma once

struct AlgorithmData
{
private:
	std::string m_Name = "";
	std::string m_TimeTaken = "";
	std::string m_PathCost = "";
	std::string m_TilesExplored = "";

public:
	AlgorithmData(std::string name = "--", std::string timeTaken = "--", std::string pathCost = "--", std::string tilesExplored = "--")
	{
		m_Name			= name;
		m_TimeTaken		= timeTaken;
		m_PathCost		= pathCost;
		m_TilesExplored = tilesExplored;
	}

public:
	void SetName(std::string name)					 { m_Name = name; }
	void SetTimeTaken(std::string timeTaken)		 { m_TimeTaken = timeTaken; }
	void SetPathCost(std::string pathCost)			 { m_PathCost = pathCost; }
	void SetTilesExplored(std::string tilesExplored) { m_TilesExplored = tilesExplored; }

	std::string GetName()			{ return m_Name; }
	std::string GetTimeTaken()		{ return m_TimeTaken; }
	std::string GetPathCost()		{ return m_PathCost; }
	std::string GetTilesExplored()  { return m_TilesExplored; }

};

enum class AlgorithmType : char
{
	Dijkstra,
	DepthFirstSearch,
	BreadthFirstSearch,
};

enum class AlgorithmVisualSpeed
{
	Slow = 16,
	Average = 48,
	Fast = 144,
	SuperFast = 296,
	Instant = 400,
};

inline std::string VisualSpeedToString(AlgorithmVisualSpeed speed)
{
	switch (speed)
	{
	case AlgorithmVisualSpeed::Slow :		return "Slow";
	case AlgorithmVisualSpeed::Average:	    return "Average";
	case AlgorithmVisualSpeed::Fast:	    return "Fast";
	case AlgorithmVisualSpeed::SuperFast:	return "Super Fast";
	case AlgorithmVisualSpeed::Instant:		return "Instant";
	}

	return "Invalid Speed";
}

enum class AlgorithmState : char
{
	UnExecuted,
	Executing,
	Executed,
	Visualizing,
	Visualized,
};