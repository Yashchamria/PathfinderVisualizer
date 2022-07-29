#pragma once

enum class Direction : char
{
	Up,
	Down,
	Right,
	Left
};

static sf::Vector2u GetNeighborCoord(sf::Vector2u coord, Direction direction)
{
	switch (direction)
	{
		case Direction::Up:    --coord.y; break;
		case Direction::Down:  ++coord.y; break;
		case Direction::Left:  --coord.x; break;
		case Direction::Right: ++coord.x; break;
	}
	return coord;
}