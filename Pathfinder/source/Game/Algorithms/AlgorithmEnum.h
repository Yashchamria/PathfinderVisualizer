#pragma once

enum class AlgorithmType : char
{
	Dijkstra,
};

enum class AlgorithmVisualSpeed
{
	Slow = 8,
	Average = 24,
	Fast = 72,
	Instant = 216,
};

enum class AlgorithmState : char
{
	Executing,
	Executed,
	Visualizing,
};