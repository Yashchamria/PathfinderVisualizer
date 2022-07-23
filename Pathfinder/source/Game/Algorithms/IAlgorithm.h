//Interface class to encapsulate different algorithms as object.
//Based on the Command design pattern.

#pragma once

enum class AlgorithmType : char;
enum class AlgorithmState : char;


class IAlgorithm
{
public:
	virtual bool Execute(AlgorithmType algorithmType) = 0;
	virtual bool PlayVisualization(int speed, float deltaTime) = 0;
	virtual void Stop() = 0;
	virtual bool IsPathFound() = 0;

	virtual AlgorithmState GetAlgorithmState() = 0;
	virtual std::string GetAlgorithmName() = 0;
	virtual std::string GetTimeTaken() = 0;
	virtual std::string GetTotalCost() = 0;
	virtual std::string GetTilesExplored() = 0;
};