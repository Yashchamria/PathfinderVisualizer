#pragma once

//Interface class to encapsulate different algorithms as object.
//Based on the Command design pattern.

class IAlgorithm
{
public:
	virtual bool Execute() = 0;
	virtual bool PlayVisualization(float switchTime, float deltaTime) = 0;

	virtual void Stop() = 0;
};