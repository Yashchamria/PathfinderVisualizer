#pragma once

enum class TileType : char
{
	Default,
	StartTile,
	EndTile,
	WallTile,
	InValid,
};

enum class TileAnimationState : char
{
	Idle,
	Processing,
	Processed,
	Found,
};

enum class NeighbourTileDirection : char
{
	Up,
	Down,
	Right,
	Left,
};