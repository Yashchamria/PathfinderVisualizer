#pragma once

//Interface class to encapsulate different algorithms as object.
//Based on the Command design pattern.

class IAlgorithm
{
public:
	virtual void Execute() = 0;
};