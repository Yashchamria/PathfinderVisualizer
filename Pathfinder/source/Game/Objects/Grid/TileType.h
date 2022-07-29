#pragma once

enum class TileType : char
{
	Default,
	StartTile,
	EndTile,
	WallTile
};

enum class TileAnimState : char
{
	Idle,
	Processing,
	Processed,
	Found
};